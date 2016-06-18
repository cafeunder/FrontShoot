#ifndef SATELLITE_DEF
#define SATELLITE_DEF

class Character;
class MyCharacter;
class ShotMgr;
class EnemyMgr;

enum {SATE_ATK, SATE_GRD, SATE_NUM};

#define SATE_SIZE 1	//サテライトのサイズ
#define SATE_PARGE 350	//パージ時にサテライトが打ちあがる距離
class Satellite : public Character
{
private:
	int type;	//サテライトのタイプ
	int count;	//カウンタ
	int shot_interval;	//弾の発射
	bool smash;	//スマッシュ中かどうか
	int num;

	bool move;
	int move_count;

public :
	Satellite(double a_x, double a_y, int a_width, int a_height, int img_han, int a_num, int a_type);

	int Update(MyCharacter* mychar, ShotMgr* sht_mgr = NULL, EnemyMgr* ene_mgr = NULL);
	virtual void Draw();
	int GetCount(){return count;}
	int GetType(){return type;}

	void SetType(int a_type);
	bool GetSmash(){return smash;}
	int GetNum(){return num;}
	bool GetMove(){return move;}

	double QuerySmashAccel();
	bool JudgeEnableSmash(MyCharacter* mychar);
	
	void Reset(){count = 0; shot_interval = 0;}
};

#endif