#ifndef BOSS_DEF
#define BOSS_DEF

#include "character.h"

class ShotMgr;
class MyCharacter;
class ItemMgr;

enum { BPHASE_ASSERT, BPHASE_MAIN, BPHASE_BRAKE};

class Boss : public Character
{
private:
	int type;
	int phase;
	int state;	//èÛë‘
	int action;
	double velocity;		//à⁄ìÆë¨ìx
	double angle;	//äpìx
	int count;
	int score;
	int flash_count;

	int col_type;

public :
	Boss(int boss_index);

	int Update(ShotMgr* sht_mgr, MyCharacter* mychar, int game_level);
	virtual void Draw();
	virtual bool Collition(double a_x0, int a_w, double a_y0, int a_h);

	void MoveX(double a_x){px+=a_x;}
	void MoveY(double a_y){py+=a_y;}

	int GetState(){return state;}
	int GetCount(){return count;}
	int GetAction(){return action;}

	void SetState(int a_sta){state = a_sta;}
	void SetAngle(double a_ang){angle = a_ang;}
	void SetColType(int a_type){col_type = a_type;}
	void SetAction(int a_act){action = a_act;}
};

#endif