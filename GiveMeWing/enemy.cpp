#include <DxLib.h>
#include <math.h>
#include "image.h"
#include "enemy.h"
#include "main.h"
#include "database.h"
#include "mychar.h"
#include "shot.h"
#include "ene_ope.h"
#include "item.h"
#include "satellite.h"
#include "efect.h"
#include "sound.h"

/*
クラス名 : Enemy
	エネミー（敵）を表すクラス

	継承 : Character

	記述ファイル : enemy.h
*/

Enemy::Enemy(double a_x, double a_y, int a_enc,int a_sinterval,int s_index, int sm_index, int a_brake, double a_velo, int a_move, int a_shot, double a_svelo, double a_angle, int data_index):Character(a_x, a_y, 0, 0, 0, 0)
{
	state = ENEST_STANDBY;
	count = 0;
	rapid_num = 0;
	flash_count = 0;
	damaged = FALSE;

	px = a_x;
	py = a_y;
	encount = a_enc;
	move_type = a_move;
	shot_type = a_shot;
	shot_interval = a_sinterval;
	shot_index = s_index;
	shotmove_index = sm_index;
	brake_count = a_brake;
	velocity = a_velo;
	shot_velo = a_svelo;
	angle = a_angle;

	EnemyData_t* e_data = ene_db.QueryEnemyData(data_index);

	width = e_data->width;
	height = e_data->height;
	life = e_data->life;

	img_index = e_data->img_num;	//データベースよりイメージハンドルを受け取る
	image.GetImageSize(img_index, &img_w, &img_h);	//グラフサイズの取得
	img_w/=img_db.QueryImgData(img_index)->div_num;
	img_div = 0;

	score = e_data->score;
	turn = e_data->turn;
	item_value = e_data->value;
	col_type = e_data->col_type;
	draw_efect = e_data->efect;
	efect_par = e_data->efe_par;
	counter = e_data->counter;
	homed = e_data->homed;

	//ショットインターバルが-1なら、ショットを自発的に撃たないということ。
	if(shot_interval == -1) interval_count = -1;
	else interval_count = 0;
}

#define FLASH_COUNT 2
void Enemy::Update(ShotMgr* sht_mgr, MyCharacter* mychar, ItemMgr* item_mgr, int g_level)
{
	int move_result;
	//動作制御関数呼び出し
	move_result = EneMove_Operate[move_type](this, mychar);

	if(turn == ENEDIR_MYCHAR){	//常にマイキャラを見るタイプであれば
		double y = mychar->GetPosY() - (py+height/2);	//マイキャラとエネミーのy距離を求める
		double x = mychar->GetPosX() - (px+width/2);	//x距離を求める
		angle = atan2(y,x);	//tan-1(x/y)　をもとめる
	}
	//移動制御関数から発射許可が出ていて、
	if((move_result == 1 && interval_count == 0) || move_result == 2) {
		Shot(sht_mgr, mychar);
	}
	//インターバルでなければショット処理

	//これらの制御関数は、 ファイル:ene_ope.cpp に記述してある

	if(col_type != ENECTYPE_ALLNOCOL){
		if(sht_mgr->ColCharCalc(this, SHOTSIDE_ENEMY)){
			flash_count = FLASH_COUNT;
			damaged = TRUE;
			if(counter == ENECOUNTER_COLSHOT && life > 0) Shot(sht_mgr, mychar, TRUE);
		}	//ショットとの当たり判定
	}

	if((px+width < 0) || (px > GAME_WIDTH) || (py+height < 0) || (py > GAME_HEIGHT)){
		//画面外に出ていれば、
		state = ENEST_DEAD;	//死亡
	}
	if(life <= 0) {
		state = ENEST_DEAD;	//体力がもうなければ死亡して

		item_mgr->SetItem(px+width/2, py+height/2, item_value);	//自壊時のアイテムをセット
		efect.SetEfect(px+width/2+GAME_X,py+height/2, EFECT_SMALLBURN, width, GetColor(255,0,0));	//爆破エフェクトセット
		if(score != 0) efect.SetEfect(px+width/2+GAME_X,py, EFECT_SCORE, score);
		mychar->AddScore(score);	//スコアをプラス

		sound.PlaySoundHandle(SOUNDEFECT_ENEBURN);

		if(counter == ENECOUNTER_DEAD) Shot(sht_mgr, mychar, TRUE);	//カウンターショット

		if(g_level == 2){	//ハードレベルなら
			if(GetRand(2) == 0){	//　1/3の確率でデスカウンター
				double y = mychar->GetPosY() - (py+height/2);	//マイキャラとエネミーのy距離を求める
				double x = mychar->GetPosX()+mychar->GetWidth()/2 - (px+width/2);	//x距離を求める
				double angle = atan2(y,x);	//tan-1(x/y)　をもとめる

				sht_mgr->SetShot(px+width/2, py+width/2, 7, angle, SHOT_ENECOUNTERSHOT, 0, SHOTSIDE_ENEMY, 0, FALSE);
			}
		}
	}

	if(interval_count > 0) interval_count--;
	if(flash_count > 0) flash_count--;
	count++;
}



void Enemy::Shot(ShotMgr* sht_mgr, MyCharacter* mychar, bool count_shot)
{
	int temp_stype = shot_index;
	if(count_shot) shot_index = SHOT_ENECOUNTERSHOT;

	if(shot_type != -1){
		switch(EneShot_Operate[shot_type](this, sht_mgr, mychar)){
		case 0 :
			rapid_num = 0;
			interval_count = shot_interval;
			break;
		case 1 : 
			rapid_num++;
			break;
		}
	}

	shot_index = temp_stype;	
}

void Enemy::Draw(MyCharacter* mychar)
{
	//当たり矩形が、画像の中心に来るように補正
	float draw_x = (float)px - (int)(img_w/2 - width/2);
	float draw_y = (float)py - (int)(img_h/2 - height/2);
	//

	switch(draw_efect){
	case EF_ALPHABLEND :
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, efect_par);
		break;
	case EF_ADDBLEND :
		SetDrawBlendMode(DX_BLENDMODE_ADD, efect_par);
		break;
	case EF_ANIME :
		if(count%efect_par == 0) img_div = (img_div+1)%img_db.QueryImgData(img_index)->div_num;
		break;
	case EF_ALPHAMOTION :
		efect.SetEfect(draw_x+(double)img_w/2, draw_y+(double)img_h/2, EFECT_ALPHAMOTION, img_index, efect_par, angle);
		break;
	}

	if(flash_count >= FLASH_COUNT/2) SetDrawBlendMode( DX_BLENDMODE_INVSRC, 255);
	if(turn != ENEDIR_NOTURN) {	
		image.DrawRotaImageF(draw_x+(int)(img_w/2), draw_y+(int)(img_h/2), 1.0, angle+PI/2, img_index, TRUE, img_div);
	} 	//回転描画は、画像の中心座標を指定する
	else image.DrawImageF(draw_x, draw_y, img_index, TRUE, img_div);
			//回転描画でなければそのまま描画
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#ifdef DEBUG_CODE
	_DrawBox(int(px), int(py), int(px)+width, int(py)+height, GetColor(0,0,255),FALSE);
	//あたり矩形範囲の表示
#endif
}



/*
クラス名 : EnemyMgr
	エネミーの管理クラス

	記述ファイル : enemy.h
*/

EnemyMgr::EnemyMgr(int num, EnemyInitData_t* init_data)
{
	ene_num = num;	//エネミー数を確保
	enemy = (Enemy**)malloc(sizeof(Enemy)*ene_num);	
		//エネミーの数だけ、エネミーポインタを動的確保（ポインタの配列

	for(int i = 0; i < ene_num; i++){
		enemy[i] = new Enemy(init_data[i].x0, init_data[i].y0, init_data[i].encount,
			init_data[i].shot_interval,init_data[i].shot_index,init_data[i].shotmove_index,
			init_data[i].brake_count,init_data[i].velocity,init_data[i].move_type,
			init_data[i].shot_type,init_data[i].shot_velo,init_data[i].angle, init_data[i].data_index);
				//エネミーオブジェクトのインスタンスを作成
	}
}

EnemyMgr::~EnemyMgr()
{
	for(int i = 0; i < ene_num; i++){
		delete enemy[i];
	}	//まず、エネミーオブジェクトを破棄してから
	free(enemy);	//オブジェクト配列を破棄
}

void EnemyMgr::Update(ShotMgr* sht_mgr, MyCharacter* mychar, ItemMgr* item_mgr, int count, int game_level)
{
	for(int i = 0; i < ene_num; i++){
		if(enemy[i]->GetState() == ENEST_STANDBY){
			//スタンバイ状態で
			if(enemy[i]->GetEncount() == count) enemy[i]->SetState(ENEST_EXIST);
				//かつ遭遇カウントを経過していたら存在させる
			else continue;//していないなら反復
		}
		if(enemy[i]->GetState() == ENEST_DEAD) continue;

		enemy[i]->Update(sht_mgr, mychar, item_mgr, game_level);
	}

}

bool EnemyMgr::Collition(int a_x, int a_w, int a_y, int a_h)
{
	for(int i = 0; i < ene_num; i++){
		if(enemy[i]->GetState() != ENEST_EXIST) continue;	//存在していないなら判定を行わない
		if(enemy[i]->GetColType() != ENECTYPE_NORMAL) continue;

		if(enemy[i]->Collition(a_x, a_w, a_y, a_h)) return TRUE;
	}
	return FALSE;
}

void EnemyMgr::Draw(int count, MyCharacter* mychar)
{
	for(int i = 0; i < ene_num; i++){
		if(enemy[i]->GetState() == ENEST_EXIST && !enemy[i]->GetOnTheGround()) enemy[i]->Draw(mychar);
			//存在しているなら普通に描画

		if(enemy[i]->GetState() == ENEST_STANDBY && enemy[i]->GetPosY() >= 480 
			&& enemy[i]->GetEncount()-100 <= count && !(enemy[i]->GetEncount() < count)){
				//画面下にスタンバイ中で、かつ登場が１００カウント以内なら
			if(enemy[i]->GetEncount()-100 == count){
				sound.PlaySoundHandle(SOUNDEFECT_ENEASERT);
			}

			//アサートの表示
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (count%30)*6);	//アルファブレンドで描画
			_DrawBox((int)enemy[i]->GetPosX(), 440, (int)enemy[i]->GetPosX()+enemy[i]->GetWidth(), 480, GetColor(255,0,0), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 126);
			//
		}
	}
}

void EnemyMgr::OnTheDraw(int count, MyCharacter* mychar)
{
	for(int i = 0; i < ene_num; i++){
		if(enemy[i]->GetState() == ENEST_EXIST && enemy[i]->GetOnTheGround()) enemy[i]->Draw(mychar);
			//存在しているなら普通に描画

		if(enemy[i]->GetState() == ENEST_STANDBY && enemy[i]->GetPosY() >= 480 
			&& enemy[i]->GetEncount()-100 <= count && !(enemy[i]->GetEncount() < count)){
				//画面下にスタンバイ中で、かつ登場が１００カウント以内なら

			//アサートの表示
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (count%30)*6);	//アルファブレンドで描画
			_DrawBox((int)enemy[i]->GetPosX(), 440, (int)enemy[i]->GetPosX()+enemy[i]->GetWidth(), 480, GetColor(255,0,0), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 126);
			//
		}
	}
}

void EnemyMgr::QueryNearEnemyPos(double px, double py, double* bf_x, double* bf_y)
{
	bool update = FALSE;

	for(int i = 0; i < ene_num; i++){
		if(enemy[i]->GetState() != ENEST_EXIST) continue;
			//存在していないなら判定しない
		if(!enemy[i]->GetHomed()) continue;

		if(!update){
			*bf_x = enemy[i]->GetPosX() + enemy[i]->GetWidth()/2;
			*bf_y = enemy[i]->GetPosY() + enemy[i]->GetHeight()/2;
			update = TRUE;
		}

		double ex = enemy[i]->GetPosX();
		double ey = enemy[i]->GetPosY();

		double df_epx = abs(ex - px);
		double df_epy = abs(ey - py);
		double df_bpx = abs(*bf_x - px);
		double df_bpy = abs(*bf_y - py);

		if(df_bpx+df_bpy > df_epx+df_epy){
			*bf_x = enemy[i]->GetPosX() + enemy[i]->GetWidth()/2;
			*bf_y = enemy[i]->GetPosY() + enemy[i]->GetHeight()/2;		
		}
	}
}
