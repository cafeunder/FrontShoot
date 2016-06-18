#ifndef STAGE_DEF
#define STAGE_DEF

class ShotMgr;
class MyCharacter;
class ItemMgr;
class EnemyMgr;
class BackGround;
class OperateArea;
class Boss;
StageMessageData_t;

class Stage
{
private:
	Boss* boss;
	ShotMgr* sht_mgr;	//ショットマネージャ
	ItemMgr* item_mgr;	//アイテムマネージャ
	EnemyMgr* ene_mgr;
	BackGround* back_g;
	StageMessageData_t* mes_data;

	int game_level;
	int stage_num;
	int boss_index;
	int boss_count;
	int boss_result;
	int sound_volume;

	int sound_index;
	int sfade_count;

	int change_sound;
	int schange_count;
	int scfade_count;

	int count;

public :
	Stage(int stg_index, OperateArea* ope_area, int a_level);
	~Stage();

	int Update(MyCharacter* mychar, OperateArea* ope_area);
	void Draw(MyCharacter* mychar, int d_count);

	void DrawBackGround(int count);
	void StopStageSound();
};

#endif