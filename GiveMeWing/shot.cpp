#include <DxLib.h>
#include "database.h"
#include "shot.h"
#include "main.h"
#include "sht_ope.h"
#include "image.h"
#include "efect.h"

/*
�N���X�� : Shot
	�}�C�L������G�l�~�[�����˂���e

	�p�� : Character

	�L�q�t�@�C�� : shot.h
*/

int Shot::Update(MyCharacter* mychar, ShotMgr* sht_mgr)
{
	if(life <= 0) return 1;;

	int result = 0;

	mv_x = 0; mv_y = 0;	//�ړ��ʃ��Z�b�g

	//����֐��Ăяo��
	result = Shot_Operate[ope_type](this, sht_mgr, mychar);
	//����֐��́A �t�@�C��:sht_ope.cpp �ɋL�q���Ă���

	if( death_type != SHOTDF_NODEATH && (
		((px+width < 0 || px > GAME_WIDTH) && (death_type != SHOTDF_NOSIDE)) ||
		py+height < 0 || (py > GAME_HEIGHT && death_type != SHOTDF_NOUNDER)) ) result = 1;

	count++;
	return result;
}

void Shot::MoveX(double a_x)
{
	px += a_x;
	mv_x = a_x;
}

void Shot::MoveY(double a_y)
{
	py += a_y;
	mv_y = a_y;
}

/*
�V���b�g�ƃL�����N�^�̓����蔻��͕��G�ŁA�P���ȓ����蔻��ł́A
�΂߈ړ����̏�����A���������e�Ȃǂ̓����蔻��Ƀo�O��������B

���̂��߁A�L�������ړ��ʂ��g���Ĉꎟ�������̌X�������߁A
���ݕ��P�ŕ␳���Ȃ��画����s���Ă���B

�܂�͓����蔻����s���Ă���킯�Ȃ̂ŁA�����c������K�v�͂Ȃ����A
����΂����gg���Ă��o�Ă��Ȃ��̂ŁA�����c���������Ȃ璼�ڕ����āB
*/
bool Shot::Collition(double a_x0, int a_w, double a_y0, int a_h)
{
	double a_x1 = a_x0+a_w;	//����a�̉E��x
	double a_y1 = a_y0+a_h; //a�̉E��y
	double b_x0 = px;		//����b(=this)�̍���x
	double b_y0 = py;		//b�̍���y
	double b_x1 = px+width;	//b�̉E��y
	double b_y1 = py+height;//b�̉E��y

	double grad = 0;	//�ꎟ�������̌X��
	if(mv_x != 0 && mv_y != 0){	//���A���Ƃ��ɓ����Ă���΁A
		grad = mv_y/mv_x;	//�X���̌v�Z
	}
	//���̂悤�Ȍv�Z�����Ă���̂́Amv_x=0�ƂȂ����Ƃ��̗돜��h�����߂ł���

	if(mv_x != 0){	//x�������ɓ����Ă���΁A
		for(int i = 0; i < (int)mv_x; i++){	//���������̏����_�؎̂ĕ�����
			double adj_x0 = b_x0 - i;		//������������x���猸�Z
			double adj_y0 = b_y0 - i * grad;	//���������X��������y���猸�Z
			double adj_x1 = adj_x0 + width;	//�E��x
			double adj_y1 = adj_y0 + height;//�E��y

			if((a_x0 < adj_x1) && (adj_x0 < a_x1) && (a_y0 < adj_y1) && (adj_y0 < a_y1)){	
				//�␳�����l�œ����蔻�莮
				return TRUE;	//�������Ă�����TRUE��Ԃ�
			}
		}
	} 
	else if(mv_y != 0){	//x�������ɓ����Ă��Ȃ��āA����y�������ɂ͓����Ă���΁A
		for(int i = 0; i < (int)mv_y; i++){	//���������̏����_�؎̂ĕ�����

			//��{�Ax�������ɓ������Ƃ��Ɠ��������A�����ł�x�������̕␳���K�v�Ȃ��̂ŁA
			double adj_y0 = b_y0 - i;
			double adj_y1 = adj_y0 + height;
			//y���݂̂𐂒��ɕ␳����
		
			if((a_x0 < b_x1) && (b_x0 < a_x1) && (a_y0 < adj_y1) && (adj_y0 < a_y1)){	
				//�␳�����l�œ����蔻�莮
				return TRUE;	//�������Ă�����TRUE��Ԃ�
			}
		}
	}

	if((a_x0 < b_x1) && (b_x0 < a_x1) && (a_y0 < b_y1) && (b_y0 < a_y1)){
		//�Ō�ɁA���݈ʒu�œ����蔻��
		return TRUE;	//�������Ă�����TRUE��Ԃ�
	}

	return FALSE;
}

bool Shot::JudgeGraze(double x0, int a_w, double y0, int a_h)
{
	if(!graze_flag) return FALSE; 
	if(!graze && Collition(x0, a_w, y0, a_h)){graze = TRUE; return TRUE;}
	return FALSE;
}

void Shot::Draw()
{
	//�������`���A�摜�̒��S�ɗ���悤�ɕ␳
	float draw_x = (float)px - (img_w/2 - width/2);
	float draw_y = (float)py - (img_h/2 - height/2);
	//

	if(graze) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
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

	//��]�`��
	image.DrawRotaImageF(draw_x+img_w/2, draw_y+img_h/2, 1.0, angle+PI/2, img_index, TRUE, img_div);
	//��]�`��́A�摜�̒��S���W���w�肷��

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 126);

#ifdef DEBUG_CODE
	_DrawBox(int(px), int(py), int(px)+width, int(py)+height, GetColor(0,0,255),FALSE);
	//�������`�͈͂̕\��
#endif
}

/*
�N���X�� : ShotMgr
	�V���b�g�̊Ǘ��N���X

	�L�q�t�@�C�� : shot.h
*/

ShotMgr::ShotMgr()
{
	for(int i = 0; i < EXISTSHOT_MAX; i++){
		//�V���b�g�͗\�񂵂Ă��Ȃ��Ƃ���NULL�Ȃ̂ŁA
		shot[i] = NULL;
		//NULL�ŏ�����
	}
}

ShotMgr::~ShotMgr()
{
	for(int i = 0; i < EXISTSHOT_MAX; i++){
		if(shot[i] == NULL) continue;	//�\�񂳂�Ă��Ȃ��Ȃ炻�̂܂�
		delete shot[i];	//����Ă�����A�j��
		//�Ȃ��A����̓f�X�g���N�^�Ȃ̂ŁANULL���l�߂�K�v�͂Ȃ�
	}
}

void ShotMgr::Draw()
{
#ifdef DEBUG_CODE
	int num = 0;
#endif

	for(int i = 0; i < EXISTSHOT_MAX; i++){
		if(shot[i] == NULL) continue;	//�\�񂳂�Ă��Ȃ��Ȃ�`�悵�Ȃ�
		shot[i]->Draw();	//����Ă�����A�`��

#ifdef DEBUG_CODE
		num++;
#endif

	}

#ifdef DEBUG_CODE
	int color = GetColor(255,255,255);
	if(num > 90) color = GetColor(255,0,0);

	DrawFormatString(50,0,color, "�e�̐�:%d", num);
#endif
}

void ShotMgr::Update(MyCharacter* mychar)
{
	for(int i = 0; i < EXISTSHOT_MAX; i++){
		if(shot[i] == NULL) continue;	//�\�񂳂�Ă��Ȃ��Ȃ�X�V���Ȃ�
		if(shot[i]->Update(mychar, this) == 1){	//����Ă�����A�X�V
			delete shot[i];	//1���A���Ă�����I�u�W�F�N�g��j��
			shot[i] = NULL;	//NULL���߂�
			continue;
		}
	}

	ColShotCalc();
}

bool ShotMgr::ColCharCalc(Character* chr, int side)
{
	bool result = FALSE;
	for(int i = 0; i < EXISTSHOT_MAX; i++){
		if(shot[i] == NULL) continue;	//�\�񂳂�Ă��Ȃ��Ȃ�A���肵�Ȃ�
		if(shot[i]->GetSide() == side) continue;	
				//�V���b�g�̌�����ƁA�픻��҂������T�C�h�Ȃ�A���肵�Ȃ�

		if(shot[i]->Collition(chr->GetPosX(), chr->GetWidth(),
				chr->GetPosY(), chr->GetHeight())){
			//�������Ă���΁A

			shot[i]->SetHit(TRUE);	//�����������Ƃ��V���b�g�ɓ`����
			chr->Damage(shot[i]->GetPower());	//�����ŗ^����ꂽ�L�����^�Ƀ_���[�W

			if(shot[i]->GetThrough() != SHOTTH_ENEMY && shot[i]->GetThrough() != SHOTTH_ALL){	//�ђʑ����łȂ���΁A
				delete shot[i];	//�����������_�ŃI�u�W�F�N�g��j��
				shot[i] = NULL;	//NULL���l�߂�
			}

			if(side == SHOTSIDE_MYCHAR) return TRUE;	//�}�C�L�����͈�x�Ɉꔭ����������s��Ȃ�
			result = TRUE;
		}
	}
	return result;
}

void ShotMgr::ColShotCalc()
{
	for(int i = 0; i < EXISTSHOT_MAX; i++){
		if(shot[i] == NULL) continue;	//�\�񂳂�Ă��Ȃ��Ȃ�A���肵�Ȃ�

		for(int j = 0; j < EXISTSHOT_MAX; j++){
			if(shot[j] == NULL) continue;	//�\�񂳂�Ă��Ȃ��Ȃ�A���肵�Ȃ�
			if(shot[j]->GetSide() == shot[i]->GetSide()) continue;
			if(shot[i]->GetThrough() != SHOTTH_SHOT && shot[i]->GetThrough() != SHOTTH_ALL){
				if(shot[j]->GetThrough() != SHOTTH_SHOTED) continue;
			}

			if(shot[j]->Collition(shot[i]->GetPosX(), shot[i]->GetWidth(), shot[i]->GetPosY(), shot[i]->GetHeight())){
				shot[j]->Damage(1);
				if(shot[j]->GetThrough() == SHOTTH_SHOTED){
					shot[i]->Damage(1);
				}
			}
		}
	}
}

int ShotMgr::JudgeGraze(double x0, int width, double y0, int height, int side)
{
	int num = 0;
	for(int i = 0; i < EXISTSHOT_MAX; i++){
		if(shot[i] == NULL) continue;	//�\�񂳂�Ă��Ȃ��Ȃ�A���肵�Ȃ�
		if(shot[i]->GetSide() == side) continue;	//�����T�C�h�Ȃ画�肵�Ȃ�

		if(shot[i]->JudgeGraze(x0, width, y0, height)) num++;
	}

	return num;
}


/*
�\�񎞁A�L�����N�^����󂯂Ƃ�f�[�^�́A

����x,y�@�ړ����x�@�ړ��p�x�@�e�̃f�[�^�^�C�v�@�e�̐���^�C�v

�܂�A

�ǂ̂悤�Ȓe : �e�̃f�[�^�^�C�v
�ǂ̂悤�ɔ��� : ����x,y�@�ړ����x�@�ړ��p�x
�ǂ̂悤�ɓ����� : �e�̐���^�C�v

�Ƃ����������Ƃł���B
*/
int ShotMgr::SetShot(double px, double py, double velo, double angle, int shot_type, int ope_type, int side, int color,bool adj)
{
	for(int i = 0; i < EXISTSHOT_MAX; i++){
		if(shot[i] != NULL) continue;	//�\�񂳂�āu����v�Ȃ�A�\�񂵂Ȃ�

		ShotData_t* data = sht_db.QueryShotData(shot_type);	//�V���b�g�f�[�^�N�G��
		
		int adj_y = 0;	//�␳y���O�ɉ���
		int adj_x = 0;	//�␳x���O�ɉ���
		if(adj && side == SHOTSIDE_MYCHAR) adj_y = data->height;
			//�����A�V���b�g���}�C�L�����ɂ����̂ł���΁A�V���b�g�̈ʒu���V���b�g�̍����������␳

		if(adj) adj_x = data->width/2;

		shot[i] = new Shot(px-adj_x, py-adj_y, data->width, data->height, data->life, 
			data->img_num, velo, angle, data->power, data->through, ope_type, side, color, data->death_type, data->graze, data->efect, data->efect_par);
									//����Ă��Ȃ��Ȃ�A�����ɗ\�񂷂�
		return 0;
	}

	return 1;	//�S���T���āA�\��ł��Ȃ�������A�P��Ԃ�
}