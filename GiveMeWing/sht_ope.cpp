#include <DxLib.h>
#include <math.h>
#include "main.h"
#include "sht_ope.h"
#include "mychar.h"
#include "efect.h"
#include "database.h"
#include "sound.h"

int ShotOpe_FlatMove(Shot* shot, ShotMgr* sht_mgr, MyCharacter* mychar)
{	//�w�肳�ꂽ�����Ɗp�x�ŒP���ړ�
	double velo = shot->GetVelocity();
	double angle = shot->GetAngle();

	shot->MoveX(velo*cos(angle));
	shot->MoveY(velo*sin(angle));

	return 0;
}

#define SHOT_BRAKE 0.25
int ShotOpe_Slow(Shot* shot, ShotMgr* sht_mgr, MyCharacter* mychar)
{	//���x2.0�܂Ō������Ȃ���ړ�
	double velo = shot->GetVelocity();
	double angle = shot->GetAngle();

	if(velo > 2.0) shot->SetVelocity(velo-SHOT_BRAKE);

	shot->MoveX(velo*cos(angle));
	shot->MoveY(velo*sin(angle));

	return 0;
}

#define SHOT_ACCEL 0.1
int ShotOpe_Accel(Shot* shot, ShotMgr* sht_mgr, MyCharacter* mychar)
{	//�������Ĉړ�
	double velo = shot->GetVelocity();
	double angle = shot->GetAngle();

	shot->SetVelocity(velo+SHOT_ACCEL);

	shot->MoveX(velo*cos(angle));
	shot->MoveY(velo*sin(angle));

	return 0;
}

int ShotOpe_TowardMyChar(Shot* shot, ShotMgr* sht_mgr, MyCharacter* mychar)
{	//���L�����Ɍ����Ĉړ�
	shot->SetColor(SHOTCOLOR_YELLOW);
	double velo = shot->GetVelocity();

	double y = mychar->GetPosY() - (shot->GetPosY()+shot->GetHeight());	//�}�C�L�����ƃG�l�~�[��y���������߂�
	double x = mychar->GetPosX() - (shot->GetPosX()+shot->GetWidth());	//x���������߂�

	double prv_ang = shot->GetAngle();
	double angle = atan2(y,x);	//tan-1(x/y)�@�����Ƃ߂�

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
{	//���L�����Ɍ����ĉ������Ĉړ�
	shot->SetColor(SHOTCOLOR_YELLOW);
	double velo = shot->GetVelocity();

	double y = mychar->GetPosY() - (shot->GetPosY()+shot->GetHeight());	//�}�C�L�����ƃG�l�~�[��y���������߂�
	double x = mychar->GetPosX() - (shot->GetPosX()+shot->GetWidth());	//x���������߂�

	double prv_ang = shot->GetAngle();
	double angle = atan2(y,x);	//tan-1(x/y)�@�����Ƃ߂�

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
{	//���L�����Ɍ����ĉ������Ĉړ��i�����J�E���g�ŕ␳���s��Ȃ�
	shot->SetColor(SHOTCOLOR_YELLOW);
	double velo = shot->GetVelocity();

	double y = mychar->GetPosY() - (shot->GetPosY()+shot->GetHeight());	//�}�C�L�����ƃG�l�~�[��y���������߂�
	double x = mychar->GetPosX() - (shot->GetPosX()+shot->GetWidth());	//x���������߂�

	double prv_ang = shot->GetAngle();
	double angle = atan2(y,x);	//tan-1(x/y)�@�����Ƃ߂�

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

	double y = mychar->GetPosY() - (shot->GetPosY()+shot->GetHeight());	//�}�C�L�����ƃG�l�~�[��y���������߂�
	double x = mychar->GetPosX() - (shot->GetPosX()+shot->GetWidth());	//x���������߂�

	double prv_ang = shot->GetAngle();
	double angle = atan2(y,x);	//tan-1(x/y)�@�����Ƃ߂�

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
{	//1�J�E���g��ɏ���
	if(shot->GetCount() > 0) return 1;
	return 0;
}

int ShotOpe_Bomb(Shot* shot, ShotMgr* sht_mgr, MyCharacter* mychar)
{	//40�J�E���g��ɏ���
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

int (*Shot_Operate[SHTOPE_NUM])(Shot* shot, ShotMgr* sht_mgr, MyCharacter* mychar) =	//�֐��|�C���^�z��̒�`
{
	ShotOpe_FlatMove,	//�w�肳�ꂽ�����Ɗp�x�ŒP���ړ�
	ShotOpe_Slow,		//���x2.0�܂Ō������Ȃ���ړ�
	ShotOpe_Accel,		//�������Ĉړ�
	ShotOpe_TowardMyChar,//���L�����Ɍ����Ĉړ�
	ShotOpe_TowardMyCharAccel,//���L�����Ɍ����ĉ������Ȃ���ړ�
	ShotOpe_TowardMyCharAccelNoAdj,	//���L�����Ɍ����ĉ������Ȃ���ړ��i�����J�E���g�ŕ␳���s��Ȃ�
	ShotOpe_TowardMyCharNoAdj,	//���L�����Ɍ����Ĉړ��i�����J�E���g�ŕ␳���s��Ȃ�
	ShotOpe_Flash, //1�J�E���g��ɏ���
	ShotOpe_Bomb,
	ShotOpe_ChargeShot,
	ShotOpe_Laser,
	ShotOpe_RollSpiral,
	ShotOpe_SplitShot,
	ShotOpe_MathShot,
	ShotOpe_NoMove,
	ShotOpe_St2Boss
};