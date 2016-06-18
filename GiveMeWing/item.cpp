#include <DxLib.h>
#include <math.h>
#include "main.h"
#include "mychar.h"
#include "item.h"
#include "database.h"
#include "satellite.h"
#include "input.h"
#include "image.h"
#include "sound.h"

/*
�N���X�� : Item
	�G�l�~�[�����Ƃ��p���[�A�b�v�A�C�e��

	�p�� : Character

	�L�q�t�@�C�� : item.h
*/

Item::Item(double a_x, double a_y, int a_w, int a_h, int a_life, int a_imgnum, int a_value):Character(a_x, a_y, a_w, a_h, a_life, a_imgnum)
{
	value = a_value;
	count = 0;
}


int Item::Update(MyCharacter* mychar)
{
	py+=2.0;	//��ɗ���

	//�}�C�L�����Ƃ̋��������Ƃ߂�
	double def_x = px - mychar->GetPosX();
	double def_y = py - mychar->GetPosY();

	double r = sqrt(def_x*def_x + def_y*def_y);
	//

	double jud_rad = 180;
	if(input.GetInput( INPUT_SHOT ) > 0) jud_rad = 80;

	if(r < jud_rad && count > 10){	//�}�C�L�����Ƃ̋������߂���΁A
		//�}�C�L�����߂����Ĉړ�
		double ang = atan2(def_y, def_x);

		px -= 7*cos(ang);
		py -= 7*sin(ang);
		//
	}

	if(mychar->Collition(px, width, py, height)){
			//�}�C�L�������A�C�e���ɐG��Ă����
		mychar->AddShotExp(value);
		sound.PlaySoundHandle(SOUNDEFECT_ITEMGAIN);

		return 1;	//�j����v��
	}

	if((px+width < 0) || (px > GAME_WIDTH) || (py > GAME_HEIGHT)){
		//��ʊO�ɏo�Ă���΁A
		return 1;	//�j����v��
	}

	count++;
	return 0;
}

void Item::Draw()
{
	//�������`���A�摜�̒��S�ɗ���悤�ɕ␳
	float draw_x = (float)px - (img_w/2 - width/2);
	float draw_y = (float)py - (img_h/2 - height/2);
	//

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, count*26);
	image.DrawImageF(draw_x, draw_y, img_index, TRUE, img_div);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 26);

#ifdef DEBUG_CODE
	_DrawBox(int(px), int(py), int(px)+width, int(py)+height, GetColor(0,0,255),FALSE);
	//�������`�͈͂̕\��
#endif
}


/*
�N���X�� : ItemMgr
	�A�C�e���̊Ǘ��֐�

	�L�q�t�@�C�� : item.h
*/

ItemMgr::ItemMgr()
{
	for(int i = 0; i < EXISTITEM_MAX; i++){
		//�A�C�e���͗\�񂵂Ă��Ȃ��Ƃ���NULL�Ȃ̂ŁA
		item[i] = NULL;
		//NULL�ŏ�����
	}
}

ItemMgr::~ItemMgr()
{
	for(int i = 0; i < EXISTITEM_MAX; i++){
		if(item[i] == NULL) continue;	//�\�񂳂�Ă��Ȃ��Ȃ炻�̂܂�
		delete item[i];	//����Ă�����A�j��
		//�Ȃ��A����̓f�X�g���N�^�Ȃ̂ŁANULL���l�߂�K�v�͂Ȃ�
	}
}

void ItemMgr::Update(MyCharacter *mychar)
{
	for(int i = 0; i < EXISTITEM_MAX; i++){
		if(item[i] == NULL) continue;	//�\�񂳂�Ă��Ȃ��Ȃ�X�V���Ȃ�
		if(item[i]->Update(mychar) == 1) {	//�\�񂳂�Ă�����X�V
			delete item[i];	//1���A���Ă�����j��
			item[i] = NULL;	//NULL���߂�
		}
	}
}

void ItemMgr::Draw()
{
	for(int i = 0; i < EXISTITEM_MAX; i++){
		if(item[i] == NULL) continue;	//�\�񂳂�Ă��Ȃ��Ȃ�`�悵�Ȃ�
		item[i]->Draw();	//����Ă�����A�`��
	}
}

void ItemMgr::SetItem(double a_x, double a_y, int a_value)
{
	for(int i = 0; i < a_value; i++){
		for(int i = 0; i < EXISTITEM_MAX; i++){
			if(item[i] != NULL) continue;	//�����\�񂳂�āu����v�Ȃ�A�\�񂵂Ȃ�

			int rand_x = GetRand(60)-30;
			int rand_y = GetRand(60)-30;

			item[i] = new Item(a_x+rand_x, a_y+rand_y, 15, 15, 0, IMG_ITEM, 1);	//����Ă��Ȃ��Ȃ�\��
			break;
		}
	}
	//���ׂĒT���Ă��\��ł��Ȃ���΂�����߂�
}