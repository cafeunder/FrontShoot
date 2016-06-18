#ifndef SHOT_DEF
#define SHOT_DEF

#include "character.h"

class MyCharacter;
class ShotMgr;

enum {SHOTSIDE_MYCHAR, SHOTSIDE_ENEMY};
enum {SHOTCOLOR_RED, SHOTCOLOR_BLUE, SHOTCOLOR_YELLOW, SHOTCOLOR_MAGENTA};

class Shot : public Character
{
private:
	double velo;	//移動速度
	double angle;	//移動角度

	int count;
	int power;		//与えるダメージ
	int ope_type;	//制御タイプ
	int through;	//どの属性かどうか
	int death_type;

	int side;		//マイキャラの弾か、エネミーの弾か

	double mv_x;	//x軸移動量
	double mv_y;	//y軸移動量

	bool hit;	//当たったかどうか
	bool graze_flag;
	bool graze;

	int draw_efect;
	int efect_par;

public :
	Shot(double a_x, double a_y, int a_w, int a_h, 
		int a_life, int a_imgnum, double a_v, double a_ang, int a_pow, int a_thr, int a_type, int a_side, int a_color, int d_type, bool a_graze, int a_efect, int a_efect_par):Character(a_x, a_y, a_w, a_h, a_life, a_imgnum)
			{power=a_pow;ope_type=a_type;through=a_thr;side=a_side;velo=a_v;angle=a_ang;count=0;hit=FALSE;graze=FALSE;img_div=a_color;death_type=d_type;
			 draw_efect=a_efect;efect_par=a_efect_par;graze_flag = a_graze;};

	int Update(MyCharacter* mychar, ShotMgr* sht_mgr);	//状態更新
	virtual bool Collition(double a_x0, int a_w, double a_y0, int a_h);
		//当たり判定
	bool JudgeGraze(double x0, int a_w, double y0, int a_h);
	virtual void Draw();

	void MoveX(double a_x);	//x軸方向の移動
	void MoveY(double a_y);	//y軸方向の移動

	int GetPower(){return power;}
	int GetThrough(){return through;}
	int GetSide(){return side;}
	double GetVelocity(){return velo;}
	double GetAngle(){return angle;};
	int GetCount(){return count;}
	bool GetHit(){return hit;}

	void SetVelocity(double a_velo){velo=a_velo;}
	void SetAngle(double a_angle){angle=a_angle;}
	void SetHit(bool a_hit){hit=a_hit;}
	void SetColor(int color){img_div = color;}
};



#define EXISTSHOT_MAX 200	//存在しうるショットの最大数

class ShotMgr
{
private:
	Shot* shot[EXISTSHOT_MAX];	//ショットポインタ配列

public :
	ShotMgr();
	~ShotMgr();

	void Draw();	//描画
	void Update(MyCharacter* mychar);	//メンバの更新
	bool ColCharCalc(Character* chrm, int side);
		//ショットとキャラクタの当たり判定と、当たったときの処理
	void ColShotCalc();
		//ショットとショットの当たり判定
	int JudgeGraze(double x0, int width, double y0, int height, int side);

	int SetShot(double px, double py, double velo, double angle, int shot_type, int ope_type, int side, int color,bool adj = TRUE);
		//新しいショットの予約
};


#endif