#include "DxLib.h"
#include "main.h"
#include "fps.h"
#include "database.h"
#include "gameloop.h"
#include "input.h"
#include "efect.h"

int CallKosen();

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	SetMainWindowText("Give Me Wing");
	SetOutApplicationLogValidFlag(FALSE);
	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen( DX_SCREEN_BACK ); //ウィンドウモード変更と初期化と裏画面設定   


	font_db.LoadFontData();	//画像ファイルのロード
	img_db.LoadImgData();
	sound_db.LoadSoundData();

	Fps fps;
	GameLoop loop;
	bool game_start = FALSE;

	while( ScreenFlip()==0 && ProcessMessage()==0 && ClearDrawScreen()==0 ){
		input.Update();	//キーボード情報の更新
		fps.Update();

		//　↓　処理本体をここに書く　↓

		if(!game_start){
			//if(CallKosen() == 1) game_start = TRUE;
			game_start = TRUE;
		}
		
		if(game_start){
			if(loop.Loop() == 1) break;
		}

		SetDrawArea(GAME_X,GAME_Y,GAME_X+GAME_WIDTH,GAME_Y+GAME_HEIGHT);//描画エリアをゲーム範囲に設定
		efect.Update();
		SetDrawArea(0,0,WINDOWSIZE_X,WINDOWSIZE_Y);	//描画エリアを元に戻す

		//　↑　処理本体をここに書く　↑

		fps.Draw();
		fps.Wait();			//fps制御
	}
	
	DxLib_End();	//終了処理
	return 0;
}

int CallKosen()
{
	static int count = 0;
	static int handle = LoadGraph("DATA/img/kosen.png");
	static int b_handle = LoadGraph("DATA/img/kosenback.png");
	static int s_handle = LoadSoundMem("DATA/soundefect/bell02.ogg");

	if(count == 30) PlaySoundMem(s_handle, DX_PLAYTYPE_BACK);
	if(count < 30) SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(((double)count/30)*255));
	if(count > 230) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255-(int)(((double)(count-230)/30)*255));

	DrawGraph(0, 0, b_handle, TRUE);
	DrawGraph(0, 200, handle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	count++;
	if(count == 380) return 1;
	return 0;
}


void _DrawBox(int x1, int y1, int x2, int y2, int Color, bool FillFlag)
{
	DrawBox(x1+GAME_X, y1+GAME_Y, x2+GAME_X, y2+GAME_Y, Color, FillFlag);
}