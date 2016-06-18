#define _CRT_SECURE_NO_WARNINGS

#include <DxLib.h>
#include <math.h>
#include "main.h"
#include "menu.h"
#include "database.h"
#include "input.h"
#include "image.h"
#include "sound.h"

Menu::Menu(int ax, int ay, char** a_item, int a_num, bool a_loop, bool a_box, bool a_cancel)
{
	px = ax;
	py = ay;

	index = 0;	//現在インデックス初期化
	move_count = 0;
	stat = MENUST_START;
	count = 0;
	result = -1;

	loop = a_loop;
	box = a_box;
	cancel = a_cancel;
	num = a_num;

	item = (char**)malloc(sizeof(char*)*num);	//文字列の配列を確保

	int max_len = 0;	//最大文字数を０と仮定
	for(int i = 0; i < num; i++){
		int str_len = strlen(a_item[i]);	//文字数を調べる

		if(max_len < str_len) max_len = str_len;	//最大文字数よりも多ければそれが最大文字数
		item[i] = (char*)malloc(sizeof(char)*(str_len+1));	//文字配列を確保

		strcpy(item[i], a_item[i]);	//文字列を確保
	}
}

Menu::~Menu()
{
	for(int i = 0; i < num; i++){
		delete item[i];	//まず、各要素を破棄してから
	}
	delete item;	//配列を破棄
}



int Menu::Update()
{
	switch(stat){
	case MENUST_START: 
		if(StartUpdate() == 1) stat = MENUST_MAIN; 
		break;

	case MENUST_MAIN: 
		result = MainUpdate();	//メイン更新の戻り値を記憶
		if(result == -2 && !cancel) result = -1;
			//それが-2の場合、キャンセル不可なら-1に

		if(result != -1){	//最終的な結果が-1でなければ
			stat = MENUST_END;	//エンド状態へ
		}
		break;
	case MENUST_END: 
		if(EndUpdate() == 1){
			return result;
		}
		break;
	}

	return -1;
}

void Menu::Draw()
{
	switch(stat){
	case MENUST_START: 
		StartDraw();
		break;

	case MENUST_MAIN:
		MainDraw();
		break;

	case MENUST_END: 
		EndDraw();
		break;
	}
}



int Menu::StartUpdate()
{
	count++;

	if(count == 10) {
		count = 0;
		return 1;
	}
	return 0;
}

#define MENUST_HEIGHT 25
#define MENUST_X 30
void Menu::StartDraw()
{
	int handle = font_db.QueryFontData(FONT_MENU);

	SetDrawBlendMode( DX_BLENDMODE_ALPHA, (int)(255*((double)count/10)));
	for(int i = 0; i < num; i++){
		DrawStringToHandle(px+MENUST_X+(10-count)*2, py+MENUST_HEIGHT*i, item[i], GetColor(255,255,255), handle);
	}
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0);
}



#define MOVE_INTERVAL 3
int Menu::MainUpdate()
{
	if(move_count == 0){
		if(input.GetInput( INPUT_DOWN ) == 1){
			index++;
			move_count = MOVE_INTERVAL;

			if(index >= num){
				if(loop) index = 0;
				else index = num-1;

				move_count = 0;
			} else sound.PlaySoundHandle(SOUNDEFECT_MENUCURSOR);
		}
		if(input.GetInput( INPUT_UP ) == 1){
			index--;
			move_count = -MOVE_INTERVAL;

			if(index < 0){
				if(loop) index = num-1;
				else index = 0;

				move_count = 0;
			} else sound.PlaySoundHandle(SOUNDEFECT_MENUCURSOR);
		}

		if(input.GetInput( INPUT_SHOT ) == 1){
			sound.PlaySoundHandle(SOUNDEFECT_MENUDECIDE);
			return index;
		}
		if(input.GetInput( INPUT_SLOW ) == 1){
			if(cancel) sound.PlaySoundHandle(SOUNDEFECT_MENUCANCEL);
			return -2;
		}
	}

	if(move_count > 0) move_count--;
	if(move_count < 0) move_count++;

	return -1;
}

void Menu::MainDraw()
{
	double move_y = (MENUST_HEIGHT*sin(((double)move_count/MOVE_INTERVAL)*PI/2));
	int handle = font_db.QueryFontData(FONT_MENU);

	for(int i = 0; i < num; i++){
		DrawStringToHandle(px+MENUST_X, py+MENUST_HEIGHT*i, item[i], GetColor(255,255,255), handle);
	}


	int str_w = GetDrawStringWidthToHandle( item[index], strlen(item[index]), handle);

	for(int i = 0; i < str_w; i++){
		image.DrawImage(px+MENUST_X+i, py+MENUST_HEIGHT*index-(int)move_y, IMG_MENUBACK, TRUE, 0, FALSE);
	}


	int draw_cx = px;
	int draw_cy = py+MENUST_HEIGHT*index-5;

	draw_cy -= (int)move_y;

	image.DrawImage(draw_cx, draw_cy, IMG_CURSOR, TRUE, 0, FALSE);
}



int Menu::EndUpdate()
{
	count++;

	if(count == 20) return 1;
	return 0;
}

void Menu::EndDraw()
{
	int handle = font_db.QueryFontData(FONT_MENU);

	if(result == -2) SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255-(int)(255*((double)count/20)));
	for(int i = 0; i < num; i++){
		if(count%2 == 0 || i != result)
			DrawStringToHandle(px+MENUST_X, py+MENUST_HEIGHT*i, item[i], GetColor(255,255,255), handle);
	}
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0);

	if(result != -2){
		int str_w = GetDrawStringWidthToHandle( item[result], strlen(item[result]), handle);

		int draw_cx = px;
		int draw_cy = py+MENUST_HEIGHT*result-5;

		if(count < 15) draw_cx += (int)(sin(PI/2 * ((double)count/15))*(str_w+30));
		else draw_cx += str_w+30;

		image.DrawImage(draw_cx, draw_cy, IMG_CURSOR, TRUE, 0, FALSE);
	}
}