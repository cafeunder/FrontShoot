#ifndef SATELLITE_DEF
#define SATELLITE_DEF

class Character;
class MyCharacter;
class ShotMgr;
class EnemyMgr;

enum {SATE_ATK, SATE_GRD, SATE_NUM};

#define SATE_SIZE 1	//�T�e���C�g�̃T�C�Y
#define SATE_PARGE 350	//�p�[�W���ɃT�e���C�g���ł������鋗��
class Satellite : public Character
{
private:
	int type;	//�T�e���C�g�̃^�C�v
	int count;	//�J�E���^
	int shot_interval;	//�e�̔���
	bool smash;	//�X�}�b�V�������ǂ���
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