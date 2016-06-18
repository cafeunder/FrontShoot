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
	//�A�^�b�N�T�e���C�g�̈ړ�����
	if(mychar->GetSateNum() == 2){
		sate->SetPosY(mychar->GetPosY()+mychar->GetHeight()/2+SATEATK_ADJY/2);

		if(sate->GetNum() == 0) sate->SetPosX(mychar->GetPosX()+mychar->GetWidth()/2 - SATEATK_ADJX);
		else sate->SetPosX(mychar->GetPosX()+mychar->GetWidth()/2 + SATEATK_ADJX);

		return;
	}

	sate->SetPosY(mychar->GetPosY()+mychar->GetHeight()/2+SATEATK_ADJY);
	sate->SetPosX(mychar->GetPosX()+mychar->GetWidth()/2);
}

#define SATEGRD_COUNT 60	//������̃J�E���g
#define SATEGRD_R 60	//���a
void SateMove_Guard(MyCharacter* mychar, Satellite* sate)
{	//�K�[�h�T�e���C�g�̈ړ�����
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
{	//�A�^�b�N�T�e���C�g�̃V���b�g����
	double bf_x = px;
	double bf_y = py;

	ene_mgr->QueryNearEnemyPos(px, py, &bf_x, &bf_y);
	double ang = atan2(bf_y-py,bf_x-px);
	if(ang == 0.0) ang = -PI/2;

	sht_mgr->SetShot(px, py, 10, ang, SHOT_SATEATKSHOT, SHOTOPE_FLATMOVE, SHOTSIDE_MYCHAR, 0);
	return 8;
}

int SateShot_Guard(double px, double py, ShotMgr* sht_mgr, EnemyMgr* ene_mgr)
{	//�K�[�h�T�e���C�g�̃V���b�g����
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
�N���X�� : Satellite
	�}�C�L�������T�|�[�g����q��

	�p�� : Character

	�L�q�t�@�C�� : satellite.h
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
	SateMove[type](mychar, this);	//�ړ�����

	if(input.GetInput(INPUT_SATECHANGE) == 1){
		type = (type+1)%2;	
	}

	//�X�}�b�V�������n
	if(JudgeEnableSmash(mychar) && sht_mgr != NULL){
		move = TRUE;
		shot_interval = SateSmash[type](mychar, this, sht_mgr);	//�X�}�b�V���̗\��
		goto MAINEND;	//�X�}�b�V������������A����ȊO�̓���̓X�L�b�v����
	} else move = FALSE;
	//
	
	if(shot_interval == 0 && input.GetInput( INPUT_SHOT ) > 0 && sht_mgr != NULL) {
		//�V���b�g�C���^�[�o����ԂłȂ��A�V���b�g�{�^����������Ă�����
		shot_interval = SateShot[type](px, py, sht_mgr, ene_mgr);	//�V���b�g����
	}

MAINEND:

	count++;	//�J�E���g��i�߂�
	if(shot_interval > 0) shot_interval--;

	return 0;
}

void Satellite::Draw()
{
	//�������`���A�摜�̒��S�ɗ���悤�ɕ␳
	float draw_x = (float)px - img_w/2;
	float draw_y = (float)py - img_h/2;
	//

	image.DrawImageF(draw_x, draw_y, img_index, TRUE, type);
		//�`��

#ifdef DEBUG_CODE
	_DrawBox(int(px), int(py), int(px)+width, int(py)+height, GetColor(0,0,255),FALSE);
	//�������`�͈͂̕\��
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
		//�X�}�b�V���L�[�����͂���Ă��Ȃ���΁A�����Ȃ�

	if(input.GetInput( INPUT_SATESMASH ) == 1){	
		//�X�}�b�V���L�[���͂̏���Ȃ�΁A

		if(!mychar->JudgeMaxCharge()) smash = FALSE;
			//�`���[�W�����^�����ǂ������ׁA���^���łȂ���΋����Ȃ�
		else {smash = TRUE; mychar->AddCharge(-29);}
			//���^���Ȃ�A�X�}�b�V���������A���񂾂����߂Ƀ`���[�W�����炷
	}

	if(input.GetInput( INPUT_SATESMASH ) > 1 && smash == TRUE){
		//�X�}�b�V���L�[�����͂��ꑱ���Ă��āA�X�}�b�V����������ā@���@���@�Ȃ�

		if(mychar->GetChargePer() == 0) smash = FALSE;
			//���݂̃`���[�W���𒲂ׁA�O�Ȃ狖���Ȃ�
		else {smash = TRUE; mychar->AddCharge(-1);}
			//�łȂ���΁A�X�}�b�V���������A�`���[�W�����炷
	}

	return smash;
}