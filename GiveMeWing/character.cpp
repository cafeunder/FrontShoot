#include <DxLib.h>
#include "character.h"
#include "main.h"
#include "database.h"
#include "image.h"

/*
�N���X�� : Character
	�����镨�̂�\����{�N���X

	�T�u�N���X : MyCharacter Enemy

	�L�q�t�@�C�� : character.h
*/

Character::Character(double a_x, double a_y, int a_w, int a_h, int a_life, int a_imgindex)
{
	px = a_x;
	py = a_y;
	width = a_w;
	height = a_h;
	life = a_life;

	img_index = a_imgindex;	//�f�[�^�x�[�X���C���[�W�n���h�����󂯎��

	image.GetImageSize(img_index, &img_w, &img_h);	//�O���t�T�C�Y�̎擾
	img_w/=img_db.QueryImgData(img_index)->div_num;
	img_div = 0;
}

bool Character::Damage(int damage)
{
	life -= damage;

	if(life <= 0) return TRUE;
	return FALSE;
}

void Character::Draw()
{
	//�������`���A�摜�̒��S�ɗ���悤�ɕ␳
	float draw_x = (float)px - (img_w/2 - width/2);
	float draw_y = (float)py - (img_h/2 - height/2);
	//

	image.DrawImageF(draw_x, draw_y, img_index, TRUE, img_div);

#ifdef DEBUG_CODE
	_DrawBox(int(px), int(py), int(px)+width, int(py)+height, GetColor(0,0,255),FALSE);
	//�������`�͈͂̕\��
#endif

}

bool Character::Collition(double a_x0, int a_w, double a_y0, int a_h) //�����蔻��
{
	double a_x1 = a_x0+a_w;	//����a�̉E��x
	double a_y1 = a_y0+a_h; //a�̉E��y
	double b_x0 = px;		//����b(=this)�̍���x
	double b_x1 = px+width;	//b�̉E��y
	double b_y0 = py;		//b�̍���y
	double b_y1 = py+height;//b�̉E��y

	if((a_x0 < b_x1) && (b_x0 < a_x1) && (a_y0 < b_y1) && (b_y0 < a_y1)){	//�����蔻�莮
		return TRUE;	//�������Ă�����TRUE��Ԃ�
	}

	return FALSE;
}

