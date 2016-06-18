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
クラス名 : Item
	エネミーが落とすパワーアップアイテム

	継承 : Character

	記述ファイル : item.h
*/

Item::Item(double a_x, double a_y, int a_w, int a_h, int a_life, int a_imgnum, int a_value):Character(a_x, a_y, a_w, a_h, a_life, a_imgnum)
{
	value = a_value;
	count = 0;
}


int Item::Update(MyCharacter* mychar)
{
	py+=2.0;	//常に落下

	//マイキャラとの距離をもとめる
	double def_x = px - mychar->GetPosX();
	double def_y = py - mychar->GetPosY();

	double r = sqrt(def_x*def_x + def_y*def_y);
	//

	double jud_rad = 180;
	if(input.GetInput( INPUT_SHOT ) > 0) jud_rad = 80;

	if(r < jud_rad && count > 10){	//マイキャラとの距離が近ければ、
		//マイキャラめがけて移動
		double ang = atan2(def_y, def_x);

		px -= 7*cos(ang);
		py -= 7*sin(ang);
		//
	}

	if(mychar->Collition(px, width, py, height)){
			//マイキャラがアイテムに触れていれば
		mychar->AddShotExp(value);
		sound.PlaySoundHandle(SOUNDEFECT_ITEMGAIN);

		return 1;	//破棄を要求
	}

	if((px+width < 0) || (px > GAME_WIDTH) || (py > GAME_HEIGHT)){
		//画面外に出ていれば、
		return 1;	//破棄を要求
	}

	count++;
	return 0;
}

void Item::Draw()
{
	//当たり矩形が、画像の中心に来るように補正
	float draw_x = (float)px - (img_w/2 - width/2);
	float draw_y = (float)py - (img_h/2 - height/2);
	//

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, count*26);
	image.DrawImageF(draw_x, draw_y, img_index, TRUE, img_div);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 26);

#ifdef DEBUG_CODE
	_DrawBox(int(px), int(py), int(px)+width, int(py)+height, GetColor(0,0,255),FALSE);
	//あたり矩形範囲の表示
#endif
}


/*
クラス名 : ItemMgr
	アイテムの管理関数

	記述ファイル : item.h
*/

ItemMgr::ItemMgr()
{
	for(int i = 0; i < EXISTITEM_MAX; i++){
		//アイテムは予約していないときはNULLなので、
		item[i] = NULL;
		//NULLで初期化
	}
}

ItemMgr::~ItemMgr()
{
	for(int i = 0; i < EXISTITEM_MAX; i++){
		if(item[i] == NULL) continue;	//予約されていないならそのまま
		delete item[i];	//されていたら、破棄
		//なお、これはデストラクタなので、NULLを詰める必要はない
	}
}

void ItemMgr::Update(MyCharacter *mychar)
{
	for(int i = 0; i < EXISTITEM_MAX; i++){
		if(item[i] == NULL) continue;	//予約されていないなら更新しない
		if(item[i]->Update(mychar) == 1) {	//予約されていたら更新
			delete item[i];	//1が帰ってきたら破棄
			item[i] = NULL;	//NULLをつめる
		}
	}
}

void ItemMgr::Draw()
{
	for(int i = 0; i < EXISTITEM_MAX; i++){
		if(item[i] == NULL) continue;	//予約されていないなら描画しない
		item[i]->Draw();	//されていたら、描画
	}
}

void ItemMgr::SetItem(double a_x, double a_y, int a_value)
{
	for(int i = 0; i < a_value; i++){
		for(int i = 0; i < EXISTITEM_MAX; i++){
			if(item[i] != NULL) continue;	//もう予約されて「いる」なら、予約しない

			int rand_x = GetRand(60)-30;
			int rand_y = GetRand(60)-30;

			item[i] = new Item(a_x+rand_x, a_y+rand_y, 15, 15, 0, IMG_ITEM, 1);	//されていないなら予約
			break;
		}
	}
	//すべて探しても予約できなければあきらめる
}