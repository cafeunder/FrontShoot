#include <DxLib.h>
#include <math.h>
#include "efect.h"
#include "main.h"
#include "database.h"
#include "image.h"
#include "sound.h"

EfectMgr efect;

int EfectOpe_SmallBurn(Efect* efe)
{
	double px = efe->GetPosX();
	double py = efe->GetPosY();
	int count = efe->GetCount();
	int option1 = efe->GetOption1();
	int option2 = efe->GetOption2();

	int m_blend = 0;

	double r = sin(PI/2 * ((double)count/30))*option1;

	if(count > 5){
		m_blend = count*10;
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255-m_blend);


	SetDrawMode(DX_DRAWMODE_BILINEAR);
	for(int i = 0; i < 5; i++){
		DrawCircle((int)px, (int)py, (int)r-i, option2, FALSE);
	}
	SetDrawMode(DX_DRAWMODE_NEAREST);

	DrawCircle((int)px, (int)py, (int)r, option2,TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255-count*5);	

	if(count > 30) return 1;
	return 0;
}

int EfectOpe_SmallBurnFast(Efect* efe)
{
	double px = efe->GetPosX();
	double py = efe->GetPosY();
	int count = efe->GetCount();
	int option1 = efe->GetOption1();
	int option2 = efe->GetOption2();

	int m_blend = 0;

	double r;
	if(count < 10){
		r = sin(PI/2 * ((double)count/10))*option1;
	}
	else r = option1;

	if(count > 5){
		m_blend = count*10;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255-m_blend);
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	DrawCircle((int)px, (int)py, (int)r, option2, FALSE);

	SetDrawMode(DX_DRAWMODE_NEAREST);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255-count*5);	

	if(count > 20) return 1;
	return 0;
}

int EfectOpe_SateBurn(Efect* efe)
{
	double px = efe->GetPosX();
	double py = efe->GetPosY();
	int count = efe->GetCount();

	int m_blend = 0;

	if(count > 5){
		m_blend = count*4;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255-m_blend);
	DrawBox(GAME_X, GAME_Y, GAME_X+GAME_WIDTH, GAME_Y+GAME_HEIGHT, GetColor(100,0,0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255-count*5);	


	if(count > 60) {SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0); return 1;}
	return 0;
}

int EfectOpe_GetScore(Efect* efe)
{
	double px = efe->GetPosX();
	double py = efe->GetPosY();
	int count = efe->GetCount();
	int option1 = efe->GetOption1();
	int m_blend = 0;

	if(count > 10){
		m_blend = count*10;
		py-=count-10;
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255-m_blend);
	DrawFormatString((int)px, (int)py, GetColor(0,0,0), "%d", option1);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255-m_blend);

	if(count > 30) return 1;
	return 0;
}

int EfectOpe_Bonus(Efect* efe)
{
	double px = efe->GetPosX();
	double py = efe->GetPosY();
	int count = efe->GetCount();
	double option3 = efe->GetOption3();
	int option2 = efe->GetOption2();
	int m_blend = 0;

	int color = GetColor(255,255,255);
	if(count%20 < 10){
		color = GetColor(255,255,0);
	}
	if(count > 40){
		px+=count-40;
		m_blend = (count-40)*10;
	}

	int font_han = font_db.QueryFontData(FONT_BONUS);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255-m_blend);
	DrawFormatStringToHandle((int)px, (int)py, color, font_han, "×%.1f", option3);
	DrawFormatStringToHandle((int)px+100, (int)py, color, font_han, "%d", option2);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255-m_blend);

	if(count > 70) return 1;
	return 0;
}

int EfectOpe_StrCallFast(Efect* efe)
{
	int px = 0;
	int py = GAME_HEIGHT/2;

	int count = efe->GetCount();
	int box_han = efe->GetOption1();
	int str_han = efe->GetOption2();

	int img_w; int img_h;
	GetGraphSize(box_han, &img_w, &img_h);
	double fimg_h = (double)img_h;

	if(count <= 10){
		fimg_h *= (double)count/10;
	}
	if(count >= 80){
		fimg_h *= (double)(10 - (count-80))/10;
	}

	DrawBox(GAME_X, GAME_Y, GAME_X+GAME_WIDTH, GAME_Y+GAME_HEIGHT, GetColor(0,0,0), TRUE);
	DrawModiGraph(px+GAME_X,py-(int)fimg_h/2, px+img_w+GAME_X,py-(int)fimg_h/2, px+img_w+GAME_X,py+(int)fimg_h/2, px+GAME_X,py+(int)fimg_h/2,box_han,TRUE);

	if(count > 10 && count < 80){
		int str_w; int str_h;

		GetGraphSize(str_han, &str_w, &str_h);
		int draw_x = px + (img_w-str_w)/2;
		int draw_y = py + (img_h-str_h)/2 - (int)fimg_h/2;
		
		DrawGraph(draw_x+GAME_X,draw_y, str_han, TRUE);
	}
	if(count == FASTCALL_COUNT) return 1;
	return 0;
}

int EfectOpe_StrCallSlow(Efect* efe)
{
	int px = 0;
	int py = GAME_HEIGHT/2;

	int box_han = efe->GetOption1();
	int str_han = efe->GetOption2();
	int count = efe->GetCount();
	
	int img_w; int img_h;
	GetGraphSize(box_han, &img_w, &img_h);
	double fimg_h = (double)img_h;

	if(count <= 20){
		fimg_h *= (double)count/20;
	}
	if(count >= 200){
		fimg_h *= (double)(10 - (count-200))/10;
	}

	DrawModiGraph(px+GAME_X,py-(int)fimg_h/2, px+img_w+GAME_X,py-(int)fimg_h/2, px+img_w+GAME_X,py+(int)fimg_h/2, px+GAME_X, py+(int)fimg_h/2,box_han,TRUE);

	if(count > 40 && count < 200){
		int str_w; int str_h;

		GetGraphSize(str_han, &str_w, &str_h);
		int draw_x = px + (img_w-str_w)/2;
		int draw_y = py + (img_h-str_h)/2 - (int)fimg_h/2;
		
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (count-40)*4);
		DrawGraph(draw_x+GAME_X,draw_y, str_han, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if(count == SLOWCALL_COUNT) return 1;
	return 0;
}

int EfectOpe_StrCallCaution(Efect* efe)
{
	int m_blend = 0;
	int count = efe->GetCount();

	if(count%100 == 0 && count/100 < 4){
		sound.PlaySoundHandle(SOUNDEFECT_CAUTION);
	}

	if(count/100 < 4){
		m_blend = (int)(sin(((double)(count%100))/100 * PI/2)*255);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255-m_blend);
		DrawBox(GAME_X, GAME_Y, GAME_X+GAME_WIDTH, GAME_Y+GAME_HEIGHT, GetColor(255,0,0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	int px = 0;
	int py = GAME_HEIGHT/2;

	int box_han = efe->GetOption1();
	
	int img_w; int img_h;
	GetGraphSize(box_han, &img_w, &img_h);
	double fimg_h = (double)img_h;

	if(count <= 20){
		fimg_h *= (double)count/20;
	}
	if(count >= 390){
		fimg_h *= (double)(10 - (count-390))/10;
	}

	DrawModiGraph(px+GAME_X,py-(int)fimg_h/2, px+img_w+GAME_X,py-(int)fimg_h/2, px+img_w+GAME_X,py+(int)fimg_h/2, px+GAME_X, py+(int)fimg_h/2,box_han,TRUE);


	int str_index = efe->GetOption2();
	if(count > 20 && count < 390){
		int str_w; int str_h;

		image.GetImageSize(str_index, &str_w, &str_h);
		str_w/=img_db.QueryImgData(str_index)->div_num;

		int draw_x = px + (img_w-str_w)/2;
		int draw_y = py + (img_h-str_h)/2 - (int)fimg_h/2;
		
		int div = 0;
		if(count%2 == 0) div = 1;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (count-20)*8);
		image.DrawImage(draw_x, draw_y, str_index, TRUE, div); 
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if(count > 400) {SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0); return 1;}
	return 0;
}

int EfectOpe_AlphaMotion(Efect* efe)
{
	float px = (float)efe->GetPosX();
	float py = (float)efe->GetPosY();
	int count = efe->GetCount();

	int img_index = efe->GetOption1();
	int max_count = efe->GetOption2();
	double img_ang = efe->GetOption3();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255-(int)(255*((double)count/max_count)));
	image.DrawRotaImageF(px, py, 1.0, img_ang, img_index, TRUE, 0);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if(count == max_count) return 1;
	return 0;
}

int EfectOpe_MycharSite(Efect* efe)
{
	int count = efe->GetCount();

	if(count == 0){
		sound.PlaySoundHandle(SOUNDEFECT_SITE);
	}

	if(count > 70){
		int div = 0;

		if(count%2 == 0) div = 1;
		image.DrawImageF((float)efe->GetPosX(), (float)efe->GetPosY(), IMG_SITE, TRUE, div);

	} else
	if(count < 3 || (count > 6 && count < 9) || (count > 20))
		image.DrawImageF((float)efe->GetPosX(), (float)efe->GetPosY(), IMG_SITE, TRUE, 0);


	if(count >= 100) return 1;
	return 0;
}

int EfectOpe_LevelUpDown(Efect* efe)
{
	int count = efe->GetCount();
	double px = efe->GetPosX();
	double py = efe->GetPosY();

	if(count < 5){
		py -= count*6;
	} else  py -= 30;

	int gr_index = efe->GetOption1();

	int img_w, img_h;
	image.GetImageSize(gr_index,&img_w,&img_h);
	img_w/=img_db.QueryImgData(gr_index)->div_num;

	if(count % 4 < 2){
		image.DrawImageF((float)px-img_w/2, (float)py, gr_index, TRUE, 0, TRUE);
	} else image.DrawImageF((float)px-img_w/2, (float)py, gr_index, TRUE, 1, TRUE);

	if(count >= 40) return 1;
	return 0;
}

/*
int EfectOpe_LaserEfect(Efect* efe)
{
	int count = efe->GetCount();
	float px = (float)efe->GetPosX();
	float py = (float)efe->GetPosY();

	int w = efe->GetOption1();
	int h = efe->GetOption2();

	px += (w/2 - 15);
	py += (h/2 - 15);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(255*((double)count/30)));
	image.DrawRotaImageF(px, py, (1-((double)count/30))*3, 0, IMG_GRDSATEBR, TRUE, 0, TRUE, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if(count == 30) return 1;
	return 0;
}
*/

int EfectOpe_Thunder(Efect* efe)
{
	//int img_handle = img_db.QueryImgData(IMG_THUNDER)->img_han;	//使うの固定
	double px = efe->GetPosX();
	double py = efe->GetPosY();
	int count = efe->GetCount();

	if(count%50 < 15){
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255-(int)(((double)(count%50)/15)*255));
		image.DrawImage((int)px,(int)py,IMG_THUNDER,TRUE,0,TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255-(int)(((double)(count%50)/20)*255));
	DrawBox(GAME_X,GAME_Y,GAME_X+GAME_WIDTH,GAME_Y+GAME_HEIGHT,GetColor(255,255,255),TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	if(count == 20) {sound.PlaySoundHandle(SOUNDEFECT_THUNDER);return 1;}
	return 0;
}

int EfectOpe_Lightning(Efect* efe)
{
	double px = efe->GetPosX();
	double py = efe->GetPosY();
	int count = efe->GetCount();

	if(count%10 < 3 || (count%10 > 7 && count%10 < 10)){
		DrawBox(GAME_X,GAME_Y,GAME_X+GAME_WIDTH,GAME_Y+GAME_HEIGHT,GetColor(255,255,255),TRUE);
	} 

	if(count == 10) return 1;
	return 0;
}

int EfectOpe_Assert(Efect* efe)
{
	int px = (int)efe->GetPosX();
	int py = (int)efe->GetPosY();
	int width = efe->GetOption1();
	int height = efe->GetOption2();
	int count = efe->GetCount();

	if(count == 0) sound.PlaySoundHandle(SOUNDEFECT_ENEASERT);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (count%30)*6);	//アルファブレンドで描画
	DrawBox(px, py, px+width, py+height, GetColor(255,0,0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);	//アルファブレンドで描画

	if(count == 60) return 1;
	return 0;
}

int EfectOpe_LaserEfect(Efect* efe)
{
	//int handle = LoadGraph("3dresa.png");
	float px = (float)efe->GetPosX();
	float py = (float)efe->GetPosY();
	float ang = (float)efe->GetOption3();
	int count = efe->GetCount();
	int blend = (int)(((double)count/30)*255);
	
	for(int i = 0; i < 15; i++){
		SetDrawBlendMode(DX_BLENDMODE_ADD, blend);	//アルファブレンドで描画
		image.DrawRotaImageF(px+48*i*cos(ang), 30+py+48*i*sin(ang), 1.0, ang+PI/2, IMG_ENEBIGLASER, TRUE, 0, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	if(count == 30) return 1;
	return 0;
}

int (*EfectOpe[EFECT_NUM])(Efect* efe) = 
{
	EfectOpe_SmallBurn,
	EfectOpe_SmallBurnFast,
	EfectOpe_SateBurn,
	EfectOpe_GetScore,
	EfectOpe_Bonus,
	EfectOpe_StrCallFast,
	EfectOpe_StrCallSlow,
	EfectOpe_AlphaMotion,
	EfectOpe_MycharSite,
	EfectOpe_StrCallCaution,
	EfectOpe_LevelUpDown,
	EfectOpe_LaserEfect,
	EfectOpe_Thunder,
	EfectOpe_Lightning,
	EfectOpe_Assert
};

int Efect::Update()
{
	int result = EfectOpe[type](this);
	count++;
	return result;
}

EfectMgr::~EfectMgr()
{
	for(int i = 0; i < EFECT_NUM; i++){
		if(efe[i] == NULL) continue;

		delete efe[i];
	}
}

EfectMgr::EfectMgr()
{
	for(int i = 0; i < EFECT_NUM; i++){
		efe[i] = NULL;
	}
}

void EfectMgr::Update()
{
	for(int i = 0; i < EFECT_NUM; i++){
		if(efe[i] == NULL) continue;

		if(efe[i]->Update() == 1){
			delete efe[i];
			efe[i] = NULL;
		}
	}
}

void EfectMgr::SetEfect(double px, double py, int type, int option1, int option2, double option3)
{
	for(int i = 0; i < EFECT_NUM; i++){
		if(efe[i] != NULL) continue;

		efe[i] = new Efect(px, py, type, option1, option2, option3);
		return;
	}
}


