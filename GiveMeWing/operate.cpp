#define _CRT_SECURE_NO_WARNINGS

#include <DxLib.h>
#include <math.h>
#include "operate.h"
#include "database.h"
#include "image.h"
#include "main.h"
#include "sound.h"


#define OPERATEAREA_X 400	//オペレーターエリアを表示するx座標
#define OPERATEAREA_Y 174	//y座標
#define OPERATEAREA_W (WINDOWSIZE_X-OPERATEAREA_X)	//オペレートエリアの幅
#define OPERATEAREA_H (WINDOWSIZE_Y-OPERATEAREA_Y)	//オペレートエリアの高さ
#define STANDBY_COUNT 35



void OperateArea::Update()
{
	switch(state){
	case OPEAREA_SHUTDOWN : break;	//シャットダウン状態ならなにもしない

	case OPEAREA_STANDBY :	//スタンバイ状態なら何もしないが、
		if(count == STANDBY_COUNT){	//スタンバイ必要カウントが経過したら、
			count = 0;	//カウントを初期化して、
			state = OPEAREA_STANDING;	//起動状態に
		}
		break;

	case OPEAREA_STANDING:	//起動状態なら
		switch(mesbox.Update()){//メッセージボックスの更新を行い、
		case 0 : draw_mes = TRUE; break;	//0が帰ってきたらメッセージ表示中
		case 1 :	//1が帰ってきたら次のメッセージへ
			for(int i = 0; i < STRING_MAXNUM-1; i++){
				operate_type[i] = operate_type[i+1];	//オペレー子表示タイプを１つシーク
			}
			operate_type[STRING_MAXNUM-1] = -1;
			break;
		case -1 : draw_mes = FALSE;	break;
		break;
		}
	}

	count++;	//カウントを進める
}

void OperateArea::Draw()
{
	switch(state){
	case OPEAREA_SHUTDOWN : break;	//シャットダウン状態なら何もしない
	case OPEAREA_STANDBY :	//スタンバイ状態なら、
		DrawBox(OPERATEAREA_X, OPERATEAREA_Y, OPERATEAREA_X+OPERATEAREA_W, WINDOWSIZE_Y+OPERATEAREA_H, GetColor(10,10,10),TRUE);
		//ディスプレイの電源をつけたときっぽい演出
		if(count >= 15 && count < 25){
			int center_x = OPERATEAREA_X + OPERATEAREA_W/2;	//中心x
			int w_toc = (int)(((double)(count-15)/15)*(OPERATEAREA_W/2));//中心からの幅

			DrawLine(center_x-w_toc, OPERATEAREA_Y+OPERATEAREA_H/2, center_x+w_toc, OPERATEAREA_Y+OPERATEAREA_H/2, GetColor(120,120,120), 5);
				//ディスプレイをつけたときの線を描画
		} else if(count > 30){
			int center_y = OPERATEAREA_Y + OPERATEAREA_H/2;	//中心y
			int h_toc = (int)(sin((PI/2)*(((double)count-30)/5))*(OPERATEAREA_H/2)); //中心からの高さ

			DrawBox(OPERATEAREA_X, center_y-h_toc, OPERATEAREA_X+OPERATEAREA_W, center_y+h_toc, GetColor(0,20,0), TRUE);
				//ディスプレイの画像が展開する演出
		}
		//

		break;
	case OPEAREA_STANDING :;

		DrawBox(OPERATEAREA_X, OPERATEAREA_Y, OPERATEAREA_X+OPERATEAREA_W, WINDOWSIZE_Y+OPERATEAREA_H, GetColor(0,20,0),TRUE);
			//オペレータエリア背景の描画

		if(operate_type[0] != -1){
			image.DrawImage(OPERATEAREA_X,OPERATEAREA_Y,IMG_OPE1,FALSE,operate_type[0],FALSE);
			//オペレー子の描画
		}

		if(draw_mes){//モジュールを描画するなら、
			mesbox.Draw();	//メッセージボックスの描画
		}

		/*
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, 63);
		//tag
		DrawBox(OPERATEAREA_X, OPERATEAREA_Y+(count*2)%OPERATEAREA_H, OPERATEAREA_X+OPERATEAREA_W, OPERATEAREA_Y+(count*2)%OPERATEAREA_H+5, GetColor(60,120,60),TRUE);
		DrawBox(OPERATEAREA_X, OPERATEAREA_Y+(count*2)%OPERATEAREA_H+7, OPERATEAREA_X+OPERATEAREA_W, OPERATEAREA_Y+(count*2)%OPERATEAREA_H+10, GetColor(60,120,60),TRUE);

		DrawBox(OPERATEAREA_X, OPERATEAREA_Y+(count*2)%OPERATEAREA_H+15, OPERATEAREA_X+OPERATEAREA_W, OPERATEAREA_Y+(count*2)%OPERATEAREA_H+15+2, GetColor(60,120,60),TRUE);
		DrawBox(OPERATEAREA_X, OPERATEAREA_Y+(count*2)%OPERATEAREA_H+19, OPERATEAREA_X+OPERATEAREA_W, OPERATEAREA_Y+(count*2)%OPERATEAREA_H+19+1, GetColor(60,120,60),TRUE);
		//砂嵐の画像を作ってそれを表示
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 126);
		*/
	}
}

void OperateArea::SetMessage(const char* str, int operater_type, int str_brake)
{
	if(state == OPEAREA_SHUTDOWN) return;	//起動していないならメッセージを予約しない
	//sound.PlaySoundHandle(SOUNDEFECT_OPESTART);

	mesbox.SetMessage(str, str_brake);

	for(int i = 0; i < STRING_MAXNUM; i++){
		if(operate_type[i] != -1) continue;
		operate_type[i] = operater_type;
		return;
	}
}

void OperateArea::Reset()
{
	draw_mes = FALSE;
	for(int i = 0; i < STRING_MAXNUM; i++){
		operate_type[i] = -1;
	}

	mesbox.Reset();
}



bool JudgeTwoByteChar(char c)
{
    if(((BYTE)0x81<=(BYTE)c && (BYTE)c<=(BYTE)0x9F) ||
        ((BYTE)0xE0<=(BYTE)c && (BYTE)c<=(BYTE)0xFF)) return TRUE;
	else return FALSE;
}


MesBox::MesBox()
{
	for(int i = 0; i < STRING_MAXNUM; i++){
		string[i] = NULL;
	}

	draw_mes[0]= '\0';
	brake_count = -1;
	interval_count = 0;
	index = 0;
}

MesBox::~MesBox()
{
	for(int i = 0; i < STRING_MAXNUM; i++){
		delete string[i];
	}
}

#define MESSAGE_BRAKECOUNT 180	//デフォルトの描画終了後待機時間
int MesBox::SetMessage(const char* str, int str_brake)
{
	for(int i = 0; i < STRING_MAXNUM; i++){
		if(string[i] != NULL) continue;	//すでにメッセージが予約されているなら、予約しない
		
		int length = strlen(str);	//メッセージの長さをしらべる(終端文字はカウントしていない!

		string[i] = (char*)malloc(sizeof(char)*(length+1));//メモリの確保（終端文字分も確保する
		
		strcpy(string[i], str);	//文字列のコピー

		//描画終了後待機時間をセット
		if(str_brake == -1){	//指定が-1なら
			string_brake[i] = MESSAGE_BRAKECOUNT;	//デフォルトの待機時間をセット
		} else string_brake[i] = str_brake;

		return 0;
	}

	return 1;
}

#define MESSAGE_INTERVAL 3	//文字を表示する時間間隔
int MesBox::StrUpdate()
{
	if(interval_count > 0){interval_count--; return 0;}
		//インターバル中なら、描画文字列を更新しない

	char c = string[0][index];	//最古のメッセージの、現在インデックスの文字を確保

	draw_mes[index] = c;	//描画文字列の現在インデックスに代入

	if(c == '\0') return 1;	//もし、それが終端文字なら、描画終了
	if(JudgeTwoByteChar(c)){	//もし、それが2byte文字なら
		index++;	//インデックスをひとつ進め、
		draw_mes[index] = string[0][index];	//もうひとつ文字をセット
	}
	draw_mes[index+1] = '\0';	//現在文字+1番目に終端文字をセット
	
	//そうでなければ
	index++; //現在インデックスを進め、
	interval_count = MESSAGE_INTERVAL;//インターバルカウントをセット

	sound.PlaySoundHandle(SOUNDEFECT_OPETYPE);//タイプ音

	return 0;
}

int MesBox::Update()
{
	int result = 0;
	if(brake_count == 0) {draw_mes[0] = '\0'; result = 1;}	//描画後待機終了なら、描画文字列を初期化
	if(brake_count > -1) {brake_count--; return result;}

	if(string[0] == NULL) return -1;	//描画すべきメッセージがひとつもないなら、何もしない

	if(StrUpdate() == 1){	//描画文字列更新
		//戻り値として1が帰ってきたら、描画終了
	
		brake_count = string_brake[0];	//描画終了後待機カウントをセット
		index = 0;	//現在インデックスを初期化

		delete string[0];	//もっとも古くセットされたメッセージを削除
		string[0] = NULL;	//NULLをつめる

		for(int i = 0; i < STRING_MAXNUM-1; i++){
			string[i] = string[i+1];	//メッセージ配列をひとつシーク
			string_brake[i] = string_brake[i+1];
		}
		string[STRING_MAXNUM-1] = NULL;	//最後の要素にNULLをつめる
		string_brake[STRING_MAXNUM-1] = 0;
	}

	return 0;
}

#define MESBOX_EDGESIZE 4	//メッセージボックスの枠サイズ
#define MESBOX_X OPERATEAREA_X		//メッセージボックスを描画するx座標
#define MESBOX_Y 371	//メッセージボックスを描画するy座標
#define MESBOX_W 240	//メッセージボックスの幅
#define MESBOX_H 110	//メッセージボックスの高さ

#define MESLINE_HEIGHT 25 //一行に要する高さ
#define MES_X 5	//メッセージボックスの左上を原点としたメッセージ描画x座標
#define MES_Y 5 //y座標
void MesBox::Draw()
{
	//メッセージボックスの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(MESBOX_X,MESBOX_Y,MESBOX_X+MESBOX_W,MESBOX_Y+MESBOX_H,GetColor(150,200,150),TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 126);
	for(int i = 0; i < MESBOX_EDGESIZE; i++){
		DrawBox(MESBOX_X+i,MESBOX_Y+i,MESBOX_X+MESBOX_W-i,MESBOX_Y+MESBOX_H-i,GetColor(80,80,80),FALSE);
	}
	//

	int mes_fonthan = font_db.QueryFontData(FONT_MESSAGE);

	int line = 0;		//現在行

	int line_index = 0;	//現在行の文字インデックス
	int draw_index = 0;	//描画文字列の文字インデックス

	int color = GetColor(10,10,10);

	char line_mes[MESLINE_MAXNUM+1];	//一行に表示できる文字数+1(終端文字

	while(line < MESBOX_LINEMAX && draw_index < MESBOX_MAXNUM){
		char c = draw_mes[draw_index];	//描画文字列の、現在インデックスの文字を確保

		line_mes[line_index] = c;	//現在行の現在インデックスに代入
		if(c == '\0'){	//もし、それが終端文字なら
			DrawStringToHandle(MESBOX_X+MES_X,MESBOX_Y+line*MESLINE_HEIGHT+MES_Y,line_mes,color,mes_fonthan);
				//文字列を描画して
			return;	//描画処理終了
		}
		if(c == '\n'){
			//もし、それが改行文字なら
			
			line_mes[line_index] = '\0';	//終端文字を挿入し
			DrawStringToHandle(MESBOX_X+MES_X,MESBOX_Y+line*MESLINE_HEIGHT+MES_Y,line_mes,color,mes_fonthan);
				//文字列を描画して

			line_index = 0;	//現在行の現在インデックスを先頭に戻し、
			line_mes[0] = '\0';	//現在行の文字列を初期化
			line++;	//次の行へ
		} else if(line_index == MESLINE_MAXNUM-1){
			//現在インデックスが行文字列末尾の直前ならなら、
			//行文字列の最後は必ず終端文字で使わなければいけないので

			line_mes[line_index+1] = '\0';
			DrawStringToHandle(MESBOX_X+MES_X,MESBOX_Y+line*MESLINE_HEIGHT+MES_Y,line_mes,color,mes_fonthan);
				//文字列を描画して

			line_index = 0;	//現在行の現在インデックスを先頭に戻し、
			line_mes[0] = '\0';	//現在行の文字列を初期化
			line++;	//次の行へ
		} else line_index++;	//改行でないなら、次へ

		draw_index++;
	}

	/*
	正常なメッセージ描画が行われた場合、
	この関数は必ず終端文字検出によるreturn文によって終了するため、
	whileが終了したということは不正が生じたと言うこと、
	*/

	//ErrorExit("不正な描画メッセージ");	//なので、エラー終了
}

void MesBox::Reset()
{
	for(int i = 0; i < STRING_MAXNUM; i++){
		string[i] = NULL;
	}

	draw_mes[0]= '\0';
	brake_count = -1;
	interval_count = 0;
	index = 0;
}
