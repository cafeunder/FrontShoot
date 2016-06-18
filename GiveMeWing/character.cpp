#include <DxLib.h>
#include "character.h"
#include "main.h"
#include "database.h"
#include "image.h"

/*
クラス名 : Character
	あらゆる物体を表す基本クラス

	サブクラス : MyCharacter Enemy

	記述ファイル : character.h
*/

Character::Character(double a_x, double a_y, int a_w, int a_h, int a_life, int a_imgindex)
{
	px = a_x;
	py = a_y;
	width = a_w;
	height = a_h;
	life = a_life;

	img_index = a_imgindex;	//データベースよりイメージハンドルを受け取る

	image.GetImageSize(img_index, &img_w, &img_h);	//グラフサイズの取得
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
	//当たり矩形が、画像の中心に来るように補正
	float draw_x = (float)px - (img_w/2 - width/2);
	float draw_y = (float)py - (img_h/2 - height/2);
	//

	image.DrawImageF(draw_x, draw_y, img_index, TRUE, img_div);

#ifdef DEBUG_CODE
	_DrawBox(int(px), int(py), int(px)+width, int(py)+height, GetColor(0,0,255),FALSE);
	//あたり矩形範囲の表示
#endif

}

bool Character::Collition(double a_x0, int a_w, double a_y0, int a_h) //当たり判定
{
	double a_x1 = a_x0+a_w;	//物体aの右下x
	double a_y1 = a_y0+a_h; //aの右下y
	double b_x0 = px;		//物体b(=this)の左上x
	double b_x1 = px+width;	//bの右下y
	double b_y0 = py;		//bの左上y
	double b_y1 = py+height;//bの右下y

	if((a_x0 < b_x1) && (b_x0 < a_x1) && (a_y0 < b_y1) && (b_y0 < a_y1)){	//当たり判定式
		return TRUE;	//当たっていたらTRUEを返す
	}

	return FALSE;
}

