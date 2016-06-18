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
	double x0;	//����x
	double y0;	//����y
	int encount;//�����J�E���g
	int shot_interval;	//�V���b�g�Ԋu
	int shot_index;		//�V���b�g�f�[�^�C���f�b�N�X
	int shotmove_index;//�V���b�g����C���f�b�N�X
	int shot_type;		//�V���b�g����^�C�v
	double shot_velo;		//�V���b�g���x
	double velocity;		//�ړ����x
	double angle;
	int move_type;		//�ړ�����^�C�v
	int brake_count;	//�ҋ@����
	int data_index;		//�f�[�^�C���f�b�N�X
} EnemyInitData_t;

class Enemy : public Character
{
private:
	int state;	//���
	int count;	//�J�E���^
	bool damaged;

	int encount;	//�����J�E���^��
	int shot_interval;	//�V���b�g�����Ԋu
	int shot_index;		//�V���b�g�f�[�^�C���f�b�N�X
	int shotmove_index;//�V���b�g����C���f�b�N�X
	double shot_velo;	//�V���b�g���x
	int brake_count;	//�ҋ@����
	double velocity;		//�ړ����x
	double angle;	//�p�x
	int score;		//������X�R�A
	int item_value;		//���Ƃ��o���l
	int turn;		//�摜�̊p�x���ǂ̂悤�ɕς��邩

	int draw_efect;
	int efect_par;
	int rapid_num;	//�A����
	int interval_count;	//�V���b�g�Ԋu�J�E���g

	int flash_count;
	int move_type;	//�ړ��^�C�v
	int shot_type;	//�V���b�g�^�C�v
	int counter;
	bool homed;
	int col_type;

	void Shot(ShotMgr* sht_mgr, MyCharacter* mychar, bool count_shot = FALSE);

public :
	Enemy(double a_x, double a_y, int a_enc,int a_sinterval,int s_index, int sm_index, int a_brake, double a_velo, int a_move, int a_shot, double a_svelo, double a_angle, int data_index);

	void Update(ShotMgr* sht_mgr, MyCharacter* mychar, ItemMgr* item_mgr, int g_level);	//��Ԃ̍X�V�E�����m�F
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
	Enemy** enemy;	//�G�l�~�[�|�C���^�z��
	int ene_num;	//�������Ă���G�l�~�[��

public :
	EnemyMgr(int num, EnemyInitData_t* init_data);	//�R���X�g���N�^
	~EnemyMgr();

	void Update(ShotMgr* sht_mgr, MyCharacter* mychar, ItemMgr* item_mgr, int count, int game_level);	//�����o�̏�ԍX�V

	bool Collition(int a_x, int a_w, int a_y, int a_h);	//�����蔻��
	void Draw(int count, MyCharacter* mychar);		//�`��
	void OnTheDraw(int count, MyCharacter* mychar);

	void QueryNearEnemyPos(double px, double py, double* bf_x, double* bf_y);
};

#endif