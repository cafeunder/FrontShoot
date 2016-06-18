#include <DxLib.h>
#include <math.h>
#include "main.h"
#include "sht_ope.h"
#include "mychar.h"
#include "efect.h"
#include "database.h"
#include "sound.h"

int ShotOpe_FlatMove(Shot* shot, ShotMgr* sht_mgr, MyCharacter* mychar)
{	//指定された速さと角度で単調移動
	double velo = shot->GetVelocity();
	double angle = shot->GetAngle();

	shot->MoveX(velo*cos(angle));
	shot->MoveY(velo*sin(angle));

	return 0;
}

#define SHOT_BRAKE 0.25
int ShotOpe_Slow(Shot* shot, ShotMgr* sht_mgr, MyCharacter* mychar)
{	//速度2.0まで減速しながら移動
	double velo = shot->GetVelocity();
	double angle = shot->GetAngle();

	if(velo > 2.0) shot->SetVelocity(velo-SHOT_BRAKE);

	shot->MoveX(velo*cos(angle));
	shot->MoveY(velo*sin(angle));

	return 0;
}

#define SHOT_ACCEL 0.1
int ShotOpe_Accel(Shot* shot, ShotMgr* sht_mgr, MyCharacter* mychar)
{	//加速して移動
	double velo = shot->GetVelocity();
	double angle = shot->GetAngle();

	shot->SetVelocity(velo+SHOT_ACCEL);

	shot->MoveX(velo*cos(angle));
	shot->MoveY(velo*sin(angle));

	return 0;
}

int ShotOpe_TowardMyChar(Shot* shot, ShotMgr* sht_mgr, MyCharacter* mychar)
{	//自キャラに向けて移動
	shot->SetColor(SHOTCOLOR_YELLOW);
	double velo = shot->GetVelocity();

	double y = mychar->GetPosY() - (shot->GetPosY()+shot->GetHeight());	//マイキャラとエネミーのy距離を求める
	double x = mychar->GetPosX() - (shot->GetPosX()+shot->GetWidth());	//x距離を求める

	double prv_ang = shot->GetAngle();
	double angle = atan2(y,x);	//tan-1(x/y)　をもとめる

	if(fabs(angle-prv_ang) > PI/360 && shot->GetCount() != 0) {
		if(angle-prv_ang > 0) angle = prv_ang+PI/360;
		else angle = prv_ang-PI/360;
	}

	shot->SetAngle(angle);

	shot->MoveX(velo*cos(angle));
	shot->MoveY(velo*sin(angle));

	return 0;
}

int ShotOpe_TowardMyCharAccel(Shot* shot, ShotMgr* sht_mgr, MyCharacter* mychar)
{	//自キャラに向けて加速して移動
	shot->SetColor(SHOTCOLOR_YELLOW);
	double velo = shot->GetVelocity();

	double y = mychar->GetPosY() - (shot->GetPosY()+shot->GetHeight());	//マイキャラとエネミーのy距離を求める
	double x = mychar->GetPosX() - (shot->GetPosX()+shot->GetWidth());	//x距離を求める

	double prv_ang = shot->GetAngle();
	double angle = atan2(y,x);	//tan-1(x/y)　をもとめる

	if(fabs(angle-prv_ang) > PI/360 && shot->GetCount() != 0){
		if(angle-prv_ang > 0) angle = prv_ang+PI/360;
		else angle = prv_ang-PI/360;
	}

	shot->SetAngle(angle);
	shot->SetVelocity(velo+sin(PI/360*shot->GetCount()));

	shot->MoveX(velo*cos(angle));
	shot->MoveY(velo*sin(angle));

	return 0;
}

int ShotOpe_TowardMyCharAccelNoAdj(Shot* shot, ShotMgr* sht_mgr, MyCharacter* mychar)
{	//自キャラに向けて加速して移動（初期カウントで補正を行わない
	shot->SetColor(SHOTCOLOR_YELLOW);
	double velo = shot->GetVelocity();

	double y = mychar->GetPosY() - (shot->GetPosY()+shot->GetHeight());	//マイキャラとエネミーのy距離を求める
	double x = mychar->GetPosX() - (shot->GetPosX()+shot->GetWidth());	//x距離を求める

	double prv_ang = shot->GetAngle();
	double angle = atan2(y,x);	//tan-1(x/y)　をもとめる

	if(fabs(angle-prv_ang) > PI/360) {
		if(angle-prv_ang > 0) angle = prv_ang+PI/360;
		else angle = prv_ang-PI/360;
	}

	shot->SetAngle(angle);
	shot->SetVelocity(velo+sin(PI/360*shot->GetCount()));

	shot->MoveX(velo*cos(angle));
	shot->MoveY(velo*sin(angle));

	return 0;
}

int ShotOpe_TowardMyCharNoAdj(Shot* shot, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	shot->SetColor(SHOTCOLOR_YELLOW);
	double velo = shot->GetVelocity();

	double y = mychar->GetPosY() - (shot->GetPosY()+shot->GetHeight());	//マイキャラとエネミーのy距離を求める
	double x = mychar->GetPosX() - (shot->GetPosX()+shot->GetWidth());	//x距離を求める

	double prv_ang = shot->GetAngle();
	double angle = atan2(y,x);	//tan-1(x/y)　をもとめる

	if(fabs(angle-prv_ang) > PI/360) {
		if(angle-prv_ang > 0) angle = prv_ang+PI/360;
		else angle = prv_ang-PI/360;
	}

	shot->SetAngle(angle);

	shot->MoveX(velo*cos(angle));
	shot->MoveY(velo*sin(angle));

	return 0;
}

int ShotOpe_Flash(Shot* shot, ShotMgr* sht_mgr, MyCharacter* mychar)
{	//1カウント後に消滅
	if(shot->GetCount() > 0) return 1;
	return 0;
}

int ShotOpe_Bomb(Shot* shot, ShotMgr* sht_mgr, MyCharacter* mychar)
{	//40カウント後に消滅
	if(shot->GetCount() > 60) return 1;
	return 0;
}

int ShotOpe_ChargeShot(Shot* shot, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	double velo = shot->GetVelocity();
	double angle = shot->GetAngle();

	shot->MoveX(velo*cos(angle));
	shot->MoveY(velo*sin(angle));

	if(shot->GetHit()){
		double px = shot->GetPosX()+shot->GetWidth()/2;
		double py = shot->GetPosY()+shot->GetHeight()/2;

		//int shot_type = SHOT_CSHOTBURN1 + mychar->GetShotLv();

		efect.SetEfect(px+GAME_X, py+GAME_Y, EFECT_SMALLBURN, 100, GetColor(0,255,255));
		sound.PlaySoundHandle(SOUNDEFECT_CHARGEBURN);
		sht_mgr->SetShot(px-65, py-65, 0, 0, SHOT_CSHOTBURN1, SHOTOPE_FLASH, shot->GetSide(), 0,FALSE);
		return 1;
	}

	return 0;
}

int ShotOpe_Laser(Shot* shot, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	double velo = shot->GetVelocity();
	double angle = shot->GetAngle();

	shot->SetPosX(mychar->GetPosX()-shot->GetWidth()/2);
	shot->MoveY(velo*sin(angle));

	return 0;
}

int ShotOpe_RollSpiral(Shot* shot, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	double velo = shot->GetVelocity();
	double angle = shot->GetAngle();

	shot->MoveX(velo*cos(angle));
	shot->MoveY(velo*sin(angle));

	shot->SetAngle(shot->GetAngle()+PI/180*4);
	shot->SetVelocity(shot->GetVelocity()+0.3);

	return 0;
}

int ShotOpe_SplitShot(Shot* shot, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	double velo = shot->GetVelocity();
	double angle = shot->GetAngle();

	shot->MoveX(velo*cos(angle));
	shot->MoveY(velo*sin(angle));

	if(shot->GetCount() == 100){
		int px = (int)shot->GetPosX() + shot->GetWidth()/2;
		int py = (int)shot->GetPosY() + shot->GetHeight()/2;

		for(int i = 0; i < 12; i++){
			sht_mgr->SetShot(px, py, 1, (((30.0)*i-180.0)/180.0)*PI+PI/2, SHOT_ENEEN, SHOTOPE_ACCEL, shot->GetSide(), SHOTCOLOR_MAGENTA);	
		}
		return 1;
	}
	return 0;
}

int ShotOpe_MathShot(Shot* shot, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	double velo = shot->GetVelocity();
	double angle = shot->GetAngle();

	shot->MoveX(velo*cos(angle));
	shot->MoveY(velo*sin(angle));

	if(shot->GetCount() == 75){
		int px = (int)shot->GetPosX() + shot->GetWidth()/2;
		int py = (int)shot->GetPosY() + shot->GetHeight()/2;

		for(int i = 0; i < 2; i++){
			sht_mgr->SetShot(px, py, shot->GetVelocity(), PI/4 + (PI/2)*i, SHOT_ENESP, SHOTOPE_MATHSHOT, shot->GetSide(), SHOTCOLOR_MAGENTA);
		}
		return 1;
	}
	return 0;
}

int ShotOpe_St2Boss(Shot* shot, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	double velo = shot->GetVelocity();
	double angle = shot->GetAngle();

	shot->MoveX(velo*cos(angle));
	shot->MoveY(velo*sin(angle));

	if(shot->GetCount() >= 200) return 1;
	return 0;
}

int ShotOpe_NoMove(Shot* shot, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	return 0;
}

int (*Shot_Operate[SHTOPE_NUM])(Shot* shot, ShotMgr* sht_mgr, MyCharacter* mychar) =	//関数ポインタ配列の定義
{
	ShotOpe_FlatMove,	//指定された速さと角度で単調移動
	ShotOpe_Slow,		//速度2.0まで減速しながら移動
	ShotOpe_Accel,		//加速して移動
	ShotOpe_TowardMyChar,//自キャラに向けて移動
	ShotOpe_TowardMyCharAccel,//自キャラに向けて加速しながら移動
	ShotOpe_TowardMyCharAccelNoAdj,	//自キャラに向けて加速しながら移動（初期カウントで補正を行わない
	ShotOpe_TowardMyCharNoAdj,	//自キャラに向けて移動（初期カウントで補正を行わない
	ShotOpe_Flash, //1カウント後に消滅
	ShotOpe_Bomb,
	ShotOpe_ChargeShot,
	ShotOpe_Laser,
	ShotOpe_RollSpiral,
	ShotOpe_SplitShot,
	ShotOpe_MathShot,
	ShotOpe_NoMove,
	ShotOpe_St2Boss
};