#ifndef GAME_DEF
#define GAME_DEF

class OperateArea;
class Stage;
class MyCharacter;
class Menu;

enum {GAME_START, GAME_MAIN, GAME_OVER, GAME_RETRY, STAGE_CLEAR, NEXT_STATE, GAME_CLEAR};

class Game
{
private:
	OperateArea* ope_area;	//オペレートエリア
	Stage* stage;	//ステージ
	MyCharacter* mychar;	//マイキャラ
	Menu* pause;

	int game_sta;	//現在状態
	int game_level;
	int stage_state;//ステージ数
	int count;	//ゲームカウンタ
	int f_count;
	bool through;

	int StartUpdate();
	void StartDraw();
	int MainUpdate();
	void MainDraw();
	int OverUpdate();
	void OverDraw();

	class GameClear{
	private:
		int count;
		int score;

	public :
		GameClear(int a_scr){count=0;score=a_scr;}
		int Update();
		void Draw();
	};
	GameClear* g_clear;

	class MyRetry{
	private:
		int select;
		int count;
		bool decide;
	public :
		MyRetry();
		int Update();
		void Draw();
	};
	MyRetry* retry;

	int StageClearUpdate();
	void StageClearDraw();

	void DrawState();
	void DrawSystem();
	void DrawScoreLife();

public :
	Game(int stg_index, int a_glevel, bool a_thr);
	~Game();

	void Retry();

	int Update();
	void Draw();
};

#endif