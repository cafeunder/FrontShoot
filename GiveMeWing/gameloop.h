#ifndef LOOP_DEF
#define LOOP_DEF

enum {GAMELOOP_TITLE, GAMELOOP_STAGESELECT, GAMELOOP_CREDIT, GAMELOOP_MAIN, GAMELOOP_GAMECLEAR, GAMELOOP_FINAL};

class Game;
class Menu;

class GameLoop
{
private:
	Game* game;	//ゲーム本体オブジェクト

	class Title{
	private:
		Menu* menu;
		int count;
		int result;
	public :
		Title();
		~Title();
		int Update();
		void Draw();
	};
	Title* title;

	class StageSelect{
	private:
		int stage_num;
		int game_level;
		int count;
		bool decide;

		int move_count;
		int fade_count;
	public :
		StageSelect();
		int Update(Game** game);
		void Draw();
	};
	StageSelect* st_select;

	class Credit{
	private:
		int count;
	public :
		Credit(){count=0;}
		int Update();
		void Draw();
	};
	Credit* credit;

	int g_stat;	//ゲームループ状態
	int m_result;
	int m_count;

	int Main();	//ゲーム本体

public :
	GameLoop();
	~GameLoop();
	int Loop();	//ゲームループ
};


#endif
