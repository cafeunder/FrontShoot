#ifndef CHARACTER_DEF
#define CHARACTER_DEF

class Character
{
protected:
	double px;	//現在x
	double py;	//現在y
	int width;	//当たり矩形の幅	
	int height; //当たり矩形の高さ

	int img_index;//イメージハンドル
	int img_w;	//画像幅
	int img_h;	//画像高さ
	int img_div;//描画ブロック数

	int life;	//耐久力

public:
	Character(){};
	Character(double a_x, double a_y, int a_w, int a_h, int a_life, int a_imgindex);
	void Initialize(){px=0;py=0;};

	virtual bool Damage(int damage);	//ダメージ処理
	virtual void Draw();	//描画n
	virtual bool Collition(double a_x0, int a_w, double a_y0, int a_h);
		//当たり判定

	double GetPosX(){return px;}
	double GetPosY(){return py;}
	void SetPosX(double a_x){px = a_x;}
	void SetPosY(double a_y){py = a_y;}
	void SetImgDiv(int a_div){img_div=a_div;}

	int GetWidth(){return width;}
	int GetHeight(){return height;}
	int GetImgWidth(){return img_w;}
	int GetImgHeight(){return img_h;}
	int GetLife(){return life;}
};

#endif