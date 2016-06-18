#include <DxLib.h>
#include <math.h>
#include "enemy.h"
#include "main.h"
#include "database.h"
#include "mychar.h"
#include "ene_ope.h"
#include "shot.h"
#include "sht_ope.h"
#include "efect.h"
#include "sound.h"

int EnemyMove_FlatY(Enemy* ene, MyCharacter* mychar)	//�㉺�ɒP���ړ�
{
	ene->MoveY(ene->GetVelocity());

	return 1;
}

int EnemyMove_FlatMove(Enemy* ene, MyCharacter* mychar)
{	//�����Ă�����ɒP���ړ�
	double velo = ene->GetVelocity();
	double ang = ene->GetAngle();

	ene->MoveX(velo*cos(ang));
	ene->MoveY(velo*sin(ang));

	return 1;
}

int EnemyMove_MoveWaitY(Enemy* ene, MyCharacter* mychar)
{	//��U��~�B���̌��ʃA�E�g
	int result = 0;	//�Ԃ��l

	double velo = ene->GetVelocity();
	double ang = ene->GetAngle();

	if(ene->GetCount() > 60 && ene->GetCount() <= 60+ene->GetBrakeCount()){
		//�ݒ肳�ꂽ�ҋ@���ԕ���~
		result = 1;
		//��~�������e�����Ă�
	} else {
		ene->MoveX(velo*cos(ang));
		ene->MoveY(velo*sin(ang));	
	}

	return result;
}

int EnemyMove_MoveWaitYNoAng(Enemy* ene, MyCharacter* mychar)
{
	int result = 0;	//�Ԃ��l

	double velo = ene->GetVelocity();

	if(ene->GetCount() > 60 && ene->GetCount() <= 60+ene->GetBrakeCount()){
		//�ݒ肳�ꂽ�ҋ@���ԕ���~
		result = 1;
		//��~�������e�����Ă�
	} else {
		ene->MoveY(velo);	
	}

	return result;
}

int EnemyMove_MoveWaitYAccel(Enemy* ene, MyCharacter* mychar)
{	//��U��~�B�������ĉ�ʃA�E�g
	int result = 0;	//�Ԃ��l

	double velo = ene->GetVelocity();
	double ang = ene->GetAngle();

	if(ene->GetCount() < 60) {
		ene->MoveX(velo*cos(ang));
		ene->MoveY(velo*sin(ang));	
	}	//��P�b�Ԏw��ǂ���ɐi��

	else if(ene->GetCount() <= 60+ene->GetBrakeCount()){
		//�ݒ肳�ꂽ�ҋ@���ԕ���~
		result = 1;
		//��~�������e�����Ă�
	}
	else{
		ene->MoveX(velo*2*cos(ang));
		ene->MoveY(velo*2*sin(ang));
		//�ʏ�̂Q�{�̑����ŉ�ʃA�E�g
	}

	return result;
}

int EnemyMove_MoveWaitYAccelNoAng(Enemy* ene, MyCharacter* mychar)
{	//��U��~�B�������ĉ�ʃA�E�g
	int result = 0;	//�Ԃ��l

	double velo = ene->GetVelocity();

	if(ene->GetCount() < 60) {
		ene->MoveY(velo);	
	}	//��P�b�Ԏw��ǂ���ɐi��

	else if(ene->GetCount() <= 60+ene->GetBrakeCount()){
		//�ݒ肳�ꂽ�ҋ@���ԕ���~
		result = 1;
		//��~�������e�����Ă�
	}
	else{
		ene->MoveY(velo*2);
		//�ʏ�̂Q�{�̑����ŉ�ʃA�E�g
	}

	return result;
}

int EnemyMove_TowardCenter(Enemy* ene, MyCharacter* mychar)
{	//���΂炭���i������A�����Ɍ������Ĉړ�
	double velo = ene->GetVelocity();
	double ang = ene->GetAngle();

	ene->MoveX(velo*cos(ang));
	ene->MoveY(velo*sin(ang));

	if(ene->GetCount() == ene->GetBrakeCount()){
		if(ene->GetPosX()+ene->GetWidth()/2 > GAME_WIDTH/2) {
						//��ʂ̒������E���ɂ���΍�����
			if(ang <= PI) ene->SetAngle((PI/4)*3);
			else ene->SetAngle((PI/4)*5);
		}
		if(ene->GetPosX()+ene->GetWidth()/2 < GAME_WIDTH/2) {
						//��ʂ̒�����荶���ɂ���ΉE���ֈړ�
			if(ang <= PI) ene->SetAngle(PI/4);
			else ene->SetAngle(-PI/4);
		}
	}

	return 1;
}

int EnemyMove_TowardEdge(Enemy* ene, MyCharacter* mychar)
{	//���΂炭���i������A��ʒ[�Ɍ������Ĉړ�
	double velo = ene->GetVelocity();
	double ang = ene->GetAngle();

	ene->MoveX(velo*cos(ang));
	ene->MoveY(velo*sin(ang));

	if(ene->GetCount() == ene->GetBrakeCount()){
		if(ene->GetPosX()+ene->GetWidth()/2 > GAME_WIDTH/2) {
						//��ʂ̒������E���ɂ���΍�����
			if(ang <= PI) ene->SetAngle(PI/4);
			else ene->SetAngle(-PI/4);
		}
		if(ene->GetPosX()+ene->GetWidth()/2 < GAME_WIDTH/2) {
						//��ʂ̒�����荶���ɂ���ΉE���ֈړ�
			if(ang <= PI) ene->SetAngle((PI/4)*3);
			else ene->SetAngle((PI/4)*5);
		}
	}

	return 1;
}

int EnemyMove_AccelTurn(Enemy* ene, MyCharacter* mychar)
{	//��ʏ㉺���Α��ֈړ���A���񂵂ēːi
	int count = ene->GetCount();

	if(count < 100) {
		ene->SetAngle(-PI/2);

		ene->MoveY(GAME_HEIGHT-ene->GetPosY());
		ene->MoveY(-sin((PI/2)*((double)count/100))*480.0);
		return 1;
	} else if(count == 100) ene->SetAngle(PI/2);
	else {
		double velo = ene->GetVelocity();
		int count = ene->GetCount();

		double y = mychar->GetPosY() - (ene->GetPosY()+ene->GetHeight()/2);	//�}�C�L�����ƃG�l�~�[��y���������߂�
		double x = mychar->GetPosX() - (ene->GetPosX()+ene->GetWidth()/2);	//x���������߂�
		double prv_ang = ene->GetAngle();
		double angle = atan2(y,x);	//tan-1(x/y)�@�����Ƃ߂�

		angle = atan2(y,x);	//tan-1(x/y)�@�����Ƃ߂�
		ene->SetAngle(angle);

		ene->SetVelocity(velo+0.2);

		if(fabs(angle-prv_ang) > PI/180) {
			if(angle-prv_ang > 0) angle = prv_ang+PI/180;
			else angle = prv_ang-PI/180;
		}

		ene->SetAngle(angle);

		ene->MoveX(velo*cos(angle));
		ene->MoveY(velo*sin(angle));

		return 0;
	}

	return 0;
}

int EnemyMove_TurnRightAngle(Enemy* ene, MyCharacter* mychar)
{	//���΂炭���āA�����v���ɒ��p����
	double velo = ene->GetVelocity();
	double ang = ene->GetAngle();

	ene->MoveX(velo*cos(ang));
	ene->MoveY(velo*sin(ang));

	if(ene->GetCount() == ene->GetBrakeCount()){
		ene->SetAngle(ang-PI/2);
	}

	return 1;
}

int EnemyMove_TurnLeftAngle(Enemy* ene, MyCharacter* mychar)
{	//���΂炭���āA���v���ɒ��p����
	double velo = ene->GetVelocity();
	double ang = ene->GetAngle();

	ene->MoveX(velo*cos(ang));
	ene->MoveY(velo*sin(ang));

	if(ene->GetCount() == ene->GetBrakeCount()){
		ene->SetAngle(ang+PI/2);
	}

	return 1;
}

int EnemyMove_Turn90(Enemy* ene, MyCharacter* mychar)
{
	double velo = ene->GetVelocity();
	double ang = ene->GetAngle();

	ene->MoveX(velo*cos(ang));
	ene->MoveY(velo*sin(ang));

	if(ene->GetCount() == ene->GetBrakeCount()){
		ene->SetAngle(PI/2);
	}

	return 1;
}

int EnemyMove_MoveWaitYSlow(Enemy* ene, MyCharacter* mychar)
{
	int result = 0;	//�Ԃ��l

	double velo = ene->GetVelocity();
	double ang = ene->GetAngle();
	int count = ene->GetCount();

	if(count <= 60){
		ene->MoveY(velo - velo*(((double)count)/60));
	}
	if(count > 60 && count <= 60+ene->GetBrakeCount()){
		//�ݒ肳�ꂽ�ҋ@���ԕ���~
		result = 1;
		//��~�������e�����Ă�
	} else {
		ene->MoveX(velo*cos(ang));
		ene->MoveY(velo*sin(ang));	
	}

	return result;
}

int EnemyMove_MoveWaitYSlowNoAng(Enemy* ene, MyCharacter* mychar)
{
	int result = 0;	//�Ԃ��l

	double velo = ene->GetVelocity();
	int count = ene->GetCount();

	if(count <= 60){
		ene->MoveY(velo - velo*(((double)count)/60));
	}
	if(count > 60 && count <= 60+ene->GetBrakeCount()){
		//�ݒ肳�ꂽ�ҋ@���ԕ���~
		result = 1;
		//��~�������e�����Ă�
	} else {
		ene->MoveY(velo);	
	}

	return result;
}



#define ENESHOT_LSHOT 30
int EnemyMove_LaserShot(Enemy* ene, MyCharacter* mychar)
{
    int count = ene->GetCount()%(ene->GetShotInterval()+ENESHOT_LSHOT);

    ene->MoveY(ene->GetVelocity());

    int interval = ene->GetShotInterval();
	if(count < interval){
		ene->SetTurn(ENEDIR_MYCHAR);
		return 0;
	} else 
	if(count == interval){
		ene->SetTurn(ENEDIR_FRONT);
		efect.SetEfect(ene->GetPosX()+ene->GetWidth()/2+GAME_X,ene->GetPosY()+ene->GetHeight()/2+GAME_Y,EFECT_LASEREFECT,-1,-1,ene->GetAngle());
		return 0;
	} else {
		//���[�U�[������
        return 1;
    }
}

int EnemyMove_LaserShotWait(Enemy* ene, MyCharacter* mychar)
{
	double velo = ene->GetVelocity();

	if(ene->GetCount() < 60) {
		ene->MoveY(velo);	
	}	//��P�b�Ԏw��ǂ���ɐi��
	else if(ene->GetCount() <= 60+ene->GetBrakeCount()){
		int count = ene->GetCount()%(ene->GetShotInterval()+ENESHOT_LSHOT);

		int interval = ene->GetShotInterval();
		if(count == interval-30){
			ene->SetTurn(ENEDIR_FRONT);
			efect.SetEfect(ene->GetPosX()+ene->GetWidth()/2,ene->GetPosY()+ene->GetHeight()/2,EFECT_LASEREFECT,-1,-1,ene->GetAngle());
			return 0;
		}
		
		if(count < interval-30){
			ene->SetTurn(ENEDIR_MYCHAR);
			return 0;
		} else if(count < interval){
			return 0;
		}
		else {
			//���[�U�[������
			return 1;
	    }
	}
	else{
		ene->MoveY(velo*2);
		//�ʏ�̂Q�{�̑����ŉ�ʃA�E�g
	}

	return 0;
}


int EnemyMove_TimerBomb(Enemy* ene, MyCharacter* mychar)
{
	double velo = ene->GetVelocity();
	double ang = ene->GetAngle();

	if(ene->GetCount() > 60){
		if((ene->GetCount()-60) % 50 == 0){
			ene->SetImgDiv((ene->GetCount()-60)/50);
			sound.PlaySoundHandle(SOUNDEFECT_TBOMBSECOND);
		}
		if((ene->GetCount()-60) >= 300){
			ene->SetState(ENEST_DEAD);
			return 1;
		}
	} else {
		ene->MoveX(velo*cos(ang));
		ene->MoveY(velo*sin(ang));
	}

	return 0;
}

int EnemyMove_MoveCircle(Enemy* ene, MyCharacter* mychar)
{
	int result = 0;

	double velo = ene->GetVelocity();
	double ang = ene->GetAngle();

	if(ene->GetCount() > 30 && ene->GetCount() <= 30+ene->GetBrakeCount()){
		ene->MoveX(velo*cos(ang));
		ene->MoveY(velo*sin(ang));

		ene->SetAngle(ang-PI/180*5);

		//�ݒ肳�ꂽ�ҋ@���ԕ���~
		result = 1;
		//��~�������e�����Ă�
	} else {
		ene->MoveX(velo*cos(ang));
		ene->MoveY(velo*sin(ang));	
	}
	
	return result;
}

int EnemyMove_MoveHexFlat(Enemy* ene, MyCharacter* mychar)
{
	int count = ene->GetCount();
	double velo = ene->GetVelocity();
	double ang = ene->GetAngle();

	if(count < 120){
		ene->SetColType(ENECTYPE_ALLNOCOL);
		ene->SetDrawEfect(EF_ALPHABLEND, (int)(255*((double)count/240)));
	} else if(count == 120){
		ene->SetColType(ENECTYPE_NORMAL);
		ene->SetDrawEfect(EF_NOEFECT, 255);
	} else {
		ene->SetAngle(ang+PI/180);
		ene->MoveY(velo);
		return 1;
	}

	return 0;
}

int EnemyMove_MoveHexClock(Enemy* ene, MyCharacter* mychar)
{
	int count = ene->GetCount();
	double velo = ene->GetVelocity();
	double ang = ene->GetAngle();

	if(count < 120){
		ene->SetColType(ENECTYPE_ALLNOCOL);
		ene->SetDrawEfect(EF_ALPHABLEND, (int)(255*((double)count/240)));
	} else if (count == 120){
		ene->SetColType(ENECTYPE_NORMAL);
		ene->SetDrawEfect(EF_NOEFECT, 255);	
	} else {
		ene->MoveY(velo);
		ene->SetColType(ENECTYPE_NORMAL);

		if(count%120 < 60){
			ene->SetAngle(ang+PI/180);
			return 0;
		}
		return 1;
	}

	return 0;
}

int EnemyMove_Thunder(Enemy* ene, MyCharacter* mychar)
{
	double px = ene->GetPosX();
	int count = ene->GetCount();

	if(count == 0){
		efect.SetEfect(px+GAME_X,0,EFECT_ASSERT,200,480);
	}
	if(count == 70){
		efect.SetEfect(px,0,EFECT_LIGHTNING);
	}
	if(count == 90){
		ene->SetState(ENEST_DEAD);
		return 1;
	}

	return 0;
}

int EnemyMove_Rush(Enemy* ene, MyCharacter* mychar)
{
	double velo = ene->GetVelocity();
	int count = ene->GetCount();

	double y = mychar->GetPosY() - (ene->GetPosY()+ene->GetHeight()/2);	//�}�C�L�����ƃG�l�~�[��y���������߂�
	double x = mychar->GetPosX() - (ene->GetPosX()+ene->GetWidth()/2);	//x���������߂�
	double angle = atan2(y,x);	//tan-1(x/y)�@�����Ƃ߂�
	double prv_ang = ene->GetAngle();

	angle = atan2(y,x);	//tan-1(x/y)�@�����Ƃ߂�
	ene->SetAngle(angle);

	if(count < 60){
		ene->MoveY(2);
		return 0;
	} else if(count < 60+ene->GetBrakeCount()){
		return 1;
	}

	ene->SetVelocity(velo+0.2);

	if(fabs(angle-prv_ang) > PI/180) {
		if(angle-prv_ang > 0) angle = prv_ang+PI/180;
		else angle = prv_ang-PI/180;
	}

	ene->SetAngle(angle);

	ene->MoveX(velo*cos(angle));
	ene->MoveY(velo*sin(angle));


	return 0;
}

int EnemyMove_Madness(Enemy* ene, MyCharacter* mychar)
{
	int count = ene->GetCount();

	double y = mychar->GetPosY() - (ene->GetPosY()+ene->GetHeight()/2);	//�}�C�L�����ƃG�l�~�[��y���������߂�
	double x = mychar->GetPosX() - (ene->GetPosX()+ene->GetWidth()/2);	//x���������߂�
	double prv_ang = ene->GetAngle();
	double angle = atan2(y, x);	//tan-1(x/y)�@�����Ƃ߂�
	double velo = ene->GetVelocity();

	if(ene->GetDamaged()){
		ene->SetImgDiv(1);
		ene->SetVelocity(velo+0.5);

		if(fabs(angle-prv_ang) > PI/180) {
			if(angle-prv_ang > 0) angle = prv_ang+PI/180;
			else angle = prv_ang-PI/180;
		}
		ene->SetAngle(angle);
	}

	velo = ene->GetVelocity();
	double ang = ene->GetAngle();

	ene->MoveX(velo*cos(ang));
	ene->MoveY(velo*sin(ang));

	return 1;
}

int EnemyMove_MoveHeadFlatMove(Enemy* ene, MyCharacter* mychar)
{
	int count = ene->GetCount();
	double velo = ene->GetVelocity();
	double ang = ene->GetAngle();

	if(count < 120){
		ene->SetColType(ENECTYPE_ALLNOCOL);
		ene->SetDrawEfect(EF_ALPHABLEND, (int)(255*((double)count/240)));
	} else if (count == 120){
		ene->SetColType(ENECTYPE_NORMAL);
		ene->SetDrawEfect(EF_NOEFECT, 255);	
	} else {
		ene->MoveX(velo*cos(ang));
		ene->MoveY(velo*sin(ang));
		return 1;
	}

	return 0;
}

int EnemyMove_MoveHexSq(Enemy* ene, MyCharacter* mychar)
{
	int count = ene->GetCount();
	double velo = ene->GetVelocity();
	double ang = ene->GetAngle();

	if(count < 120){
		ene->SetColType(ENECTYPE_ALLNOCOL);
		ene->SetDrawEfect(EF_ALPHABLEND, (int)(255*((double)count/240)));
	} else if (count == 120){
		ene->SetColType(ENECTYPE_NORMAL);
		ene->SetDrawEfect(EF_NOEFECT, 255);	
	} else {
		int d_count = count-120;

		if(d_count/200 == 0){
			ene->MoveY(velo);
		} else
		if(d_count/200 == 1){
			ene->MoveX(-velo);
		} else
		if(d_count/200 == 2){
			ene->MoveY(-velo);
		} else{
			ene->MoveX(velo);
		} 

		ene->SetColType(ENECTYPE_NORMAL);

		if(count%120 < 60){
			ene->SetAngle(ang+PI/180);
			return 0;
		}
		return 1;
	}

	return 0;
}

int EnemyMove_MoveSq(Enemy* ene, MyCharacter* mychar)
{
	int count = ene->GetCount();
	double velo = ene->GetVelocity();
	double ang = ene->GetAngle();

	if(count < 120){
		ene->SetColType(ENECTYPE_ALLNOCOL);
		ene->SetDrawEfect(EF_ALPHABLEND, (int)(255*((double)count/240)));
	} else if (count == 120){
		ene->SetColType(ENECTYPE_NORMAL);
		ene->SetDrawEfect(EF_NOEFECT, 255);	
	} else {
		int d_count = count-120;

		if(d_count/200 == 0){
			ene->SetAngle(PI/2);
			ene->MoveY(velo);
		} else
		if(d_count/200 == 1){
			ene->SetAngle(PI);
			ene->MoveX(-velo);
		} else
		if(d_count/200 == 2){
			ene->SetAngle(3*PI/2);
			ene->MoveY(-velo);
		} else{
			ene->SetAngle(0);
			ene->MoveX(velo);
		} 

		ene->SetColType(ENECTYPE_NORMAL);

		return 1;
	}

	return 0;
}

int (*EneMove_Operate[ENEMOVE_NUM])(Enemy* ene, MyCharacter* mychar) =
{
	EnemyMove_FlatY,
	EnemyMove_FlatMove,
	EnemyMove_MoveWaitY,
	EnemyMove_MoveWaitYNoAng,
	EnemyMove_MoveWaitYAccel,
	EnemyMove_MoveWaitYAccelNoAng,
	EnemyMove_MoveWaitYSlow,
	EnemyMove_MoveWaitYSlowNoAng,
	EnemyMove_TowardCenter,
	EnemyMove_TowardEdge,
	EnemyMove_AccelTurn,
	EnemyMove_TurnRightAngle,
	EnemyMove_TurnLeftAngle,
	EnemyMove_Turn90,
	EnemyMove_LaserShot,
	EnemyMove_LaserShotWait,
	EnemyMove_TimerBomb,
	EnemyMove_MoveCircle,
	EnemyMove_MoveHexFlat,
	EnemyMove_MoveHexClock,
	EnemyMove_Thunder,
	EnemyMove_Rush,
	EnemyMove_Madness,
	EnemyMove_MoveHeadFlatMove,
	EnemyMove_MoveHexSq,
	EnemyMove_MoveSq
};



int EnemyShot_FlatMove(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar)	
{	//�����ɔ���
	sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight(), ene->GetShotVelo(), ene->GetAngle(), ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY, SHOTCOLOR_RED);

	return 0;
}

int EnemyShot_FlatMove5(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar)	
{	//�����ɔ���
	if(ene->GetCount() % 5 != 0) return 2;
	sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight(), ene->GetShotVelo(), ene->GetAngle(), ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY, SHOTCOLOR_RED);

	if(ene->GetRapidNum() < 4) return 1;
	return 0;
}

int EnemyShot_ToWardMyChar(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar)
{	//�}�C�L�����Ɍ����Ĕ���

	/*
		atan2(double y, double x)�֐��́Ax/y�̃A�[�N�^���W�F���g�����߂�֐��ł���B		
		�܂�A�������W���ȍ��W�ɕϊ�����ۂ́A�p�x�����߂�Ƃ������ƁB
	*/

	double y = mychar->GetPosY() - (ene->GetPosY()+ene->GetHeight());	//�}�C�L�����ƃG�l�~�[��y���������߂�
	double x = mychar->GetPosX()+mychar->GetWidth()/2 - (ene->GetPosX()+ene->GetWidth()/2);	//x���������߂�
	double angle = atan2(y,x);	//tan-1(x/y)�@�����Ƃ߂�
	sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight(), ene->GetShotVelo(), angle, ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY,SHOTCOLOR_RED);

	return 0;
}

int EnemyShot_ToWardMyChar3(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar)
{	//�}�C�L�����Ɍ����Ĕ���

	if(ene->GetCount()%10 != 0) return 2;

	double y = mychar->GetPosY() - (ene->GetPosY()+ene->GetHeight());	//�}�C�L�����ƃG�l�~�[��y���������߂�
	double x = mychar->GetPosX()+mychar->GetWidth()/2 - (ene->GetPosX()+ene->GetWidth()/2);	//x���������߂�
	double angle = atan2(y,x);	//tan-1(x/y)�@�����Ƃ߂�
	sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight(), ene->GetShotVelo(), angle, ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY,SHOTCOLOR_RED);

	if(ene->GetRapidNum() < 2) return 1;
	return 0;
}

int EnemyShot_ToWardMyChar5(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar)
{	//�}�C�L�����Ɍ����Ĕ���

	if(ene->GetCount()%10 != 0) return 2;

	double y = mychar->GetPosY() - (ene->GetPosY()+ene->GetHeight());	//�}�C�L�����ƃG�l�~�[��y���������߂�
	double x = mychar->GetPosX()+mychar->GetWidth()/2 - (ene->GetPosX()+ene->GetWidth()/2);	//x���������߂�
	double angle = atan2(y,x);	//tan-1(x/y)�@�����Ƃ߂�
	sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight(), ene->GetShotVelo(), angle, ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY,SHOTCOLOR_RED);

	if(ene->GetRapidNum() < 4) return 1;
	return 0;
}

int EnemyShot_FrontRandom(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar)
{	//�O���Ɍ����ă����_���ɔ���
	int rand = GetRand(90) - 45;	//-30~30�͈̔͂Ń����_���Ȓl������
	double rand_ang = ((double)rand/180.0)*PI;
		//�����_���Ȓl��x���ƍl���A���W�A���ɕϊ�

	sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight(), ene->GetShotVelo(), rand_ang+PI/2, ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY,SHOTCOLOR_MAGENTA);

	return 0;
}

int EnemyShot_FrontRandom5(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar)
{	//�O���Ɍ����ă����_���ɔ���
	if(ene->GetCount()%5 != 0) return 2;

	int rand = GetRand(90) - 45;	//-30~30�͈̔͂Ń����_���Ȓl������
	double rand_ang = ((double)rand/180.0)*PI;
		//�����_���Ȓl��x���ƍl���A���W�A���ɕϊ�

	sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight(), ene->GetShotVelo(), rand_ang+PI/2, ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY,SHOTCOLOR_MAGENTA);

	if(ene->GetRapidNum() < 4) return 1;
	return 0;
}

int EnemyShot_MycharRandom3(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar)
{	//�O���Ɍ����ă����_���ɔ���
	if(ene->GetCount()%5 != 0) return 2;

	double y = mychar->GetPosY() - (ene->GetPosY()+ene->GetHeight());	//�}�C�L�����ƃG�l�~�[��y���������߂�
	double x = mychar->GetPosX()+mychar->GetWidth()/2 - (ene->GetPosX()+ene->GetWidth()/2);	//x���������߂�
	double angle = atan2(y,x);	//tan-1(x/y)�@�����Ƃ߂�

	int rand = GetRand(90) - 45;	//-30~30�͈̔͂Ń����_���Ȓl������
	double rand_ang = ((double)rand/180.0)*PI+angle;
		//�����_���Ȓl��x���ƍl���A���W�A���ɕϊ�

	sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight(), ene->GetShotVelo(), rand_ang, ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY,SHOTCOLOR_MAGENTA);

	if(ene->GetRapidNum() < 2) return 1;
	return 0;
}

int EnemyShot_MycharRandom5(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar)
{	//�O���Ɍ����ă����_���ɔ���
	if(ene->GetCount()%5 != 0) return 2;

	double y = mychar->GetPosY() - (ene->GetPosY()+ene->GetHeight());	//�}�C�L�����ƃG�l�~�[��y���������߂�
	double x = mychar->GetPosX()+mychar->GetWidth()/2 - (ene->GetPosX()+ene->GetWidth()/2);	//x���������߂�
	double angle = atan2(y,x);	//tan-1(x/y)�@�����Ƃ߂�

	int rand = GetRand(90) - 45;	//-30~30�͈̔͂Ń����_���Ȓl������
	double rand_ang = ((double)rand/180.0)*PI+angle;
		//�����_���Ȓl��x���ƍl���A���W�A���ɕϊ�

	sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight(), ene->GetShotVelo(), rand_ang, ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY,SHOTCOLOR_MAGENTA);

	if(ene->GetRapidNum() < 4) return 1;
	return 0;
}

int EnemyShot_Front3Way(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar)
{	//�O���Ɍ����ĂR���g�U����
	sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight(), ene->GetShotVelo(), (30.0/180.0)*PI+PI/2, ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY,SHOTCOLOR_BLUE);
	sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight(), ene->GetShotVelo(), 0+PI/2, ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY,SHOTCOLOR_BLUE);
	sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight(), ene->GetShotVelo(), (-30.0/180.0)*PI+PI/2, ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY,SHOTCOLOR_BLUE);

	return 0;
}

int EnemyShot_Front5Way(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar)
{	//�O���Ɍ����ĂT���g�U����
	for(int i = 0; i < 5; i++){
		sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight(), ene->GetShotVelo(), (((i*15.0)-30.0)/180.0)*PI+PI/2, ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY,SHOTCOLOR_BLUE);
	}

	return 0;
}

int EnemyShot_Front7WayHorm(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar)
{	//�O���Ɍ����ĂT���g�U���˂Q���z�[�~���O�e
	sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight(), ene->GetShotVelo(), (30.0/180.0)*PI+PI/2, ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY,SHOTCOLOR_BLUE);
	sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight(), ene->GetShotVelo(), (15.0/180.0)*PI+PI/2, ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY,SHOTCOLOR_BLUE);
	sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight(), ene->GetShotVelo(), 0+PI/2, ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY,SHOTCOLOR_BLUE);
	sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight(), ene->GetShotVelo(), (-15.0/180.0)*PI+PI/2, ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY,SHOTCOLOR_BLUE);
	sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight(), ene->GetShotVelo(), (-30.0/180.0)*PI+PI/2, ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY,SHOTCOLOR_BLUE);

	return 0;
}

int EnemyShot_AllRange30(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	for(int i = 0; i < 12; i++){
		sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight(), ene->GetShotVelo(), (((30.0)*i-180.0)/180.0)*PI+PI/2, ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY,SHOTCOLOR_BLUE);	
	}

	return 0;
}

int EnemyShot_AllRange15(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	for(int i = 0; i < 24; i++){
		sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight(), ene->GetShotVelo(), (((15.0)*i-180.0)/180.0)*PI+PI/2, ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY,SHOTCOLOR_BLUE);	
	}

	return 0;
}

int EnemyShot_Spiral(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	if(ene->GetCount()%2 != 0) return 2;

	double angle = (((ene->GetCount()+GetRand(10)-5)*6)/180.0)*PI+PI/2;

	sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight(), ene->GetShotVelo(), angle, ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY,SHOTCOLOR_YELLOW);

	if(ene->GetRapidNum() < 18) return 1;
	return 0;
}

int EnemyShot_AllRangeRand16(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	for(int j = 0; j < 4; j++){
		for(int i = 0; i < 4; i++){
			int rand = GetRand(90) + 90*j;
			double rand_ang = ((double)rand/180.0)*PI;
				//�����_���Ȓl��x���ƍl���A���W�A���ɕϊ�

			sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight(), ene->GetShotVelo(), rand_ang+PI/2, ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY,SHOTCOLOR_MAGENTA);
		}
	}

	return 0;
}

int EnemyShot_AllRangeRand30(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	if(ene->GetRapidNum() == 0){
		for(int i = 0; i < 12; i++){
			sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight(), ene->GetShotVelo(), (((30.0)*i-180.0)/180.0)*PI+PI/2, ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY,SHOTCOLOR_BLUE);	
		}

		return 1;
	}

	if(ene->GetCount()%10 == 0){
		int rand = GetRand(90) - 45;	//-30~30�͈̔͂Ń����_���Ȓl������
		double rand_ang = ((double)rand/180.0)*PI;
			//�����_���Ȓl��x���ƍl���A���W�A���ɕϊ�

		sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight(), ene->GetShotVelo(), rand_ang+PI/2, ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY,SHOTCOLOR_MAGENTA);
		if(ene->GetRapidNum() < 5) return 1;
		return 0;
	}
	return 2;
}

int EnemyShot_Laser(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight()/2, ene->GetShotVelo(), ene->GetAngle(), ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY,0);
	return 1;	
}

int EnemyShot_NBombFlame(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	int rand = GetRand(1);
	int sht_ope = SHOTOPE_ACCEL;

	if(rand == 0) sht_ope = SHOTOPE_ROLLSPIRAL;

	for(int i = 0; i < 12; i++){
		sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight()/2, ene->GetShotVelo(), (((30.0)*i-180.0)/180.0)*PI+PI/2, ene->GetShotIndex(), sht_ope, SHOTSIDE_ENEMY,rand);
	}

	return 0;
}

int EnemyShot_TBombFlame(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	for(int i = 0; i < 24; i++){
		sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight()/2, ene->GetShotVelo(), (((15.0)*i-180.0)/180.0)*PI+PI/2, ene->GetShotIndex(), SHOTOPE_ACCEL, SHOTSIDE_ENEMY,1);
		sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight()/2, ene->GetShotVelo(), (((15.0)*i-180.0)/180.0)*PI+PI/2, ene->GetShotIndex(), SHOTOPE_ROLLSPIRAL, SHOTSIDE_ENEMY,0);
	}

	return 0;
}

int EnemyShot_HexShot(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	for(int i = 0; i < 6; i++){
		sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight()/2, ene->GetShotVelo(), ene->GetAngle()+(PI/3)*i, ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY, SHOTCOLOR_RED);
	}

	return 0;
}

int EnemyShot_TriShot(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	for(int i = 0; i < 3; i++){
		sht_mgr->SetShot(ene->GetPosX()+ene->GetWidth()/2, ene->GetPosY()+ene->GetHeight()/2, ene->GetShotVelo(), ene->GetAngle()+2*(PI/3)*i, ene->GetShotIndex(), ene->GetShotMoveIndex(), SHOTSIDE_ENEMY, SHOTCOLOR_RED);
	}

	return 0;
}

int EnemyShot_Thunder(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	efect.SetEfect(ene->GetPosX()+GAME_X,0,EFECT_THUNDER);
	sht_mgr->SetShot(ene->GetPosX(),0,0,0,SHOT_THUNDER,SHOTOPE_FLASH,SHOTSIDE_ENEMY,0,FALSE);
	return 0;
}

int (*EneShot_Operate[ENESHOT_NUM])(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar) =
{
	EnemyShot_FlatMove,		//�����ɔ���
	EnemyShot_FlatMove5,
	EnemyShot_ToWardMyChar,	//�}�C�L�����Ɍ����Ĕ���
	EnemyShot_ToWardMyChar3,
	EnemyShot_ToWardMyChar5,
	EnemyShot_FrontRandom,	//�O���Ɍ����ă����_���ɔ���
	EnemyShot_FrontRandom5,
	EnemyShot_MycharRandom5,
	EnemyShot_MycharRandom3,
	EnemyShot_Front3Way,	//�O���Ɍ����ĂR���g�U����
	EnemyShot_Front5Way,	//�O���Ɍ����ĂT���g�U����
	EnemyShot_Front7WayHorm, //�O���Ɍ����ĂT���g�U���˂Q���z�[�~���O�e
	EnemyShot_AllRange30,
	EnemyShot_AllRange15,
	EnemyShot_AllRangeRand16,
	EnemyShot_Spiral,
	EnemyShot_AllRangeRand30,
	EnemyShot_Laser,
	EnemyShot_NBombFlame,
	EnemyShot_TBombFlame,
	EnemyShot_HexShot,
	EnemyShot_TriShot,
	EnemyShot_Thunder
};

