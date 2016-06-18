#ifndef ENE_OPE_DEF
#define ENE_OPE_DEF

class Enemy;

enum {
	ENEMOVE_FLATY,
	ENEMOVE_FLATMOVE,
	ENEMOVE_MOVEWAITY,
	ENEMOVE_MOVEWAITYNOANG,
	ENEMOVE_MOVEWAITYACCEL,
	ENEMOVE_MOVEWAITYACCELNOANG,
	ENEMOVE_MOVEWAITYSLOW,
	ENEMOVE_MOVEWAITYSLOWNOANG,
	ENEMOVE_TOWARDCENTER,
	ENEMOVE_TOWARDEDGE,
	ENEMOVE_ACCELTURN,
	ENEMOVE_TURNRIGHTANGLE,
	ENEMOVE_TURNLEFTANGLE,
	ENEMOVE_TURN90,
	ENEMOVE_LASERSHOT,
	ENEMOVE_LASERSHOTWAIT,
	ENEMOVE_TIMERBOMB,
	ENEMOVE_MOVECIRCLE,
	ENEMOVE_MOVEHEXFLAT,
	ENEMOVE_MOVEHEXCLOCK,
	ENEMOVE_THUNDER,
	ENEMOVE_RUSH,
	ENEMOVE_MADNESS,
	ENEMOVE_MOVEHEADFLATMOVE,
	ENEMOVE_MOVEHEXSQ,
	ENEMOVE_MOVESQ,
	ENEMOVE_NUM
};

extern int (*EneMove_Operate[ENEMOVE_NUM])(Enemy* ene, MyCharacter* mychar);
					//関数ポインタ配列の宣言


class Shot;
class MyCharacter;

enum {
	ENESHOT_FLATMOVE,		 //水平に発射
	ENESHOT_FLATMOVE5,		 //水平に発射
	ENESHOT_TOWARDMYCHAR,//マイキャラに向けて発射
	ENESHOT_TOWARDMYCHAR3,
	ENESHOT_TOWARDMYCHAR5,
	ENESHOT_FRONTRANDOM, //前方に向けてランダムに発射
	ENESHOT_FRONTRANDOM5,
	ENESHOT_MYCHARRANDOM5,
	ENESHOT_MYCHARRANDOM3,
	ENESHOT_FRONT3WAY,	//前方に向けて３発拡散発射
	ENESHOT_FRONT5WAY,	//前方に向けて５発拡散発射
	ENESHOT_FRONT7WAYHORM,
	ENESHOT_ALLRANGE30,
	ENESHOT_ALLRANGE15,
	ENESHOT_ALLRANGERAND16,
	ENESHOT_SPIRAL,
	ENESHOT_ALLRANGERAND30,
	ENESHOT_LASER,
	ENESHOT_NBOMBFLAME,
	ENESHOT_TBOMBFLAME,
	ENESHOT_HEXSHOT,
	ENESHOT_TRISHOT,
	ENESHOT_THUNDER,
	ENESHOT_NUM
};

extern int (*EneShot_Operate[ENESHOT_NUM])(Enemy* ene, ShotMgr* sht_mgr, MyCharacter* mychar);
					//関数ポインタ配列の宣言

#endif