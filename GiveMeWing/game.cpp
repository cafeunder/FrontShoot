#include <DxLib.h>
#include <math.h>
#include "input.h"
#include "mychar.h"
#include "main.h"
#include "game.h"
#include "database.h"
#include "image.h"
#include "stage.h"
#include "operate.h"
#include "sound.h"
#include "efect.h"
#include "menu.h"

/*
クラス名 : Game
	ゲーム処理本体

	記述ファイル : game.h
*/

Game::Game(int stg_index, int a_glevel, bool a_thr)
{
	f_count = 0;
	count = 0;
	game_sta = GAME_START;
	stage_state = stg_index;
	game_level = a_glevel;
	through = a_thr;

	ope_area = new OperateArea();
	ope_area->StartUp();

	stage = new Stage(stg_index, ope_area, game_level);
	mychar = new MyCharacter(GAME_WIDTH/2-7.0, 440.0, 2, 2, 80, 50, 6, IMG_MYCHARACTER);

	pause = NULL;
	retry = NULL;
	g_clear = NULL;
}

void Game::Retry()
{
	delete stage;
	delete mychar;
	delete ope_area;
	delete pause;
	delete g_clear;
	game_level--;	//リトライ時は難易度を１つ下げる
	if(game_level < 0) game_level = 0;

	f_count = 0;
	count = 0;
	game_sta = GAME_START;

	ope_area = new OperateArea();
	ope_area->StartUp();

	stage = new Stage(stage_state, ope_area, game_level);
	mychar = new MyCharacter(GAME_WIDTH/2-7.0, 440.0, 2, 2, 80, 50, 6, IMG_MYCHARACTER);

	pause = NULL;
	retry = NULL;
}

Game::~Game()
{
	delete stage;
	delete mychar;
	delete ope_area;
	delete pause;
	delete retry;
}



int Game::Update()
{
	switch(game_sta){
	case GAME_START :	//スタート状態なら、
		//スタート画面更新
		if(StartUpdate() == 1) {
			game_sta = GAME_MAIN;
		}
		//戻り値が1ならゲームメイン状態へ
		break;

	case GAME_MAIN :	//ゲームメイン状態なら、
		switch(MainUpdate()){	//ゲームメイン更新
		case 1 : 
			game_sta = GAME_OVER; 
			ope_area->Reset();
			break;	//戻り値が1ならゲームオーバー
		case 2 : 
			mychar->ReleasePoint(); 
			mychar->AddScore((mychar->GetLife()-1)*LEFT_BONUS);
			mychar->ReleasePoint();
			game_sta = STAGE_CLEAR;
			count = 0;
			break;	//戻り値が2ならゲームクリア
		case 3 : return 3;
		}
		break;

	case GAME_OVER :	//ゲームオーバー状態なら、
		//ゲームオーバー画面を更新
		if(OverUpdate() == 1) {
			
			if(!through) return 1;

			game_sta = GAME_RETRY;
			count = 0;
			retry = new MyRetry();
		}
		//戻り値が1ならゲーム終了
		break;

	case GAME_RETRY:
		switch(retry->Update()){
		case 0 : 
			Retry();
			break;
		case 1 : delete retry; retry=NULL; return 1;
		}

		break;

	case STAGE_CLEAR :
		if(StageClearUpdate() == 1) game_sta = NEXT_STATE;
		break;

	case NEXT_STATE :

		delete stage;	//ステージオブジェクトを破棄して
		stage = NULL;	//NULLを詰める
		
		stage_state = stg_db.QueryStageData(stage_state)->next_state;
			//次状態をクエリ
		if(stage_state == STAGEST_CLEAR) {
			g_clear = new GameClear(mychar->GetScore());
			game_sta = GAME_CLEAR; 
			break;
		}
			//次状態がゲームクリアなら、ゲーム終了
			
			//そうでないなら
		ope_area->Reset();	//オペレートエリアを初期化
		
		if(mychar->GetLife() >= 5) game_level++;	//残機が４以上ならレベルアップ
		else if(mychar->GetLife() >= 3) game_level = game_level;	//そうでなくて、２以上なら据え置き
		else game_level--;	//そうでもないなら、レベルダウン

		if(game_level < 0) game_level = 0;
		if(game_level > 2) game_level = 2;

		mychar->Initialize();
		stage = new Stage(stage_state, ope_area, game_level);	//次のステージを作成
		count = 0;	//カウントを初期化して
		game_sta = GAME_START;	//ゲームスタート状態に
		break;
	case GAME_CLEAR :
		if(g_clear->Update() == 1) return 2;
		break;
	}

	if(pause == NULL) ope_area->Update();	//オペレーションエリアは常に更新

	return 0;
}

void Game::DrawSystem()
{
	image.DrawImage(0,0,IMG_BOARD,FALSE,0,FALSE);	//ボードの描画

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	image.DrawImage(404+232/2-156/2,4+166/2-66/2,IMG_STSLMEASY+game_level,TRUE,0,FALSE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 100);

	DrawState();//ステータス描画
	ope_area->Draw();//オペレーションエリア描画

	image.DrawImage(0,0,IMG_DISPLAY,TRUE,0,FALSE);

	DrawBox(GAME_X,GAME_Y,GAME_X+GAME_WIDTH,GAME_Y+GAME_HEIGHT,GetColor(10,10,10),TRUE);

	image.DrawImage(0,0,IMG_DISPLAYFRAME,TRUE,0,FALSE);
}

void Game::DrawScoreLife()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);	//ブレンドモードをアルファに
	//スコアの描画
	int score = mychar->GetScore();	//スコアを変数に記憶
	int scrnum_han = img_db.QueryImgData(IMG_SCORENUM)->img_han;
	for(int i = 0; i < 8; i++){	//ループを８回回す（＝８桁で表示する
		int num = score%10;	//変数の１桁目を取得
		score /= 10;	//桁をひとつへらす

		DrawRectGraph(140+30*(7-i),0,30*num,0,30,50,scrnum_han,TRUE,FALSE);
			//１桁目の数に対応した画像を表示
	}
	//
	
	//シールドの描画
	int strshield_han = img_db.QueryImgData(IMG_STRSHIELD)->img_han;
	int strshield_width, strshield_height;
	GetGraphSize(strshield_han, &strshield_width, &strshield_height);
	DrawGraph(GAME_X,480-strshield_height, strshield_han, TRUE);

	DrawRectGraph(GAME_X+strshield_width+20,480-strshield_height,30*(mychar->GetLife()-1),0,30,50,scrnum_han,TRUE,FALSE);
	//
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 80);
}

void Game::Draw()
{
	//状態に合わせて
	switch(game_sta){
	case GAME_START :
		StartDraw();
		break;
	case GAME_MAIN :
		MainDraw();
		break;
	case GAME_OVER :
		OverDraw();
		break;
	case GAME_RETRY :
		retry->Draw();
		break;
	case STAGE_CLEAR :
		StageClearDraw();
		break;
	case GAME_CLEAR :
		g_clear->Draw();
		break;
	}
	//描画
}



int Game::StartUpdate()
{

#ifdef DEBUG_CODE
	return 1;
#endif
	if(count == 0){
		efect.SetEfect(0,0,EFECT_STRCALLSLOW,img_db.QueryImgData(IMG_MESBOARD)->img_han,img_db.QueryImgData(IMG_STRSTAGE1+stage_state)->img_han);
		sound.PlaySoundHandle(SOUNDEFECT_STARTEFECT);
	}
	if(count == SLOWCALL_COUNT){
		efect.SetEfect(0,0,EFECT_STRCALLFAST,img_db.QueryImgData(IMG_ASRMESBOARD)->img_han,img_db.QueryImgData(IMG_STRREADY)->img_han);
	}
	if(count >= SLOWCALL_COUNT){
		sound.ChangeVolumeSound(255-255*(count-SLOWCALL_COUNT)/FASTCALL_COUNT,SOUNDEFECT_STARTEFECT);
	}

	if(count >= FASTCALL_COUNT+SLOWCALL_COUNT){
			//ステージコールとレディコールに十分なカウントが経過したら
		sound.StopSoundHandle(SOUNDEFECT_STARTEFECT);
		count = 0;	//カウントを初期化
		return 1;	//次状態へ
	}

	count++;
	return 0;
}

void Game::StartDraw()
{
	DrawSystem();
	if(count <= 210){	//カウントが210以下なら、ステージ表示中
		SetDrawBright(count*3, count*3, count*3);	//輝度を設定してフェードイン
		stage->DrawBackGround(count);	//背景の描画
		SetDrawBright(255,255,255);	//輝度をもとにもどす
	}
	DrawScoreLife();
}



#define PAUSEMENU_ITEMNUM 2
char* PAUSEMENU_ITEM[PAUSEMENU_ITEMNUM] = {"ゲームに戻る","タイトルへ"};
int Game::MainUpdate()
{
	if(input.GetInput( INPUT_MENU ) == 1){
		if(pause == NULL)
			pause = new Menu(200,200,PAUSEMENU_ITEM,PAUSEMENU_ITEMNUM, TRUE, FALSE, TRUE);
		else {
			delete pause;
			pause = NULL;
		}
	}

	int result = 0;

	if(pause != NULL){
		switch(pause->Update()){
		case 0 : delete pause; pause=NULL; break;
		case -2: delete pause; pause=NULL; break;
		case 1 : delete pause; pause=NULL; stage->StopStageSound(); return 3;
		}
	} else {
		result = stage->Update(mychar, ope_area);
		count++;
	}
		//ステージの更新

	if(result != 0) {
		f_count = count;
		count = 0;
	}
		//ステージ更新の戻り値が0以外なら、カウントを初期化

	return result;
}

void Game::MainDraw()
{
	if(pause != NULL) pause->Draw();
	else {
		DrawSystem();
		stage->Draw(mychar, count);	//ステージの描画
		DrawScoreLife();
	}
}



int Game::OverUpdate()
{
	if(count == 0){
		efect.SetEfect(0,0,EFECT_STRCALLSLOW,img_db.QueryImgData(IMG_ASRMESBOARD)->img_han,img_db.QueryImgData(IMG_STRGAMEOVER)->img_han);
	}

	if(count >= SLOWCALL_COUNT){
		return 1;
	}
		//ゲームオーバー描画に必要なカウントを経過していたら、ゲーム終了

	count++;
	return 0;
}

void Game::OverDraw()
{
	DrawSystem();

	SetDrawBright(255-count*3, 255-count*3, 255-count*3);	//輝度を設定してフェードアウト
	stage->Draw(mychar, count);	//ステージの描画は続ける
	SetDrawBright(255,255,255);	//輝度を元に戻す

	DrawScoreLife();
}


int Game::GameClear::Update()
{
	count++;

	if(count == 240) return 1;
	return 0;
}

void Game::GameClear::Draw()
{
	int game_cx = WINDOWSIZE_X/2;
	int game_3y = WINDOWSIZE_Y/3;

	int img_w, img_h;
	image.GetImageSize(IMG_STRGCLEAR, &img_w, &img_h);
	int imgy_w, imgy_h;
	image.GetImageSize(IMG_STRYOURS, &imgy_w, &imgy_h);

	if(count < 30) {
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, (int)((double)count/30 * 255));
	}

	image.DrawImage(game_cx - img_w/2, game_3y - img_h/2, IMG_STRGCLEAR, TRUE, 0);
	image.DrawImage(game_cx - imgy_w, game_3y+img_h, IMG_STRYOURS, TRUE, 0);

	int bonus = score;	//スコアを変数に記憶
	for(int i = 0; i < 8; i++){	//ループを８回回す（＝８桁で表示する
		int num = bonus%10;	//変数の１桁目を取得
		bonus /= 10;	//桁をひとつへらす

		image.DrawImage(game_cx + 30 + 17*(7-i),game_3y+img_h+5,IMG_BONUSNUM,TRUE,num);
			//１桁目の数に対応した画像を表示
	}

	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0);
}

Game::MyRetry::MyRetry()
{
	select = 0;
	count = 0;
	decide = FALSE;
}

int Game::MyRetry::Update()
{	
	if(decide){
		if(count > 0) {count--; return -1;}
		return select;
	}

	if(input.GetInput( INPUT_LEFT ) == 1){
		select--;
		if(select < 0) select = 0;
	}
	if(input.GetInput( INPUT_RIGHT ) == 1){
		select++;
		if(select > 1) select = 1;
	}

	if(input.GetInput( INPUT_SHOT ) == 1){
		decide = TRUE;
		count = 60;
	}

	return -1;
}

void Game::MyRetry::Draw()
{
	if(decide) SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(255*((double)count/60)));
	image.DrawImage(0,GAME_HEIGHT/2-50,IMG_STRRETRY,TRUE,0);
	image.DrawImage(0,GAME_HEIGHT/2+20,IMG_STRYESNO,TRUE,0);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if(!decide){
		if(select == 0){
			image.DrawImage(90,GAME_HEIGHT/2+20,IMG_CURSOR,TRUE,0);
		} else {
			image.DrawImage(260,GAME_HEIGHT/2+20,IMG_CURSOR,TRUE,0);
		}
	}
}



#define STAGECLEARSTATE_COUNT (230+FASTCALL_COUNT)
int Game::StageClearUpdate()
{
	mychar->Update();
	count++;

	if(count == 230 && stg_db.QueryStageData(stage_state)->next_state != STAGEST_CLEAR){
		efect.SetEfect(0,0,EFECT_STRCALLFAST,img_db.QueryImgData(IMG_MESBOARD)->img_han,img_db.QueryImgData(IMG_STRNEXTSTAGE)->img_han);
	}

	if(count > STAGECLEARSTATE_COUNT) {
		return 1;
	}
	return 0;
}

void Game::StageClearDraw()
{
	DrawSystem();

	int game_cx = GAME_WIDTH/2;
	int game_3y = GAME_HEIGHT/3;

	int img_w, img_h;
	image.GetImageSize(IMG_STRSTAGECLEAR, &img_w, &img_h);

	if(count >= 230) return;
	if(count >= 200){
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255 - (int)((double)(count-200)/30*255));
	}


	stage->DrawBackGround(f_count+count);
	mychar->Draw(f_count+count);

	if(count < 30) {
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, (int)((double)count/30 * 255));
	}

	image.DrawImage(game_cx - img_w/2, game_3y - img_h/2, IMG_STRSTAGECLEAR, TRUE, 0);
	image.DrawImage(20, game_3y+img_h, IMG_STRLEFTBONUS, TRUE, 0);

	int bonus = (mychar->GetLife()-1)*LEFT_BONUS;	//スコアを変数に記憶
	for(int i = 0; i < 8; i++){	//ループを８回回す（＝８桁で表示する
		int num = bonus%10;	//変数の１桁目を取得
		bonus /= 10;	//桁をひとつへらす

		image.DrawImage(178+30+17*(7-i)+20,game_3y+img_h+5,IMG_BONUSNUM,TRUE,num);
			//１桁目の数に対応した画像を表示
	}

	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0);

	DrawScoreLife();
}



void Game::DrawState()
{
	//ポイントの描画
	int point = mychar->GetPoint();	//スコアを変数に記憶

	for(int i = 0; i < 8; i++){	//ループを８回回す（＝８桁で表示する
		int num = point%10;	//変数の１桁目を取得
		point /= 10;	//桁をひとつへらす

		image.DrawImage(510+15*(7-i),16,IMG_STATENUM,TRUE,num,FALSE);
			//１桁目の数に対応した画像を表示
	}
	//

	//レートの表示
	double rate = mychar->GetRate();	//レートを変数に記憶
	int rate_int = (int)rate;			//レートの整数部分を取り出す
	for(int i = 0; i < 2; i++){	//ループを２回回す（＝２桁で表示する
		int num = rate_int%10;	//変数の１桁目を取得
		rate_int /= 10;	//桁をひとつへらす

		image.DrawImage(510+15*(1-i),47,IMG_STATENUM,TRUE,num,FALSE);
			//１桁目の数に対応した画像を表示
	}
	int rate_dec = (int)(rate*10)%10;	//レートの小数第１位をとりだす
	image.DrawImage(510+35,47,IMG_STATENUM,TRUE,rate_dec,FALSE);
	//

	//ショットレベルの描画
	image.DrawImage(510,80,IMG_STATENUM,TRUE,mychar->GetShotLv()+1,FALSE);
	//

	//経験値バーの描画
	DrawBox(509,106,611,118,GetColor(255,255,255),FALSE);
	DrawBox(510,107,510+100,117,GetColor(64,0,0),TRUE);
	DrawBox(510,107,510+(int)(100*mychar->GetShotExpPer()),117,GetColor(0,200,255),TRUE);
	//

	//チャージバーの描画
	DrawBox(509,134,611,146,GetColor(255,255,255),FALSE);
	DrawBox(510,135,510+100,145,GetColor(64,0,0),TRUE);
	int color = GetColor(0,200,255); 
	if(mychar->JudgeMaxCharge()){
		if(count%20 > 10) color = GetColor(255,255,255);
		else color = GetColor(255, 255, 0);
	}
	DrawBox(510,135,510+(int)(100*mychar->GetChargePer()),145,color,TRUE);
	//

	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 63);
	//tag
	DrawBox(405, 4+(count*2)%166, 405+232, 4+(count*2)%166+5, GetColor(60,60,60),TRUE);
	DrawBox(405, 4+(count*2)%166+7, 405+232, 4+(count*2)%166+10, GetColor(60,60,60),TRUE);

	DrawBox(405, 4+(count*2)%166+15, 405+232, 4+(count*2)%166+15+2, GetColor(60,60,60),TRUE);
	DrawBox(405, 4+(count*2)%166+19, 405+232, 4+(count*2)%166+19+1, GetColor(60,60,60),TRUE);
	//砂嵐の画像を作ってそれを表示


	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 80);
		//ブレンドモードを元に戻す
}

double SinFract(double point, double count, double cycle, double fract){
	 return sin(point*((count)/cycle))*fract - sin(point*((count-1)/cycle))*fract;
}

