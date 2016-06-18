#include <DxLib.h>
#include <math.h>
#include "main.h"
#include "shot.h"
#include "database.h"
#include "boss.h"
#include "sht_ope.h"
#include "mychar.h"
#include "efect.h"
#include "image.h"
#include "sound.h"

typedef struct
{
	int img_index;
	double fx;
	double fy;
	double f_angle;
	int width;
	int height;
	int life;
	int score;
} BossData_t;

BossData_t BossDataBase[2] = 
{
	{IMG_S2BOSS, 120, -250, PI/2, 50, 250, 400, 10000},
	{IMG_S4BOSS, 124, -280, PI/2, 134, 280, 1500, 15000}
};

void BossH_Stage2(Boss* boss, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	if(boss->GetState() == 0){
		boss->MoveY(2);

		if(boss->GetCount()%100 == 0){
			for(int i = 0; i < 12; i++){
				sht_mgr->SetShot(boss->GetPosX()+boss->GetWidth()/2, boss->GetPosY()+boss->GetHeight()/2, 5, (((30.0)*i-180.0)/180.0)*PI+PI/2, SHOT_ENEEN, SHOTOPE_FLATMOVE, SHOTSIDE_ENEMY,SHOTCOLOR_BLUE);
			}
		}

		if((boss->GetCount()%150 <= 40) && boss->GetCount()%10 == 0){
			double y = mychar->GetPosY()+mychar->GetHeight()/2 - boss->GetPosY();	//�}�C�L�����ƃG�l�~�[��y���������߂�
			double x = mychar->GetPosX()+mychar->GetWidth()/2 - (boss->GetPosX()+boss->GetWidth()/2);	//x���������߂�
			double angle = atan2(y,x);	//tan-1(x/y)�@�����Ƃ߂�

			sht_mgr->SetShot(boss->GetPosX()+boss->GetWidth()/2, boss->GetPosY(), 7, angle, SHOT_ENESMALL, SHOTOPE_FLATMOVE, SHOTSIDE_ENEMY,SHOTCOLOR_RED);
		}

		if(boss->GetCount()%60 < 20 && boss->GetCount()%5 == 0){
			double y = mychar->GetPosY()+mychar->GetHeight()/2 - (boss->GetPosY()+boss->GetHeight());	//�}�C�L�����ƃG�l�~�[��y���������߂�
			double x = mychar->GetPosX()+mychar->GetWidth()/2 - (boss->GetPosX()+boss->GetWidth()/2);	//x���������߂�
			double angle = atan2(y,x);	//tan-1(x/y)�@�����Ƃ߂�

			int rand = GetRand(90) - 45;	//-30~30�͈̔͂Ń����_���Ȓl������
			double rand_ang = ((double)rand/180.0)*PI+angle;

			sht_mgr->SetShot(boss->GetPosX()+boss->GetWidth()/2, boss->GetPosY()+boss->GetHeight(), 5, rand_ang, SHOT_ENESQ, SHOTOPE_FLATMOVE, SHOTSIDE_ENEMY,SHOTCOLOR_MAGENTA);
		}

		if(boss->GetPosY()-50 > GAME_HEIGHT){
			boss->SetColType(ENECTYPE_ALLNOCOL);
			boss->SetState(GetRand(1)+1);
			boss->SetAngle(-PI/2);
			boss->SetImgDiv(1);
		}
	} else 
	if(boss->GetState() != 0){
		boss->MoveY(-4);

		if(boss->GetState() == 1 && boss->GetCount()%100 == 0){
			efect.SetEfect(mychar->GetPosX()-mychar->GetImgWidth()/2+1,mychar->GetPosY()-mychar->GetImgHeight()/2+1,EFECT_MYCHARSITE);

			for(int i = 0; i < 12; i++){
				double px = mychar->GetPosX();
				double py = mychar->GetPosY();
				double ang = (((30.0)*i-180.0)/180.0)*PI+PI/2;
			
				double sx = 800*cos(ang+PI) + px;
				double sy = 800*sin(ang+PI) + py;

				sht_mgr->SetShot(sx, sy, 4, ang, SHOT_ST2BOSS, SHOTOPE_ST2BOSS, SHOTSIDE_ENEMY, 0, FALSE);
			}
		}
		if(boss->GetState() == 2 && boss->GetCount()%10 == 0){
			int i = (boss->GetCount()/10)%25;
			double sx = i*15;
			double sy = 0;

			double velo = (double)(GetRand(70))/10 + 1.0;

			sht_mgr->SetShot(sx, sy, velo, PI/2, SHOT_ENEEN, SHOTOPE_FLATMOVE, SHOTSIDE_ENEMY, 3, FALSE);
		}


		if(boss->GetCount()%10 == 0){
			int rand = GetRand(90) - 45;	//-30~30�͈̔͂Ń����_���Ȓl������
			double rand_ang = ((double)rand/180.0)*PI+PI/2;

			sht_mgr->SetShot(GAME_WIDTH/2+GAME_X, 0, 7, rand_ang, SHOT_ENESMALL, SHOTOPE_FLATMOVE, SHOTSIDE_ENEMY,SHOTCOLOR_BLUE);
		}

		if(boss->GetPosY()+boss->GetImgHeight()+50 < 0){
			boss->SetColType(ENECTYPE_NORMAL);
			boss->SetState(0);
			boss->SetAngle(PI/2);
			boss->SetImgDiv(0);
		}
	}
}

void BossS_Stage2(Boss* boss, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	if(boss->GetState() == 0){
		boss->MoveY(2);

		if(boss->GetCount()%150 == 0){
			for(int i = 0; i < 12; i++){
				sht_mgr->SetShot(boss->GetPosX()+boss->GetWidth()/2, boss->GetPosY()+boss->GetHeight()/2, 4, (((30.0)*i-180.0)/180.0)*PI+PI/2, SHOT_ENEEN, SHOTOPE_FLATMOVE, SHOTSIDE_ENEMY,SHOTCOLOR_BLUE);
			}
		}

		if((boss->GetCount()%150 <= 60) && boss->GetCount()%15 == 0){
			double y = mychar->GetPosY()+mychar->GetHeight()/2 - boss->GetPosY();	//�}�C�L�����ƃG�l�~�[��y���������߂�
			double x = mychar->GetPosX()+mychar->GetWidth()/2 - (boss->GetPosX()+boss->GetWidth()/2);	//x���������߂�
			double angle = atan2(y,x);	//tan-1(x/y)�@�����Ƃ߂�

			sht_mgr->SetShot(boss->GetPosX()+boss->GetWidth()/2, boss->GetPosY(), 5, angle, SHOT_ENESMALL, SHOTOPE_FLATMOVE, SHOTSIDE_ENEMY,SHOTCOLOR_RED);
		}

		if(boss->GetCount()%70 < 32 && boss->GetCount()%8 == 0){
			double y = mychar->GetPosY()+mychar->GetHeight()/2 - (boss->GetPosY()+boss->GetHeight());	//�}�C�L�����ƃG�l�~�[��y���������߂�
			double x = mychar->GetPosX()+mychar->GetWidth()/2 - (boss->GetPosX()+boss->GetWidth()/2);	//x���������߂�
			double angle = atan2(y,x);	//tan-1(x/y)�@�����Ƃ߂�

			int rand = GetRand(90) - 45;	//-30~30�͈̔͂Ń����_���Ȓl������
			double rand_ang = ((double)rand/180.0)*PI+angle;

			sht_mgr->SetShot(boss->GetPosX()+boss->GetWidth()/2, boss->GetPosY()+boss->GetHeight(), 5, rand_ang, SHOT_ENESQ, SHOTOPE_FLATMOVE, SHOTSIDE_ENEMY,SHOTCOLOR_MAGENTA);
		}
		if(boss->GetPosY()-50 > GAME_HEIGHT){
			boss->SetColType(ENECTYPE_ALLNOCOL);
			boss->SetState(GetRand(1)+1);
			boss->SetAngle(-PI/2);
			boss->SetImgDiv(1);
		}
	} else 
	if(boss->GetState() != 0){
		boss->MoveY(-4);

		if(boss->GetCount()%10 == 0){
			int i = (boss->GetCount()/10)%25;
			double sx = i*15;
			double sy = 0;

			double velo = (double)(GetRand(70))/10 + 1.0;

			sht_mgr->SetShot(sx, sy, velo, PI/2, SHOT_ENEEN, SHOTOPE_FLATMOVE, SHOTSIDE_ENEMY, 3, FALSE);
		}


		if(boss->GetCount()%20 == 0){
			int rand = GetRand(90) - 45;	//-30~30�͈̔͂Ń����_���Ȓl������
			double rand_ang = ((double)rand/180.0)*PI+PI/2;

			sht_mgr->SetShot(GAME_WIDTH/2+GAME_X, 0, 5, rand_ang, SHOT_ENESMALL, SHOTOPE_FLATMOVE, SHOTSIDE_ENEMY,SHOTCOLOR_BLUE);
		}

		if(boss->GetPosY()+boss->GetImgHeight()+50 < 0){
			boss->SetColType(ENECTYPE_NORMAL);
			boss->SetState(0);
			boss->SetAngle(PI/2);
			boss->SetImgDiv(0);
		}
	}
}

void BossE_Stage2(Boss* boss, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	if(boss->GetState() == 0){
		boss->MoveY(2);

		if(boss->GetCount()%200 == 0){
			for(int i = 0; i < 12; i++){
				sht_mgr->SetShot(boss->GetPosX()+boss->GetWidth()/2, boss->GetPosY()+boss->GetHeight()/2, 4, (((30.0)*i-180.0)/180.0)*PI+PI/2, SHOT_ENEEN, SHOTOPE_FLATMOVE, SHOTSIDE_ENEMY,SHOTCOLOR_BLUE);
			}
		}

		if((boss->GetCount()%150 <= 40) && boss->GetCount()%20 == 0){
			double y = mychar->GetPosY()+mychar->GetHeight()/2 - boss->GetPosY();	//�}�C�L�����ƃG�l�~�[��y���������߂�
			double x = mychar->GetPosX()+mychar->GetWidth()/2 - (boss->GetPosX()+boss->GetWidth()/2);	//x���������߂�
			double angle = atan2(y,x);	//tan-1(x/y)�@�����Ƃ߂�

			sht_mgr->SetShot(boss->GetPosX()+boss->GetWidth()/2, boss->GetPosY(), 5, angle, SHOT_ENESMALL, SHOTOPE_FLATMOVE, SHOTSIDE_ENEMY,SHOTCOLOR_RED);
		}

		if(boss->GetCount()%80 < 40 && boss->GetCount()%10 == 0){
			double y = mychar->GetPosY()+mychar->GetHeight()/2 - (boss->GetPosY()+boss->GetHeight());	//�}�C�L�����ƃG�l�~�[��y���������߂�
			double x = mychar->GetPosX()+mychar->GetWidth()/2 - (boss->GetPosX()+boss->GetWidth()/2);	//x���������߂�
			double angle = atan2(y,x);	//tan-1(x/y)�@�����Ƃ߂�

			int rand = GetRand(90) - 45;	//-30~30�͈̔͂Ń����_���Ȓl������
			double rand_ang = ((double)rand/180.0)*PI+angle;

			sht_mgr->SetShot(boss->GetPosX()+boss->GetWidth()/2, boss->GetPosY()+boss->GetHeight(), 5, rand_ang, SHOT_ENESQ, SHOTOPE_FLATMOVE, SHOTSIDE_ENEMY,SHOTCOLOR_MAGENTA);
		}

		if(boss->GetPosY()-50 > GAME_HEIGHT){
			boss->SetColType(ENECTYPE_ALLNOCOL);
			boss->SetState(GetRand(1)+1);
			boss->SetAngle(-PI/2);
			boss->SetImgDiv(1);
		}
	} else 
	if(boss->GetState() != 0){
		boss->MoveY(-4);

		if(boss->GetCount()%10 == 0){
			int i = (boss->GetCount()/10)%25;
			double sx = i*15;
			double sy = 0;

			double velo = (double)(GetRand(70))/10 + 1.0;

			sht_mgr->SetShot(sx, sy, velo, PI/2, SHOT_ENEEN, SHOTOPE_FLATMOVE, SHOTSIDE_ENEMY, 3, FALSE);
		}

		if(boss->GetPosY()+boss->GetImgHeight()+50 < 0){
			boss->SetColType(ENECTYPE_NORMAL);
			boss->SetState(0);
			boss->SetAngle(PI/2);
			boss->SetImgDiv(0);
		}
	}
}

void BossH_Stage4(Boss* boss, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	if(boss->GetState() == 0){
		if(boss->GetCount() < 120){
			boss->SetAngle(PI/2);
			boss->SetPosY(boss->GetPosY()+2);
		}
		if(boss->GetCount() == 120) boss->SetState(1);
	} else if(boss->GetState() == 1){
		if(boss->GetCount()%80 < 20){
			if(boss->GetCount()%5 == 0){
				double y = mychar->GetPosY()+mychar->GetHeight()/2 - (boss->GetPosY()+boss->GetHeight()/2);	//�}�C�L�����ƃG�l�~�[��y���������߂�
				double x = mychar->GetPosX()+mychar->GetWidth()/2 - (boss->GetPosX()+boss->GetWidth()/2);	//x���������߂�
				double angle = atan2(y,x);	//tan-1(x/y)�@�����Ƃ߂�

				sht_mgr->SetShot(boss->GetPosX()+boss->GetWidth()/2, boss->GetPosY()+boss->GetHeight()/2, 7, angle, SHOT_ENEEN, SHOTOPE_FLATMOVE, SHOTSIDE_ENEMY,SHOTCOLOR_RED);
			}
		}

		if(boss->GetCount()%10 == 0){
			for(int j = 0; j < 4; j++){
				for(int i = 0; i < 4; i++){
					int rand = GetRand(90) + 90*j;
					double angle = ((double)rand/180.0)*PI;
					//�����_���Ȓl��x���ƍl���A���W�A���ɕϊ�

					sht_mgr->SetShot(boss->GetPosX()+boss->GetWidth()/2, boss->GetPosY()+boss->GetHeight()/2, 7, angle, SHOT_ENESMALL, SHOTOPE_SLOW, SHOTSIDE_ENEMY, SHOTCOLOR_YELLOW);
				}
			}
		}

		if(boss->GetCount()%240 == 120) {
			boss->SetState(2);
			boss->SetImgDiv(1);
		}
	} else if(boss->GetState() == 2){
		if(boss->GetCount()%240 == 121) {
			efect.SetEfect(380,0,EFECT_LASEREFECT,-1,-1,3*PI/4);
			efect.SetEfect(380,160,EFECT_LASEREFECT,-1,-1,3*PI/4);
			efect.SetEfect(380,320,EFECT_LASEREFECT,-1,-1,3*PI/4);

			efect.SetEfect(0,0,EFECT_LASEREFECT,-1,-1,PI/4);
			efect.SetEfect(0,160,EFECT_LASEREFECT,-1,-1,PI/4);
			efect.SetEfect(0,320,EFECT_LASEREFECT,-1,-1,PI/4);
		} 
		if(boss->GetCount()%240 > 180){
			sht_mgr->SetShot(0,0,20,PI/4,SHOT_ENEBIGLASER,SHOTOPE_FLATMOVE,SHOTSIDE_ENEMY,0);
			sht_mgr->SetShot(0,160,20,PI/4,SHOT_ENEBIGLASER,SHOTOPE_FLATMOVE,SHOTSIDE_ENEMY,0);
			sht_mgr->SetShot(0,320,20,PI/4,SHOT_ENEBIGLASER,SHOTOPE_FLATMOVE,SHOTSIDE_ENEMY,0);

			sht_mgr->SetShot(380,0,20,3*PI/4,SHOT_ENEBIGLASER,SHOTOPE_FLATMOVE,SHOTSIDE_ENEMY,0);
			sht_mgr->SetShot(380,160,20,3*PI/4,SHOT_ENEBIGLASER,SHOTOPE_FLATMOVE,SHOTSIDE_ENEMY,0);
			sht_mgr->SetShot(380,320,20,3*PI/4,SHOT_ENEBIGLASER,SHOTOPE_FLATMOVE,SHOTSIDE_ENEMY,0);
		}

		if(boss->GetCount()%240 == 0){
			boss->SetState(1);
			boss->SetImgDiv(0);
		}
	}
}

void BossS_Stage4(Boss* boss, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	if(boss->GetState() == 0){
		if(boss->GetCount() < 120){
			boss->SetAngle(PI/2);
			boss->SetPosY(boss->GetPosY()+2);
		}
		if(boss->GetCount() == 120) boss->SetState(1);
	} else if(boss->GetState() == 1){
		if(boss->GetCount()%120 < 20){
			if(boss->GetCount()%5 == 0){
				double y = mychar->GetPosY()+mychar->GetHeight()/2 - (boss->GetPosY()+boss->GetHeight()/2);	//�}�C�L�����ƃG�l�~�[��y���������߂�
				double x = mychar->GetPosX()+mychar->GetWidth()/2 - (boss->GetPosX()+boss->GetWidth()/2);	//x���������߂�
				double angle = atan2(y,x);	//tan-1(x/y)�@�����Ƃ߂�

				sht_mgr->SetShot(boss->GetPosX()+boss->GetWidth()/2, boss->GetPosY()+boss->GetHeight()/2, 7, angle, SHOT_ENEEN, SHOTOPE_FLATMOVE, SHOTSIDE_ENEMY,SHOTCOLOR_RED);
			}
		}

		if(boss->GetCount()%30 == 0){
			for(int j = 0; j < 4; j++){
				for(int i = 0; i < 4; i++){
					int rand = GetRand(90) + 90*j;
					double angle = ((double)rand/180.0)*PI;
					//�����_���Ȓl��x���ƍl���A���W�A���ɕϊ�

					sht_mgr->SetShot(boss->GetPosX()+boss->GetWidth()/2, boss->GetPosY()+boss->GetHeight()/2, 7, angle, SHOT_ENESMALL, SHOTOPE_SLOW, SHOTSIDE_ENEMY, SHOTCOLOR_YELLOW);
				}
			}
		}

		if(boss->GetCount()%240 == 120) {
			boss->SetState(2);
			boss->SetImgDiv(1);
		}
	} else if(boss->GetState() == 2){
		if(boss->GetCount()%240 == 121) {
			efect.SetEfect(380,0,EFECT_LASEREFECT,-1,-1,3*PI/4);
			efect.SetEfect(380,160,EFECT_LASEREFECT,-1,-1,3*PI/4);
			efect.SetEfect(380,320,EFECT_LASEREFECT,-1,-1,3*PI/4);

			efect.SetEfect(0,0,EFECT_LASEREFECT,-1,-1,PI/4);
			efect.SetEfect(0,160,EFECT_LASEREFECT,-1,-1,PI/4);
			efect.SetEfect(0,320,EFECT_LASEREFECT,-1,-1,PI/4);
		} 
		if(boss->GetCount()%240 > 180){
			sht_mgr->SetShot(0,0,20,PI/4,SHOT_ENEBIGLASER,SHOTOPE_FLATMOVE,SHOTSIDE_ENEMY,0);
			sht_mgr->SetShot(0,160,20,PI/4,SHOT_ENEBIGLASER,SHOTOPE_FLATMOVE,SHOTSIDE_ENEMY,0);
			sht_mgr->SetShot(0,320,20,PI/4,SHOT_ENEBIGLASER,SHOTOPE_FLATMOVE,SHOTSIDE_ENEMY,0);

			sht_mgr->SetShot(380,0,20,3*PI/4,SHOT_ENEBIGLASER,SHOTOPE_FLATMOVE,SHOTSIDE_ENEMY,0);
			sht_mgr->SetShot(380,160,20,3*PI/4,SHOT_ENEBIGLASER,SHOTOPE_FLATMOVE,SHOTSIDE_ENEMY,0);
			sht_mgr->SetShot(380,320,20,3*PI/4,SHOT_ENEBIGLASER,SHOTOPE_FLATMOVE,SHOTSIDE_ENEMY,0);
		}

		if(boss->GetCount()%240 == 0){
			boss->SetState(1);
			boss->SetImgDiv(0);
		}
	}
}

void BossE_Stage4(Boss* boss, ShotMgr* sht_mgr, MyCharacter* mychar)
{
	if(boss->GetState() == 0){
		if(boss->GetCount() < 120){
			boss->SetAngle(PI/2);
			boss->SetPosY(boss->GetPosY()+2);
		}
		if(boss->GetCount() == 120) boss->SetState(1);
	} else if(boss->GetState() == 1){
		if(boss->GetCount()%160 < 15){
			if(boss->GetCount()%5 == 0){
				double y = mychar->GetPosY()+mychar->GetHeight()/2 - (boss->GetPosY()+boss->GetHeight()/2);	//�}�C�L�����ƃG�l�~�[��y���������߂�
				double x = mychar->GetPosX()+mychar->GetWidth()/2 - (boss->GetPosX()+boss->GetWidth()/2);	//x���������߂�
				double angle = atan2(y,x);	//tan-1(x/y)�@�����Ƃ߂�

				sht_mgr->SetShot(boss->GetPosX()+boss->GetWidth()/2, boss->GetPosY()+boss->GetHeight()/2, 7, angle, SHOT_ENEEN, SHOTOPE_FLATMOVE, SHOTSIDE_ENEMY,SHOTCOLOR_RED);
			}
		}

		if(boss->GetCount()%30 == 0){
			for(int j = 0; j < 4; j++){
				for(int i = 0; i < 4; i++){
					int rand = GetRand(90) + 90*j;
					double angle = ((double)rand/180.0)*PI;
					//�����_���Ȓl��x���ƍl���A���W�A���ɕϊ�

					sht_mgr->SetShot(boss->GetPosX()+boss->GetWidth()/2, boss->GetPosY()+boss->GetHeight()/2, 7, angle, SHOT_ENESMALL, SHOTOPE_SLOW, SHOTSIDE_ENEMY, SHOTCOLOR_YELLOW);
				}
			}
		}

		if(boss->GetCount()%240 == 120) {
			boss->SetState(2);
			boss->SetImgDiv(1);
		}
	} else if(boss->GetState() == 2){
		if(boss->GetCount()%240 == 121) {
			efect.SetEfect(380,0,EFECT_LASEREFECT,-1,-1,3*PI/4);
			efect.SetEfect(380,320,EFECT_LASEREFECT,-1,-1,3*PI/4);

			efect.SetEfect(0,0,EFECT_LASEREFECT,-1,-1,PI/4);
			efect.SetEfect(0,320,EFECT_LASEREFECT,-1,-1,PI/4);
		} 
		if(boss->GetCount()%240 > 180){
			sht_mgr->SetShot(0,0,20,PI/4,SHOT_ENEBIGLASER,SHOTOPE_FLATMOVE,SHOTSIDE_ENEMY,0);
			sht_mgr->SetShot(0,320,20,PI/4,SHOT_ENEBIGLASER,SHOTOPE_FLATMOVE,SHOTSIDE_ENEMY,0);

			sht_mgr->SetShot(380,0,20,3*PI/4,SHOT_ENEBIGLASER,SHOTOPE_FLATMOVE,SHOTSIDE_ENEMY,0);
			sht_mgr->SetShot(380,320,20,3*PI/4,SHOT_ENEBIGLASER,SHOTOPE_FLATMOVE,SHOTSIDE_ENEMY,0);
		}

		if(boss->GetCount()%240 == 0){
			boss->SetState(1);
			boss->SetImgDiv(0);
		}
	}
}

void (*BossEasy_Operate[BOSS_NUM])(Boss* boss, ShotMgr* sht_mgr, MyCharacter* mychar) =
{
	BossE_Stage2,
	BossE_Stage4,
};

void (*BossStandard_Operate[BOSS_NUM])(Boss* boss, ShotMgr* sht_mgr, MyCharacter* mychar) =
{
	BossS_Stage2,
	BossS_Stage4,
};

void (*BossHard_Operate[BOSS_NUM])(Boss* boss, ShotMgr* sht_mgr, MyCharacter* mychar) =
{
	BossH_Stage2,
	BossH_Stage4,
};

void (**Boss_Operate[3])(Boss* boss, ShotMgr* sht_mgr, MyCharacter* mychar) =
{
	BossEasy_Operate,
	BossStandard_Operate,
	BossHard_Operate,
};

Boss::Boss(int boss_index):Character()
{
	BossData_t data = BossDataBase[boss_index];
	type = boss_index;

	px = data.fx;
	py = data.fy;
	angle = data.f_angle;
	width = data.width;
	height = data.height;
	life = data.life;
	score = data.score;
	img_index = data.img_index;
	flash_count = 0;
	action = 0;

	image.GetImageSize(img_index, &img_w, &img_h);
	img_w/=img_db.QueryImgData(img_index)->div_num;
	img_div = 0;

	phase = BPHASE_ASSERT;
	state = 0;
	count = 0;
	col_type = ENECTYPE_NORMAL;
}

#define BOSSFLASH_COUNT 2
int Boss::Update(ShotMgr *sht_mgr, MyCharacter *mychar, int game_level)
{
	switch(phase){
	case BPHASE_ASSERT:
		if(count == 0){
			efect.SetEfect(0,0,EFECT_STRCALLCAUTION,img_db.QueryImgData(IMG_BOSSADRBOARD)->img_han, IMG_STRCAUTION);
		}

		if(count >= 400){
			count = 0;
			phase = BPHASE_MAIN;
			return 0;
		}
		break;

	case BPHASE_MAIN:
		Boss_Operate[game_level][type](this, sht_mgr, mychar);

		if(col_type != ENECTYPE_ALLNOCOL) {
			if(sht_mgr->ColCharCalc(this, SHOTSIDE_ENEMY) == 1){
				flash_count = BOSSFLASH_COUNT;
			}
		}

		if(life <= 0) {
			mychar->AddScore(score);	//�X�R�A���v���X
			efect.SetEfect(px+width/2+GAME_X,py+height/2, EFECT_SMALLBURN, height, GetColor(255,0,0));	//���j�G�t�F�N�g�Z�b�g
			sound.PlaySoundHandle(SOUNDEFECT_BOSSBURN);

			count = 0;
			phase = BPHASE_BRAKE;
			return 0;
		}
		break;

	case BPHASE_BRAKE:
		if(count >= 200){
			return 1;
		}
	}

	count++;
	if(flash_count > 0) flash_count--;
	return 0;
}

void Boss::Draw()
{
	if(px > GAME_HEIGHT){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (count%30)*6);	//�A���t�@�u�����h�ŕ`��
		_DrawBox((int)px, 440, (int)px+width, 480, GetColor(255,0,0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 126);
	}

	if(phase == BPHASE_BRAKE){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(255 - ((double)count/50)*255));
	}

	if(flash_count >= BOSSFLASH_COUNT/2) SetDrawBlendMode( DX_BLENDMODE_INVSRC, 255);

	//�������`���A�摜�̒��S�ɗ���悤�ɕ␳
	float draw_x = (float)px - (img_w/2 - width/2);
	float draw_y = (float)py - (img_h/2 - height/2);
	//

	image.DrawRotaImageF(draw_x+img_w/2, draw_y+img_h/2, 1.0, angle+PI/2, img_index, TRUE, img_div);

	if(phase == BPHASE_BRAKE) DrawBox(GAME_X,GAME_Y,GAME_X+GAME_WIDTH,GAME_Y+GAME_HEIGHT,GetColor(200,0,0),TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#ifdef DEBUG_CODE
	_DrawBox(int(px), int(py), int(px)+width, int(py)+height, GetColor(0,0,255),FALSE);
	//�������`�͈͂̕\��
#endif
}

bool Boss::Collition(double a_x0, int a_w, double a_y0, int a_h)
{
	if(col_type != ENECTYPE_NORMAL) return FALSE;
	if(phase != BPHASE_MAIN) return FALSE;

	double a_x1 = a_x0+a_w;	//����a�̉E��x
	double a_y1 = a_y0+a_h; //a�̉E��y
	double b_x0 = px;		//����b(=this)�̍���x
	double b_x1 = px+width;	//b�̉E��y
	double b_y0 = py;		//b�̍���y
	double b_y1 = py+height;//b�̉E��y

	if((a_x0 < b_x1) && (b_x0 < a_x1) && (a_y0 < b_y1) && (b_y0 < a_y1)){	//�����蔻�莮
		return TRUE;	//�������Ă�����TRUE��Ԃ�
	}

	return FALSE;
}


