#ifndef MYCHAR_DEF
#define MYCHAR_DEF

#include "character.h"

class EnemyMgr;
class ShotMgr;
class Satellite;
class Boss;

#define MYSHOT_MAXCHARGE 100	//チャージの最大値
#define MYSHOT_MAXLEVEL 4	//チャージの最大値
#define RATE_MAX 99.9	//レートの最大値
#define LEFT_BONUS 10000

class MyCharacter : public Character
{
private:
	Satellite* sate[2];

	int g_width;	//かすり判定幅
	int g_height;	//かすり判定高さ

	double rate;	//ポイント倍率
	int point;	//ポイント
	int score;	//スコア
	int charge;	//チャージ

	int invin_count;	//無敵時間
	int dash_count;		//ダッシュカウント
	double dash_invin;	//ダッシュ時の無敵時間
	int shot_exp;		//ショット経験値
	int shot_level;		//ショットレベル
	int interval_count;	//ショット休み中カウンタ

	void SetSatellite(double ax, double ay, int num);
	void DrawSatellite();	//サテライトの描画
	void UpdateSatellite(ShotMgr* sht_mgr = NULL, EnemyMgr* ene_mgr = NULL);
		//サテライトの更新

	void Move();	//移動制御
	int Shot(ShotMgr* sht_mgr); //ショット制御

public :
	MyCharacter(double a_x, double a_y, int a_w, int a_h, int a_gw, int a_gh, int a_life, int a_imgnum);
	~MyCharacter();
	void Initialize();

	int Update();
	int Update(EnemyMgr* ene_mgr,ShotMgr* sht_mgr, Boss* boss); //入力に対する状態の更新
	virtual void Draw(int count);
	
	void AddShotExp(int a_exp){shot_exp+=a_exp;}
	bool JudgeMaxCharge(){if(charge == MYSHOT_MAXCHARGE) return TRUE; return FALSE;}

	void AddScore(int a_scr){point+=a_scr;}
	void AddCharge(int add){charge+=add;if(charge > MYSHOT_MAXCHARGE) charge = MYSHOT_MAXCHARGE; if(charge < 0) charge = 0;}
	void ReleasePoint();

	int GetShotExp(){return shot_exp;}
	double GetShotExpPer();
	int GetShotLv(){return shot_level;}
	int GetScore(){return score;}
	int GetPoint(){return point;}
	double GetChargePer(){return (double)charge/MYSHOT_MAXCHARGE;}
	double GetRate(){return rate;}
	int GetSateNum(){if(sate[1] != NULL) return 2; else if(sate[0] != NULL) return 1; return 0;}
};

#endif