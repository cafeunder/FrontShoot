#include <DxLib.h>
#include "image.h"
#include "main.h"
#include "database.h"

Image image;

void Image::DrawImageF(float x, float y, int GrIndex, bool TransFlag, int div, bool adj)
{
	Image_t* img = img_db.QueryImgData(GrIndex);
	if(div > img->div_num-1) div = img->div_num-1; 

	float adj_x = GAME_X;
	float adj_y = GAME_Y;
	if(!adj) {
		adj_x = 0;
		adj_y = 0;
	}

	int img_w, img_h;
	GetGraphSize(img->img_han,&img_w,&img_h);
	int per_w = img_w/img->div_num;

	SetDrawMode(DX_DRAWMODE_BILINEAR);
	DrawRectGraphF(x+adj_x,y+adj_y,per_w*div,0,per_w,img_h,img->img_han,TransFlag,FALSE);
	SetDrawMode(DX_DRAWMODE_NEAREST);
}

void Image::DrawImage(int x, int y, int GrIndex, bool TransFlag, int div, bool adj)
{
	Image_t* img = img_db.QueryImgData(GrIndex);
	if(div > img->div_num-1) div = img->div_num-1; 

	int adj_x = GAME_X;
	int adj_y = GAME_Y;
	if(!adj) {
		adj_x = 0;
		adj_y = 0;
	}	

	int img_w, img_h;
	GetGraphSize(img->img_han,&img_w,&img_h);
	int per_w = img_w/img->div_num;

	DrawRectGraph(x+adj_x,y+adj_y,per_w*div,0,per_w,img_h,img->img_han,TransFlag,FALSE);
}

void Image::DrawRotaImageF(float x, float y, double ExRate, double Angle, int GrIndex, bool TransFlag, int div, bool adj, bool corner)
{
	Image_t* img = img_db.QueryImgData(GrIndex);
	if(div > img->div_num-1) div = img->div_num-1; 

	float draw_x = x;
	float draw_y = y;

	if(corner){
		int img_w, img_h;
		GetImageSize(GrIndex, &img_w, &img_h);

		draw_x+=(float)img_w/2;
		draw_y+=(float)img_h/2;
	}

	if(adj){
		draw_x += GAME_X;
		draw_y += GAME_Y;
	}


	int img_w, img_h;
	GetGraphSize(img->img_han,&img_w,&img_h);
	int per_w = img_w/img->div_num;

	SetDrawMode(DX_DRAWMODE_BILINEAR);
	DrawRectRotaGraphF(draw_x,draw_y,per_w*div,0,per_w,img_h,ExRate,Angle,img->img_han,TransFlag,FALSE);
	SetDrawMode(DX_DRAWMODE_NEAREST);
}

void Image::DrawRotaImage(int x, int y, double ExRate, double Angle, int GrIndex, bool TransFlag, int div, bool adj, bool corner)
{
	Image_t* img = img_db.QueryImgData(GrIndex);
	if(div > img->div_num-1) div = img->div_num-1; 

	int draw_x = x;
	int draw_y = y;

	if(corner){
		int img_w, img_h;
		GetImageSize(GrIndex, &img_w, &img_h);

		draw_x+=img_w/2;
		draw_y+=img_h/2;
	}

	if(adj){
		draw_x = GAME_X;
		draw_y = GAME_Y;
	}


	int img_w, img_h;
	GetGraphSize(img->img_han,&img_w,&img_h);
	int per_w = img_w/img->div_num;

	DrawRectRotaGraph(draw_x,draw_y,per_w*div,0,per_w,img_h,ExRate,Angle,img->img_han,TransFlag,FALSE);

/*
	Image_t* img = img_db.QueryImgData(GrIndex);
	if(div > img->div_num-1) div = img->div_num-1; 

	int adj_x = GAME_X;
	int adj_y = GAME_Y;
	if(!adj) {
		adj_x = 0;
		adj_y = 0;
	}

	DrawRotaGraph(x+adj_x, y+adj_y, ExRate, Angle, img->img_han,TransFlag);
*/
}

void Image::GetImageSize(int index, int *BufXSize, int *BufYSize)
{
	Image_t* img = img_db.QueryImgData(index);

	GetGraphSize(img->img_han, BufXSize, BufYSize);
}