#include <DxLib.h>
#include "database.h"
#include "background.h"
#include "main.h"
#include "image.h"

BackGround::BackGround(int fimg_index, int bimg_index, int f_velo, int b_velo)
{
	backimg_index = bimg_index;
	frontimg_index = fimg_index;
	b_velocity = b_velo;	//スクロール速度を記憶
	f_velocity = f_velo;

	image.GetImageSize(backimg_index, NULL, &bimg_height);	//画像の縦サイズを読み込む
	image.GetImageSize(frontimg_index, NULL, &fimg_height);	//画像の縦サイズを読み込む
}

void BackGround::BackDraw(int count)
{
	//同じ画像を２つ重ねて描画
	image.DrawImage(0,(count%(bimg_height/b_velocity))*b_velocity-bimg_height,backimg_index,FALSE,0);
	image.DrawImage(0,(count%(bimg_height/b_velocity))*b_velocity,backimg_index,FALSE,0);
	//スクロール速度にあわせてループ移動
}

void BackGround::FrontDraw(int count)
{
	image.DrawImage(0,(count%(fimg_height/f_velocity))*f_velocity-fimg_height,frontimg_index,TRUE,0);
	image.DrawImage(0,(count%(fimg_height/f_velocity))*f_velocity,frontimg_index,TRUE,0);
}

