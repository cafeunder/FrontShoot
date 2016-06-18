#include <DxLib.h>
#include "database.h"
#include "background.h"
#include "main.h"
#include "image.h"

BackGround::BackGround(int fimg_index, int bimg_index, int f_velo, int b_velo)
{
	backimg_index = bimg_index;
	frontimg_index = fimg_index;
	b_velocity = b_velo;	//�X�N���[�����x���L��
	f_velocity = f_velo;

	image.GetImageSize(backimg_index, NULL, &bimg_height);	//�摜�̏c�T�C�Y��ǂݍ���
	image.GetImageSize(frontimg_index, NULL, &fimg_height);	//�摜�̏c�T�C�Y��ǂݍ���
}

void BackGround::BackDraw(int count)
{
	//�����摜���Q�d�˂ĕ`��
	image.DrawImage(0,(count%(bimg_height/b_velocity))*b_velocity-bimg_height,backimg_index,FALSE,0);
	image.DrawImage(0,(count%(bimg_height/b_velocity))*b_velocity,backimg_index,FALSE,0);
	//�X�N���[�����x�ɂ��킹�ă��[�v�ړ�
}

void BackGround::FrontDraw(int count)
{
	image.DrawImage(0,(count%(fimg_height/f_velocity))*f_velocity-fimg_height,frontimg_index,TRUE,0);
	image.DrawImage(0,(count%(fimg_height/f_velocity))*f_velocity,frontimg_index,TRUE,0);
}

