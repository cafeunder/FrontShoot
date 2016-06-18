#include <DxLib.h>
#include "database.h"
#include "shot.h"
#include "main.h"
#include "sht_ope.h"
#include "image.h"
#include "efect.h"

/*
クラス名 : Shot
	マイキャラやエネミーが発射する弾

	継承 : Character

	記述ファイル : shot.h
*/

int Shot::Update(MyCharacter* mychar, ShotMgr* sht_mgr)
{
	if(life <= 0) return 1;;

	int result = 0;

	mv_x = 0; mv_y = 0;	//移動量リセット

	//制御関数呼び出し
	result = Shot_Operate[ope_type](this, sht_mgr, mychar);
	//制御関数は、 ファイル:sht_ope.cpp に記述してある

	if( death_type != SHOTDF_NODEATH && (
		((px+width < 0 || px > GAME_WIDTH) && (death_type != SHOTDF_NOSIDE)) ||
		py+height < 0 || (py > GAME_HEIGHT && death_type != SHOTDF_NOUNDER)) ) result = 1;

	count++;
	return result;
}

void Shot::MoveX(double a_x)
{
	px += a_x;
	mv_x = a_x;
}

void Shot::MoveY(double a_y)
{
	py += a_y;
	mv_y = a_y;
}

/*
ショットとキャラクタの当たり判定は複雑で、単純な当たり判定では、
斜め移動時の処理や、速く動く弾などの当たり判定にバグが生じる。

そのため、記憶した移動量を使って一次方程式の傾きを求め、
刻み幅１で補正しながら判定を行っている。

つまりは当たり判定を行っているわけなので、動作を把握する必要はないが、
こればかりはggっても出てこないので、動作を把握したいなら直接聞いて。
*/
bool Shot::Collition(double a_x0, int a_w, double a_y0, int a_h)
{
	double a_x1 = a_x0+a_w;	//物体aの右下x
	double a_y1 = a_y0+a_h; //aの右下y
	double b_x0 = px;		//物体b(=this)の左上x
	double b_y0 = py;		//bの左上y
	double b_x1 = px+width;	//bの右下y
	double b_y1 = py+height;//bの右下y

	double grad = 0;	//一次方程式の傾き
	if(mv_x != 0 && mv_y != 0){	//ｘ、ｙともに動いていれば、
		grad = mv_y/mv_x;	//傾きの計算
	}
	//このような計算をしているのは、mv_x=0となったときの零除を防ぐためである

	if(mv_x != 0){	//x軸方向に動いていれば、
		for(int i = 0; i < (int)mv_x; i++){	//動いた分の小数点切捨て分反復
			double adj_x0 = b_x0 - i;		//反復分を現在xから減算
			double adj_y0 = b_y0 - i * grad;	//反復分＊傾きを現在yから減算
			double adj_x1 = adj_x0 + width;	//右下x
			double adj_y1 = adj_y0 + height;//右下y

			if((a_x0 < adj_x1) && (adj_x0 < a_x1) && (a_y0 < adj_y1) && (adj_y0 < a_y1)){	
				//補正した値で当たり判定式
				return TRUE;	//当たっていたらTRUEを返す
			}
		}
	} 
	else if(mv_y != 0){	//x軸方向に動いていなくて、かつy軸方向には動いていれば、
		for(int i = 0; i < (int)mv_y; i++){	//動いた分の小数点切捨て分反復

			//基本、x軸方向に動いたときと同じだが、ここではx軸方向の補正が必要ないので、
			double adj_y0 = b_y0 - i;
			double adj_y1 = adj_y0 + height;
			//y軸のみを垂直に補正する
		
			if((a_x0 < b_x1) && (b_x0 < a_x1) && (a_y0 < adj_y1) && (adj_y0 < a_y1)){	
				//補正した値で当たり判定式
				return TRUE;	//当たっていたらTRUEを返す
			}
		}
	}

	if((a_x0 < b_x1) && (b_x0 < a_x1) && (a_y0 < b_y1) && (b_y0 < a_y1)){
		//最後に、現在位置で当たり判定
		return TRUE;	//当たっていたらTRUEを返す
	}

	return FALSE;
}

bool Shot::JudgeGraze(double x0, int a_w, double y0, int a_h)
{
	if(!graze_flag) return FALSE; 
	if(!graze && Collition(x0, a_w, y0, a_h)){graze = TRUE; return TRUE;}
	return FALSE;
}

void Shot::Draw()
{
	//当たり矩形が、画像の中心に来るように補正
	float draw_x = (float)px - (img_w/2 - width/2);
	float draw_y = (float)py - (img_h/2 - height/2);
	//

	if(graze) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
	switch(draw_efect){
	case EF_ALPHABLEND :
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, efect_par);
		break;
	case EF_ADDBLEND :
		SetDrawBlendMode(DX_BLENDMODE_ADD, efect_par);
		break;
	case EF_ANIME :
		if(count%efect_par == 0) img_div = (img_div+1)%img_db.QueryImgData(img_index)->div_num;
		break;
	case EF_ALPHAMOTION :
		efect.SetEfect(draw_x+(double)img_w/2, draw_y+(double)img_h/2, EFECT_ALPHAMOTION, img_index, efect_par, angle);
		break;
	}

	//回転描画
	image.DrawRotaImageF(draw_x+img_w/2, draw_y+img_h/2, 1.0, angle+PI/2, img_index, TRUE, img_div);
	//回転描画は、画像の中心座標を指定する

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 126);

#ifdef DEBUG_CODE
	_DrawBox(int(px), int(py), int(px)+width, int(py)+height, GetColor(0,0,255),FALSE);
	//あたり矩形範囲の表示
#endif
}

/*
クラス名 : ShotMgr
	ショットの管理クラス

	記述ファイル : shot.h
*/

ShotMgr::ShotMgr()
{
	for(int i = 0; i < EXISTSHOT_MAX; i++){
		//ショットは予約していないときはNULLなので、
		shot[i] = NULL;
		//NULLで初期化
	}
}

ShotMgr::~ShotMgr()
{
	for(int i = 0; i < EXISTSHOT_MAX; i++){
		if(shot[i] == NULL) continue;	//予約されていないならそのまま
		delete shot[i];	//されていたら、破棄
		//なお、これはデストラクタなので、NULLを詰める必要はない
	}
}

void ShotMgr::Draw()
{
#ifdef DEBUG_CODE
	int num = 0;
#endif

	for(int i = 0; i < EXISTSHOT_MAX; i++){
		if(shot[i] == NULL) continue;	//予約されていないなら描画しない
		shot[i]->Draw();	//されていたら、描画

#ifdef DEBUG_CODE
		num++;
#endif

	}

#ifdef DEBUG_CODE
	int color = GetColor(255,255,255);
	if(num > 90) color = GetColor(255,0,0);

	DrawFormatString(50,0,color, "弾の数:%d", num);
#endif
}

void ShotMgr::Update(MyCharacter* mychar)
{
	for(int i = 0; i < EXISTSHOT_MAX; i++){
		if(shot[i] == NULL) continue;	//予約されていないなら更新しない
		if(shot[i]->Update(mychar, this) == 1){	//されていたら、更新
			delete shot[i];	//1が帰ってきたらオブジェクトを破棄
			shot[i] = NULL;	//NULLをつめる
			continue;
		}
	}

	ColShotCalc();
}

bool ShotMgr::ColCharCalc(Character* chr, int side)
{
	bool result = FALSE;
	for(int i = 0; i < EXISTSHOT_MAX; i++){
		if(shot[i] == NULL) continue;	//予約されていないなら、判定しない
		if(shot[i]->GetSide() == side) continue;	
				//ショットの撃ち主と、被判定者が同じサイドなら、判定しない

		if(shot[i]->Collition(chr->GetPosX(), chr->GetWidth(),
				chr->GetPosY(), chr->GetHeight())){
			//当たっていれば、

			shot[i]->SetHit(TRUE);	//当たったことをショットに伝える
			chr->Damage(shot[i]->GetPower());	//引数で与えられたキャラタにダメージ

			if(shot[i]->GetThrough() != SHOTTH_ENEMY && shot[i]->GetThrough() != SHOTTH_ALL){	//貫通属性でなければ、
				delete shot[i];	//当たった時点でオブジェクトを破棄
				shot[i] = NULL;	//NULLを詰める
			}

			if(side == SHOTSIDE_MYCHAR) return TRUE;	//マイキャラは一度に一発しか判定を行わない
			result = TRUE;
		}
	}
	return result;
}

void ShotMgr::ColShotCalc()
{
	for(int i = 0; i < EXISTSHOT_MAX; i++){
		if(shot[i] == NULL) continue;	//予約されていないなら、判定しない

		for(int j = 0; j < EXISTSHOT_MAX; j++){
			if(shot[j] == NULL) continue;	//予約されていないなら、判定しない
			if(shot[j]->GetSide() == shot[i]->GetSide()) continue;
			if(shot[i]->GetThrough() != SHOTTH_SHOT && shot[i]->GetThrough() != SHOTTH_ALL){
				if(shot[j]->GetThrough() != SHOTTH_SHOTED) continue;
			}

			if(shot[j]->Collition(shot[i]->GetPosX(), shot[i]->GetWidth(), shot[i]->GetPosY(), shot[i]->GetHeight())){
				shot[j]->Damage(1);
				if(shot[j]->GetThrough() == SHOTTH_SHOTED){
					shot[i]->Damage(1);
				}
			}
		}
	}
}

int ShotMgr::JudgeGraze(double x0, int width, double y0, int height, int side)
{
	int num = 0;
	for(int i = 0; i < EXISTSHOT_MAX; i++){
		if(shot[i] == NULL) continue;	//予約されていないなら、判定しない
		if(shot[i]->GetSide() == side) continue;	//同じサイドなら判定しない

		if(shot[i]->JudgeGraze(x0, width, y0, height)) num++;
	}

	return num;
}


/*
予約時、キャラクタから受けとるデータは、

発射x,y　移動速度　移動角度　弾のデータタイプ　弾の制御タイプ

つまり、

どのような弾 : 弾のデータタイプ
どのように発射 : 発射x,y　移動速度　移動角度
どのように動くか : 弾の制御タイプ

とこういうことである。
*/
int ShotMgr::SetShot(double px, double py, double velo, double angle, int shot_type, int ope_type, int side, int color,bool adj)
{
	for(int i = 0; i < EXISTSHOT_MAX; i++){
		if(shot[i] != NULL) continue;	//予約されて「いる」なら、予約しない

		ShotData_t* data = sht_db.QueryShotData(shot_type);	//ショットデータクエリ
		
		int adj_y = 0;	//補正yを０に仮定
		int adj_x = 0;	//補正xを０に仮定
		if(adj && side == SHOTSIDE_MYCHAR) adj_y = data->height;
			//もし、ショットがマイキャラによるものであれば、ショットの位置をショットの高さ分だけ補正

		if(adj) adj_x = data->width/2;

		shot[i] = new Shot(px-adj_x, py-adj_y, data->width, data->height, data->life, 
			data->img_num, velo, angle, data->power, data->through, ope_type, side, color, data->death_type, data->graze, data->efect, data->efect_par);
									//されていないなら、そこに予約する
		return 0;
	}

	return 1;	//全部探して、予約できなかったら、１を返す
}