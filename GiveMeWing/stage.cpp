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
	count = 0;	//�J�E���^�̏�����
	boss_result = 0;
	sound_volume = 255;

	StageData_t* stg_data = stg_db.QueryStageData(stg_index);	//�X�e�[�W�f�[�^�̓ǂݍ���
	boss_index = stg_data->boss_index;	//�{�X�f�[�^���L��
	boss_count = stg_data->boss_count;	//�{�X�o���J�E���g���L��
	sound_index = stg_data->sound_index;
	sfade_count = stg_data->sfade_count;


	change_sound = stg_data->change_sound;
	schange_count = stg_data->schange_count;
	scfade_count = schange_count - 255;

	sht_mgr = new ShotMgr();	//�V���b�g�}�l�[�W���̃C���X�^���X�쐬
	item_mgr = new ItemMgr();	//�A�C�e���}�l�[�W���̃C���X�^���X�쐬

	//�G�l�~�[������
	const char* file_p;

	switch(a_level){
	case 0: file_p = stg_data->efile_name; break;
	case 1: file_p = stg_data->sfile_name; break;
	case 2: file_p = stg_data->hfile_name; break;
	}

	int file_han = FileRead_open(file_p);	//�t�@�C���ǂݍ���

	int ene_num; EnemyInitData_t* init_data;	//�K�v�������ϐ�

	FileRead_read(&ene_num, sizeof(int), file_han);	//�G�̐���ǂݍ���

			//�G�̐��������������m��
	init_data = (EnemyInitData_t*)malloc(sizeof(EnemyInitData_t)*ene_num);
	FileRead_read(init_data, sizeof(EnemyInitData_t)*ene_num, file_han);
			//�G�̏������f�[�^��ǂݍ���

	assert(FileRead_eof(file_han) != 0);	//�t�@�C���̏I�[�܂œǂݍ���ł��Ȃ���΂Ȃ�Ȃ�

	ene_mgr = new EnemyMgr(ene_num, init_data);	
			//�������f�[�^�ŃG�l�~�[�}�l�[�W���̃C���X�^���X�쐬

	FileRead_close(file_han);	//�t�@�C���N���[�Y
	free(init_data);			//�������̊J��
	//

	back_g = new BackGround(stg_data->fgimg_index, stg_data->bgimg_index, stg_data->f_velocity, stg_data->b_velocity);	//�w�i�̃C���X�^���X�쐬

	mes_data = stg_data->mes_data;	//���b�Z�[�W�f�[�^�|�C���g���L��

	boss = NULL;

#ifndef DEBUG_CODE
	ope_area->SetMessage(mes_data[0].message, mes_data[0].operater_type, mes_data[0].brake_count);
#endif
}

Stage::~Stage()
{
	//�e�C���X�^���X�̔j��
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

	int chr_result = 0;	//�Ԃ��l

	chr_result = mychar->Update(ene_mgr,sht_mgr, boss);//�}�C�L�����̍X�V�i�Q�[���I�[�o�[�Ȃ�1���Ԃ�
	sht_mgr->Update(mychar);//�V���b�g�̍X�V
	item_mgr->Update(mychar);//�A�C�e���̍X�V

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

	ene_mgr->Update(sht_mgr,mychar,item_mgr,count,game_level);//�G�l�~�[�̍X�V



	//���b�Z�[�W����
	int i = 0;
	while(i < STAGEMES_MAXNUM){	//�X�e�[�W���b�Z�[�W�\��`�F�b�N
		if(mes_data[i].count == -3) break;	//���݃��b�Z�[�W�f�[�^���ԕ��Ȃ�`�F�b�N�I��
		if(mes_data[i].count > count) break;
			//���݃��b�Z�[�W�f�[�^�̃J�E���g�����݃J�E���g�����傫����΁A
			//�������̌�͒T���������ʂȂ̂Ń`�F�b�N�I��

		if(count == mes_data[i].count){ //�����A���݃J�E���g�����b�Z�[�W�\��J�E���g�Ȃ�
			ope_area->SetMessage(mes_data[i].message, mes_data[i].operater_type, mes_data[i].brake_count);
			//���b�Z�[�W��\��
		}

		i++;
	}
	//



	if(!ope_area->GetDrawMes()) count++;	//�I�y���[�^�[�G���A�\�����łȂ���΃J�E���g��i�߂�

	if(CheckHitKey( KEY_INPUT_D ) == 1){
		count = KeyInputNumber(0,0,boss_count,0,TRUE);
		ope_area->Reset();
	}

	if(CheckHitKey( KEY_INPUT_F ) == 1){
		sound.StopSoundHandle(sound_index);
		return 2;
	}

	//�{�X�J�E���g�ɒB���Ă���΁A
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


	}	//�N���A��m�点�邽��2��Ԃ��i������
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
	} //�}�C�L�����X�V�̖߂�l��1�Ȃ�Q�[���I�[�o�[

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
	SetDrawArea(GAME_X,GAME_Y,GAME_X+GAME_WIDTH,GAME_Y+GAME_HEIGHT);//�`��G���A���Q�[���͈͂ɐݒ�

	back_g->BackDraw(d_count);	//��w�i�̕`��
	ene_mgr->OnTheDraw(count,mychar);	//�ڒn�^�G�l�~�[�̕`��
	if(boss != NULL) boss->Draw();	//�{�X�̕`��
	back_g->FrontDraw(d_count);	//�O�w�i�̕`��
	item_mgr->Draw();	//�A�C�e���̕`��
	mychar->Draw(d_count);	//�}�C�L�����̕`��
	ene_mgr->Draw(count,mychar);	//�ʏ�G�l�~�[�̕`��
	sht_mgr->Draw();	//�V���b�g�̕`��

	SetDrawArea(0,0,WINDOWSIZE_X,WINDOWSIZE_Y);	//�`��G���A�����ɖ߂�

#ifdef DEBUG_CODE
	DrawFormatString(140,0,GetColor(255,255,255),"�J�E���g:%d",count);
#endif
}

void Stage::DrawBackGround(int count){back_g->BackDraw(count);}