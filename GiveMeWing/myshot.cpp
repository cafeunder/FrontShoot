#include <DxLib.h>
#include <math.h>
#include "main.h"
#include "database.h"
#include "sht_ope.h"
#include "shot.h"
#include "mychar.h"
#include "myshot.h"

int MyShot_NShotLevel1(MyCharacter* mychar, ShotMgr* sht_mgr)
{
	double px = mychar->GetPosX();
	double py = mychar->GetPosY();
	int width = mychar->GetWidth();
	int height = mychar->GetHeight();
	sht_mgr->SetShot(px+width/2, py, 10, -PI/2, SHOT_NSHOT1, SHOTOPE_FLATMOVE, SHOTSIDE_MYCHAR, 0);
	return 12;
}

int MyShot_NShotLevel2(MyCharacter* mychar, ShotMgr* sht_mgr)
{
	double px = mychar->GetPosX();
	double py = mychar->GetPosY();
	int width = mychar->GetWidth();
	int height = mychar->GetHeight();

	sht_mgr->SetShot(px+width/2-18, py+15, 10, -PI/2, SHOT_NSHOT2_2, SHOTOPE_FLATMOVE, SHOTSIDE_MYCHAR, 0);
	sht_mgr->SetShot(px+width/2, py, 10, -PI/2, SHOT_NSHOT2_1, SHOTOPE_FLATMOVE, SHOTSIDE_MYCHAR, 0);
	sht_mgr->SetShot(px+width/2+18, py+15, 10, -PI/2, SHOT_NSHOT2_2, SHOTOPE_FLATMOVE, SHOTSIDE_MYCHAR, 0);
	return 12;
}

int MyShot_NShotLevel3(MyCharacter* mychar, ShotMgr* sht_mgr)
{
	double px = mychar->GetPosX();
	double py = mychar->GetPosY();
	int width = mychar->GetWidth();
	int height = mychar->GetHeight();

	sht_mgr->SetShot(px+width/2-18, py+15, 10, -PI/2, SHOT_NSHOT3_2, SHOTOPE_FLATMOVE, SHOTSIDE_MYCHAR,0);
	sht_mgr->SetShot(px+width/2, py, 10, -PI/2, SHOT_NSHOT3_1, SHOTOPE_FLATMOVE, SHOTSIDE_MYCHAR,0);
	sht_mgr->SetShot(px+width/2+18, py+15, 10, -PI/2, SHOT_NSHOT3_2, SHOTOPE_FLATMOVE, SHOTSIDE_MYCHAR,0);

	sht_mgr->SetShot(px+width/2, py, 10, -PI/2.25, SHOT_NSHOT3_REV, SHOTOPE_FLATMOVE, SHOTSIDE_MYCHAR,0);
	sht_mgr->SetShot(px+width/2-1, py+8, 10, -PI/2.25, SHOT_NSHOT3_REV, SHOTOPE_FLATMOVE, SHOTSIDE_MYCHAR,0);
	sht_mgr->SetShot(px+width/2-2, py+16, 10, -PI/2.25, SHOT_NSHOT3_REV, SHOTOPE_FLATMOVE, SHOTSIDE_MYCHAR,0);

	sht_mgr->SetShot(px+width/2, py, 10, PI+PI/2.25, SHOT_NSHOT3_REV, SHOTOPE_FLATMOVE, SHOTSIDE_MYCHAR,0);
	sht_mgr->SetShot(px+width/2+1, py+8, 10, PI+PI/2.25, SHOT_NSHOT3_REV, SHOTOPE_FLATMOVE, SHOTSIDE_MYCHAR,0);
	sht_mgr->SetShot(px+width/2+2, py+16, 10, PI+PI/2.25, SHOT_NSHOT3_REV, SHOTOPE_FLATMOVE, SHOTSIDE_MYCHAR,0);

	return 8;
}

int (*MyShot_NShot[MYSHOT_MAXLEVEL+1])(MyCharacter* mychar, ShotMgr* sht_mgr) =
{
	MyShot_NShotLevel1,
	MyShot_NShotLevel2,
	MyShot_NShotLevel2,
	MyShot_NShotLevel3,
	MyShot_NShotLevel3,
};

int MyShotOperate(int shot_level, MyCharacter* mychar, ShotMgr* sht_mgr)
{
	assert(shot_level >= 0);	//shot_levelÇÕ0à»è„Ç≈ñ≥ÇØÇÍÇŒÇ»ÇÁÇ»Ç¢
	assert(shot_level < MYSHOT_MAXLEVEL+1);	//shot_levelÇÕMYSHOT_MAXLEVELñ¢ñûÇ≈ñ≥ÇØÇÍÇŒÇ»ÇÁÇ»Ç¢
	return MyShot_NShot[shot_level](mychar, sht_mgr);
}
