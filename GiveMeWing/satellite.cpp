#include <DxLib.h>
#include <math.h>
#include "main.h" 
#include "mychar.h"
#include "satellite.h"
#include "shot.h"
#include "sht_ope.h"
#include "database.h"
#include "input.h"
#include "efect.h"
#include "image.h"
#include "enemy.h"

double SinFract(double point, double count, double cycle, double fract);

#define SATEATK_ADJY 40
#define SATEATK_ADJX 40
void SateMove_Attack(MyCharacter* mychar, Satellite* sate)
{
	//アタックサテライトの移動処理
	if(mychar->GetSateNum() == 2){
		sate->SetPosY(mychar->GetPosY()+mychar->GetHeight()/2+SATEATK_ADJY/2);

		if(sate->GetNum() == 0) sate->SetPosX(mychar->GetPosX()+mychar->GetWidth()/2 - SATEATK_ADJX);
		else sate->SetPosX(mychar->GetPosX()+mychar->GetWidth()/2 + SATEATK_ADJX);

		return;
	}

	sate->SetPosY(mychar->GetPosY()+mychar->GetHeight()/2+SATEATK_ADJY);
	sate->SetPosX(mychar->GetPosX()+mychar->GetWidth()/2);
}

#define SATEGRD_COUNT 60	//一周期のカウント
#define SATEGRD_R 60	//半径
void SateMove_Guard(MyCharacter* mychar, Satellite* sate)
{	//ガードサテライトの移動処理
	double rad = 2*PI * ((double)sate->GetCount()/SATEGRD_COUNT);
	if(sate->GetNum() == 1) rad = rad+PI;

	sate->SetPosX(mychar->GetPosX()+mychar->GetWidth()/2+SATEGRD_R*cos(rad));
	sate->SetPosY(mychar->GetPosY()+mychar->GetHeight()/2+SATEGRD_R*sin(rad));
}

void (*SateMove[SATE_NUM])(MyCharacter* mychar, Satellite* sate) =
{
	SateMove_Attack,
	SateMove_Guard,
};



int SateShot_Attack(double px, double py, ShotMgr* sht_mgr, EnemyMgr* ene_mgr)
{	//アタックサテライトのショット処理
	double bf_x = px;
	double bf_y = py;

	ene_mgr->QueryNearEnemyPos(px, py, &bf_x, &bf_y);
	double ang = atan2(bf_y-py,bf_x-px);
	if(ang == 0.0) ang = -PI/2;

	sht_mgr->SetShot(px, py, 10, ang, SHOT_SATEATKSHOT, SHOTOPE_FLATMOVE, SHOTSIDE_MYCHAR, 0);
	return 8;
}

int SateShot_Guard(double px, double py, ShotMgr* sht_mgr, EnemyMgr* ene_mgr)
{	//ガードサテライトのショット処理
	sht_mgr->SetShot(px, py+15, 10, -PI/2, SHOT_SATEBAR, SHOTOPE_FLASH, SHOTSIDE_MYCHAR, 0);
	return 1;
}

int (*SateShot[SATE_NUM])(double px, double py, ShotMgr* sht_mgr, EnemyMgr* ene_mgr) =
{
	SateShot_Attack,
	SateShot_Guard,
};



int SateSmash_Attack(MyCharacter* mychar, Satellite* sate, ShotMgr* sht_mgr)
{
	double px = sate->GetPosX();
	double py = sate->GetPosY();

	sht_mgr->SetShot(px-10, py, 18, -PI/2, SHOT_SATEATKSMASH, SHOTOPE_LASER, SHOTSIDE_MYCHAR, 0, FALSE);

	return 1;
}

int SateSmash_Guard(MyCharacter* mychar, Satellite* sate, ShotMgr* sht_mgr)
{
	double px = sate->GetPosX();
	double py = sate->GetPosY();

	if(sate->GetCount()%30 == 0) efect.SetEfect(mychar->GetPosX()+GAME_X,mychar->GetPosY()+GAME_Y,EFECT_SMALLBURN,60,GetColor(0,0,255));
	sht_mgr->SetShot(mychar->GetPosX()-59, mychar->GetPosY()-59, 0, -PI/2, SHOT_SATEGRDSMASH, SHOTOPE_FLASH, SHOTSIDE_MYCHAR, 0, FALSE);

	return 1;
}

int (*SateSmash[SATE_NUM])(MyCharacter* mychar, Satellite* sate, ShotMgr* sht_mgr) =
{
	SateSmash_Attack,
	SateSmash_Guard,
};

/*
クラス名 : Satellite
	マイキャラをサポートする衛星

	継承 : Character

	記述ファイル : satellite.h
*/
#define SATE_PUSH 30
Satellite::Satellite(double a_x, double a_y, int a_width, int a_height, int img_han, int a_num, int a_type):Character(a_x, a_y, a_width, a_height, 1, img_han)
{
	life = 338;
	count=0;
	shot_interval=0;
	type=a_type;
	width=SATE_SIZE;
	height=SATE_SIZE;
	smash=FALSE;
	num = a_num;

	move = FALSE;
	move_count = 0;

	img_index=IMG_SATELLITE;
	image.GetImageSize(img_index, &img_w, &img_h);
	img_w/=img_db.QueryImgData(img_index)->div_num;
}

int Satellite::Update(MyCharacter* mychar, ShotMgr* sht_mgr, EnemyMgr* ene_mgr)
{
	SateMove[type](mychar, this);	//移動処理

	if(input.GetInput(INPUT_SATECHANGE) == 1){
		type = (type+1)%2;	
	}

	//スマッシュ処理系
	if(JudgeEnableSmash(mychar) && sht_mgr != NULL){
		move = TRUE;
		shot_interval = SateSmash[type](mychar, this, sht_mgr);	//スマッシュの予約
		goto MAINEND;	//スマッシュを撃ったら、それ以外の動作はスキップする
	} else move = FALSE;
	//
	
	if(shot_interval == 0 && input.GetInput( INPUT_SHOT ) > 0 && sht_mgr != NULL) {
		//ショットインターバル状態でなく、ショットボタンが押されていたら
		shot_interval = SateShot[type](px, py, sht_mgr, ene_mgr);	//ショット処理
	}

MAINEND:

	count++;	//カウントを進める
	if(shot_interval > 0) shot_interval--;

	return 0;
}

void Satellite::Draw()
{
	//当たり矩形が、画像の中心に来るように補正
	float draw_x = (float)px - img_w/2;
	float draw_y = (float)py - img_h/2;
	//

	image.DrawImageF(draw_x, draw_y, img_index, TRUE, type);
		//描画

#ifdef DEBUG_CODE
	_DrawBox(int(px), int(py), int(px)+width, int(py)+height, GetColor(0,0,255),FALSE);
	//あたり矩形範囲の表示
#endif
}

double Satellite::QuerySmashAccel()
{
	switch(type){
	case SATE_ATK:
		return 1.0;
	case SATE_GRD:
		return 0.0;
	}
	return 4.0;
}

bool Satellite::JudgeEnableSmash(MyCharacter* mychar)
{
	if(mychar->GetShotLv() < 2) {smash = FALSE; return FALSE;}
	if(input.GetInput( INPUT_SATESMASH ) == 0) smash = FALSE;
		//スマッシュキーが入力されていなければ、許可しない

	if(input.GetInput( INPUT_SATESMASH ) == 1){	
		//スマッシュキー入力の初回ならば、

		if(!mychar->JudgeMaxCharge()) smash = FALSE;
			//チャージが満タンかどうか調べ、満タンでなければ許可しない
		else {smash = TRUE; mychar->AddCharge(-29);}
			//満タンなら、スマッシュを許可し、初回だけ多めにチャージを減らす
	}

	if(input.GetInput( INPUT_SATESMASH ) > 1 && smash == TRUE){
		//スマッシュキーが入力され続けていて、スマッシュが許可されて　い　た　なら

		if(mychar->GetChargePer() == 0) smash = FALSE;
			//現在のチャージ％を調べ、０なら許可しない
		else {smash = TRUE; mychar->AddCharge(-1);}
			//でなければ、スマッシュを許可し、チャージを減らす
	}

	return smash;
}