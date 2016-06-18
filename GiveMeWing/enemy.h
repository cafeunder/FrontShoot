#ifndef ENEMY_DEF
#define ENEMY_DEF

#include "character.h"
#include "database.h"

class ShotMgr;
class MyCharacter;
class ItemMgr;

enum {ENEST_STANDBY, ENEST_EXIST, ENEST_DEAD};

typedef struct
{
	double x0;	//初期x
	double y0;	//初期y
	int encount;//発生カウント
	int shot_interval;	//ショット間隔
	int shot_index;		//ショットデータインデックス
	int shotmove_index;//ショット動作インデックス
	int shot_type;		//ショット制御タイプ
	double shot_velo;		//ショット速度
	double velocity;		//移動速度
	double angle;
	int move_type;		//移動制御タイプ
	int brake_count;	//待機時間
	int data_index;		//データインデックス
} EnemyInitData_t;

class Enemy : public Character
{
private:
	int state;	//状態
	int count;	//カウンタ
	bool damaged;

	int encount;	//遭遇カウンタ数
	int shot_interval;	//ショットを撃つ間隔
	int shot_index;		//ショットデータインデックス
	int shotmove_index;//ショット動作インデックス
	double shot_velo;	//ショット速度
	int brake_count;	//待機時間
	double velocity;		//移動速度
	double angle;	//角度
	int score;		//得られるスコア
	int item_value;		//落とす経験値
	int turn;		//画像の角度をどのように変えるか

	int draw_efect;
	int efect_par;
	int rapid_num;	//連発数
	int interval_count;	//ショット間隔カウント

	int flash_count;
	int move_type;	//移動タイプ
	int shot_type;	//ショットタイプ
	int counter;
	bool homed;
	int col_type;

	void Shot(ShotMgr* sht_mgr, MyCharacter* mychar, bool count_shot = FALSE);

public :
	Enemy(double a_x, double a_y, int a_enc,int a_sinterval,int s_index, int sm_index, int a_brake, double a_velo, int a_move, int a_shot, double a_svelo, double a_angle, int data_index);

	void Update(ShotMgr* sht_mgr, MyCharacter* mychar, ItemMgr* item_mgr, int g_level);	//状態の更新・生存確認
	virtual void Draw(MyCharacter* mychar);

	void MoveX(double a_x){px+=a_x;}
	void MoveY(double a_y){py+=a_y;}
	void SetState(int a_s){state = a_s;}
	void SetAngle(double a_ang){angle = a_ang;}
	void SetVelocity(double a_velo){velocity = a_velo;}
	void SetTurn(int a_turn){turn=a_turn;}
	void SetColType(int c_type){col_type=c_type;}
	void SetDrawEfect(int type, int par){draw_efect=type; efect_par=par;}

	int GetState(){return state;}
	int GetCount(){return count;}
	int GetEncount(){return encount;}
	int GetShotInterval(){return shot_interval;};
	int GetShotIndex(){return shot_index;}
	int GetShotMoveIndex(){return shotmove_index;}
	int GetBrakeCount(){return brake_count;}
	int GetRapidNum(){return rapid_num;}
	double GetVelocity(){return velocity;}
	double GetShotVelo(){return shot_velo;}
	double GetAngle(){return angle;}
	int GetColType(){return col_type;}
	bool GetOnTheGround(){if(col_type == ENECTYPE_MYCHARNOCOL || col_type == ENECTYPE_ALLNOCOL) return TRUE; return FALSE;}
	bool GetHomed(){return homed;}
	bool GetDamaged(){return damaged;}
};

class EnemyMgr
{
private:
	Enemy** enemy;	//エネミーポインタ配列
	int ene_num;	//所持しているエネミー数

public :
	EnemyMgr(int num, EnemyInitData_t* init_data);	//コンストラクタ
	~EnemyMgr();

	void Update(ShotMgr* sht_mgr, MyCharacter* mychar, ItemMgr* item_mgr, int count, int game_level);	//メンバの状態更新

	bool Collition(int a_x, int a_w, int a_y, int a_h);	//当たり判定
	void Draw(int count, MyCharacter* mychar);		//描画
	void OnTheDraw(int count, MyCharacter* mychar);

	void QueryNearEnemyPos(double px, double py, double* bf_x, double* bf_y);
};

#endif