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
�N���X�� : Enemy
	�G�l�~�[�i�G�j��\���N���X

	�p�� : Character

	�L�q�t�@�C�� : enemy.h
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

	img_index = e_data->img_num;	//�f�[�^�x�[�X���C���[�W�n���h�����󂯎��
	image.GetImageSize(img_index, &img_w, &img_h);	//�O���t�T�C�Y�̎擾
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

	//�V���b�g�C���^�[�o����-1�Ȃ�A�V���b�g�������I�Ɍ����Ȃ��Ƃ������ƁB
	if(shot_interval == -1) interval_count = -1;
	else interval_count = 0;
}

#define FLASH_COUNT 2
void Enemy::Update(ShotMgr* sht_mgr, MyCharacter* mychar, ItemMgr* item_mgr, int g_level)
{
	int move_result;
	//���쐧��֐��Ăяo��
	move_result = EneMove_Operate[move_type](this, mychar);

	if(turn == ENEDIR_MYCHAR){	//��Ƀ}�C�L����������^�C�v�ł����
		double y = mychar->GetPosY() - (py+height/2);	//�}�C�L�����ƃG�l�~�[��y���������߂�
		double x = mychar->GetPosX() - (px+width/2);	//x���������߂�
		angle = atan2(y,x);	//tan-1(x/y)�@�����Ƃ߂�
	}
	//�ړ�����֐����甭�ˋ����o�Ă��āA
	if((move_result == 1 && interval_count == 0) || move_result == 2) {
		Shot(sht_mgr, mychar);
	}
	//�C���^�[�o���łȂ���΃V���b�g����

	//�����̐���֐��́A �t�@�C��:ene_ope.cpp �ɋL�q���Ă���

	if(col_type != ENECTYPE_ALLNOCOL){
		if(sht_mgr->ColCharCalc(this, SHOTSIDE_ENEMY)){
			flash_count = FLASH_COUNT;
			damaged = TRUE;
			if(counter == ENECOUNTER_COLSHOT && life > 0) Shot(sht_mgr, mychar, TRUE);
		}	//�V���b�g�Ƃ̓����蔻��
	}

	if((px+width < 0) || (px > GAME_WIDTH) || (py+height < 0) || (py > GAME_HEIGHT)){
		//��ʊO�ɏo�Ă���΁A
		state = ENEST_DEAD;	//���S
	}
	if(life <= 0) {
		state = ENEST_DEAD;	//�̗͂������Ȃ���Ύ��S����

		item_mgr->SetItem(px+width/2, py+height/2, item_value);	//���󎞂̃A�C�e�����Z�b�g
		efect.SetEfect(px+width/2+GAME_X,py+height/2, EFECT_SMALLBURN, width, GetColor(255,0,0));	//���j�G�t�F�N�g�Z�b�g
		if(score != 0) efect.SetEfect(px+width/2+GAME_X,py, EFECT_SCORE, score);
		mychar->AddScore(score);	//�X�R�A���v���X

		sound.PlaySoundHandle(SOUNDEFECT_ENEBURN);

		if(counter == ENECOUNTER_DEAD) Shot(sht_mgr, mychar, TRUE);	//�J�E���^�[�V���b�g

		if(g_level == 2){	//�n�[�h���x���Ȃ�
			if(GetRand(2) == 0){	//�@1/3�̊m���Ńf�X�J�E���^�[
				double y = mychar->GetPosY() - (py+height/2);	//�}�C�L�����ƃG�l�~�[��y���������߂�
				double x = mychar->GetPosX()+mychar->GetWidth()/2 - (px+width/2);	//x���������߂�
				double angle = atan2(y,x);	//tan-1(x/y)�@�����Ƃ߂�

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
	//�������`���A�摜�̒��S�ɗ���悤�ɕ␳
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
	} 	//��]�`��́A�摜�̒��S���W���w�肷��
	else image.DrawImageF(draw_x, draw_y, img_index, TRUE, img_div);
			//��]�`��łȂ���΂��̂܂ܕ`��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#ifdef DEBUG_CODE
	_DrawBox(int(px), int(py), int(px)+width, int(py)+height, GetColor(0,0,255),FALSE);
	//�������`�͈͂̕\��
#endif
}



/*
�N���X�� : EnemyMgr
	�G�l�~�[�̊Ǘ��N���X

	�L�q�t�@�C�� : enemy.h
*/

EnemyMgr::EnemyMgr(int num, EnemyInitData_t* init_data)
{
	ene_num = num;	//�G�l�~�[�����m��
	enemy = (Enemy**)malloc(sizeof(Enemy)*ene_num);	
		//�G�l�~�[�̐������A�G�l�~�[�|�C���^�𓮓I�m�ہi�|�C���^�̔z��

	for(int i = 0; i < ene_num; i++){
		enemy[i] = new Enemy(init_data[i].x0, init_data[i].y0, init_data[i].encount,
			init_data[i].shot_interval,init_data[i].shot_index,init_data[i].shotmove_index,
			init_data[i].brake_count,init_data[i].velocity,init_data[i].move_type,
			init_data[i].shot_type,init_data[i].shot_velo,init_data[i].angle, init_data[i].data_index);
				//�G�l�~�[�I�u�W�F�N�g�̃C���X�^���X���쐬
	}
}

EnemyMgr::~EnemyMgr()
{
	for(int i = 0; i < ene_num; i++){
		delete enemy[i];
	}	//�܂��A�G�l�~�[�I�u�W�F�N�g��j�����Ă���
	free(enemy);	//�I�u�W�F�N�g�z���j��
}

void EnemyMgr::Update(ShotMgr* sht_mgr, MyCharacter* mychar, ItemMgr* item_mgr, int count, int game_level)
{
	for(int i = 0; i < ene_num; i++){
		if(enemy[i]->GetState() == ENEST_STANDBY){
			//�X�^���o�C��Ԃ�
			if(enemy[i]->GetEncount() == count) enemy[i]->SetState(ENEST_EXIST);
				//�������J�E���g���o�߂��Ă����瑶�݂�����
			else continue;//���Ă��Ȃ��Ȃ甽��
		}
		if(enemy[i]->GetState() == ENEST_DEAD) continue;

		enemy[i]->Update(sht_mgr, mychar, item_mgr, game_level);
	}

}

bool EnemyMgr::Collition(int a_x, int a_w, int a_y, int a_h)
{
	for(int i = 0; i < ene_num; i++){
		if(enemy[i]->GetState() != ENEST_EXIST) continue;	//���݂��Ă��Ȃ��Ȃ画����s��Ȃ�
		if(enemy[i]->GetColType() != ENECTYPE_NORMAL) continue;

		if(enemy[i]->Collition(a_x, a_w, a_y, a_h)) return TRUE;
	}
	return FALSE;
}

void EnemyMgr::Draw(int count, MyCharacter* mychar)
{
	for(int i = 0; i < ene_num; i++){
		if(enemy[i]->GetState() == ENEST_EXIST && !enemy[i]->GetOnTheGround()) enemy[i]->Draw(mychar);
			//���݂��Ă���Ȃ畁�ʂɕ`��

		if(enemy[i]->GetState() == ENEST_STANDBY && enemy[i]->GetPosY() >= 480 
			&& enemy[i]->GetEncount()-100 <= count && !(enemy[i]->GetEncount() < count)){
				//��ʉ��ɃX�^���o�C���ŁA���o�ꂪ�P�O�O�J�E���g�ȓ��Ȃ�
			if(enemy[i]->GetEncount()-100 == count){
				sound.PlaySoundHandle(SOUNDEFECT_ENEASERT);
			}

			//�A�T�[�g�̕\��
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (count%30)*6);	//�A���t�@�u�����h�ŕ`��
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
			//���݂��Ă���Ȃ畁�ʂɕ`��

		if(enemy[i]->GetState() == ENEST_STANDBY && enemy[i]->GetPosY() >= 480 
			&& enemy[i]->GetEncount()-100 <= count && !(enemy[i]->GetEncount() < count)){
				//��ʉ��ɃX�^���o�C���ŁA���o�ꂪ�P�O�O�J�E���g�ȓ��Ȃ�

			//�A�T�[�g�̕\��
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (count%30)*6);	//�A���t�@�u�����h�ŕ`��
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
			//���݂��Ă��Ȃ��Ȃ画�肵�Ȃ�
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
