#ifndef MYCHAR_DEF
#define MYCHAR_DEF

#include "character.h"

class EnemyMgr;
class ShotMgr;
class Satellite;
class Boss;

#define MYSHOT_MAXCHARGE 100	//�`���[�W�̍ő�l
#define MYSHOT_MAXLEVEL 4	//�`���[�W�̍ő�l
#define RATE_MAX 99.9	//���[�g�̍ő�l
#define LEFT_BONUS 10000

class MyCharacter : public Character
{
private:
	Satellite* sate[2];

	int g_width;	//�����蔻�蕝
	int g_height;	//�����蔻�荂��

	double rate;	//�|�C���g�{��
	int point;	//�|�C���g
	int score;	//�X�R�A
	int charge;	//�`���[�W

	int invin_count;	//���G����
	int dash_count;		//�_�b�V���J�E���g
	double dash_invin;	//�_�b�V�����̖��G����
	int shot_exp;		//�V���b�g�o���l
	int shot_level;		//�V���b�g���x��
	int interval_count;	//�V���b�g�x�ݒ��J�E���^

	void SetSatellite(double ax, double ay, int num);
	void DrawSatellite();	//�T�e���C�g�̕`��
	void UpdateSatellite(ShotMgr* sht_mgr = NULL, EnemyMgr* ene_mgr = NULL);
		//�T�e���C�g�̍X�V

	void Move();	//�ړ�����
	int Shot(ShotMgr* sht_mgr); //�V���b�g����

public :
	MyCharacter(double a_x, double a_y, int a_w, int a_h, int a_gw, int a_gh, int a_life, int a_imgnum);
	~MyCharacter();
	void Initialize();

	int Update();
	int Update(EnemyMgr* ene_mgr,ShotMgr* sht_mgr, Boss* boss); //���͂ɑ΂����Ԃ̍X�V
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