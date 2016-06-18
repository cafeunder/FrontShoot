#include <DxLib.h>
#include <math.h>
#include "main.h"
#include "mychar.h"
#include "database.h"
#include "enemy.h"
#include "shot.h"
#include "background.h"
#include "item.h"
#include "image.h"
#include "stage.h"
#include "operate.h"
#include "boss.h"
#include "sound.h"

Stage::Stage(int stg_index, OperateArea* ope_area, int a_level)
{
	stage_num = stg_index;
	game_level = a_level;
	count = 0;	//カウンタの初期化
	boss_result = 0;
	sound_volume = 255;

	StageData_t* stg_data = stg_db.QueryStageData(stg_index);	//ステージデータの読み込み
	boss_index = stg_data->boss_index;	//ボスデータを記憶
	boss_count = stg_data->boss_count;	//ボス出現カウントを記憶
	sound_index = stg_data->sound_index;
	sfade_count = stg_data->sfade_count;


	change_sound = stg_data->change_sound;
	schange_count = stg_data->schange_count;
	scfade_count = schange_count - 255;

	sht_mgr = new ShotMgr();	//ショットマネージャのインスタンス作成
	item_mgr = new ItemMgr();	//アイテムマネージャのインスタンス作成

	//エネミー初期化
	const char* file_p;

	switch(a_level){
	case 0: file_p = stg_data->efile_name; break;
	case 1: file_p = stg_data->sfile_name; break;
	case 2: file_p = stg_data->hfile_name; break;
	}

	int file_han = FileRead_open(file_p);	//ファイル読み込み

	int ene_num; EnemyInitData_t* init_data;	//必要初期化変数

	FileRead_read(&ene_num, sizeof(int), file_han);	//敵の数を読み込み

			//敵の数だけメモリを確保
	init_data = (EnemyInitData_t*)malloc(sizeof(EnemyInitData_t)*ene_num);
	FileRead_read(init_data, sizeof(EnemyInitData_t)*ene_num, file_han);
			//敵の初期化データを読み込み

	assert(FileRead_eof(file_han) != 0);	//ファイルの終端まで読み込んでいなければならない

	ene_mgr = new EnemyMgr(ene_num, init_data);	
			//初期化データでエネミーマネージャのインスタンス作成

	FileRead_close(file_han);	//ファイルクローズ
	free(init_data);			//メモリの開放
	//

	back_g = new BackGround(stg_data->fgimg_index, stg_data->bgimg_index, stg_data->f_velocity, stg_data->b_velocity);	//背景のインスタンス作成

	mes_data = stg_data->mes_data;	//メッセージデータポイントを記憶

	boss = NULL;

#ifndef DEBUG_CODE
	ope_area->SetMessage(mes_data[0].message, mes_data[0].operater_type, mes_data[0].brake_count);
#endif
}

Stage::~Stage()
{
	//各インスタンスの破棄
	delete sht_mgr;
	delete item_mgr;
	delete ene_mgr;
	delete back_g;
	delete boss;
	//
}

int Stage::Update(MyCharacter* mychar, OperateArea* ope_area)
{
	if(count == 0) 	sound.PlaySoundHandle(sound_index);

	int chr_result = 0;	//返す値

	chr_result = mychar->Update(ene_mgr,sht_mgr, boss);//マイキャラの更新（ゲームオーバーなら1が返る
	sht_mgr->Update(mychar);//ショットの更新
	item_mgr->Update(mychar);//アイテムの更新

	if(boss != NULL) 
		if(boss->Update(sht_mgr, mychar,game_level) == 1){
			boss_result = 1;

			delete boss;
			boss = NULL;

			int i = 0;
			while(mes_data[i].count != -3){
				if(mes_data[i].count == -4){
					ope_area->SetMessage(mes_data[i].message, mes_data[i].operater_type, mes_data[i].brake_count);
				}
				i++;
			}

			sound.PlaySoundHandle(SOUNDEFECT_STARTEFECT);
			return 0;
		}

	ene_mgr->Update(sht_mgr,mychar,item_mgr,count,game_level);//エネミーの更新



	//メッセージ処理
	int i = 0;
	while(i < STAGEMES_MAXNUM){	//ステージメッセージ予約チェック
		if(mes_data[i].count == -3) break;	//現在メッセージデータが番兵ならチェック終了
		if(mes_data[i].count > count) break;
			//現在メッセージデータのカウントが現在カウントよりも大きければ、
			//もうその後は探すだけ無駄なのでチェック終了

		if(count == mes_data[i].count){ //もし、現在カウントがメッセージ予約カウントなら
			ope_area->SetMessage(mes_data[i].message, mes_data[i].operater_type, mes_data[i].brake_count);
			//メッセージを予約
		}

		i++;
	}
	//



	if(!ope_area->GetDrawMes()) count++;	//オペレーターエリア表示中でなければカウントを進める

	if(CheckHitKey( KEY_INPUT_D ) == 1){
		count = KeyInputNumber(0,0,boss_count,0,TRUE);
		ope_area->Reset();
	}

	if(CheckHitKey( KEY_INPUT_F ) == 1){
		sound.StopSoundHandle(sound_index);
		return 2;
	}

	//ボスカウントに達していれば、
	//tag
	if(count == boss_count) {

		int i = 0;
		while(mes_data[i].count != -3){
			if(mes_data[i].count == -2){
				ope_area->SetMessage(mes_data[i].message, mes_data[i].operater_type, mes_data[i].brake_count);
			}
			i++;
		}

		if(boss_index == -1) {sound.StopSoundHandle(sound_index);return 2;}

		boss = new Boss(boss_index);


	}	//クリアを知らせるため2を返す（仮実装
	//

	if(sfade_count != -1 && count >= sfade_count){
		sound.ChangeVolumeSound(255-(count-sfade_count),sound_index);
	}

	if(change_sound != -1 && count >= scfade_count && (sound_index != change_sound)){
		sound.ChangeVolumeSound(250-(count-scfade_count),sound_index);
		if(255-(count-scfade_count) == 0){
			sound.StopSoundHandle(sound_index);

			sound_index = change_sound;
			sound.PlaySoundHandle(sound_index);
		}
	}
	if(chr_result == 1) {
		StopStageSound();
		count = 0; 
		return 1;
	} //マイキャラ更新の戻り値が1ならゲームオーバー

	if(boss_result == 1) {
		//sound.ChangeVolumeSound(63,sound_index);
		if(sound_volume > 0) sound_volume--;
		sound.ChangeVolumeSound(sound_volume,sound_index);

		if(!ope_area->GetDrawMes()){
			sound.StopSoundHandle(SOUNDEFECT_STARTEFECT);
			StopStageSound();
			return 2;
		}
	}

	return 0;
}

void Stage::StopStageSound()
{
	sound.StopSoundHandle(sound_index);
}

void Stage::Draw(MyCharacter* mychar, int d_count)
{
	SetDrawArea(GAME_X,GAME_Y,GAME_X+GAME_WIDTH,GAME_Y+GAME_HEIGHT);//描画エリアをゲーム範囲に設定

	back_g->BackDraw(d_count);	//後背景の描画
	ene_mgr->OnTheDraw(count,mychar);	//接地型エネミーの描画
	if(boss != NULL) boss->Draw();	//ボスの描画
	back_g->FrontDraw(d_count);	//前背景の描画
	item_mgr->Draw();	//アイテムの描画
	mychar->Draw(d_count);	//マイキャラの描画
	ene_mgr->Draw(count,mychar);	//通常エネミーの描画
	sht_mgr->Draw();	//ショットの描画

	SetDrawArea(0,0,WINDOWSIZE_X,WINDOWSIZE_Y);	//描画エリアを元に戻す

#ifdef DEBUG_CODE
	DrawFormatString(140,0,GetColor(255,255,255),"カウント:%d",count);
#endif
}

void Stage::DrawBackGround(int count){back_g->BackDraw(count);}