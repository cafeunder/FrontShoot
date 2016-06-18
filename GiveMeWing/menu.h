#ifndef MENU_DEF
#define MENU_DEF

enum { MENUST_START, MENUST_MAIN, MENUST_END};

class Menu
{
private:
	int px;
	int py;
	int height;
	int width;

	char** item;
	int num;

	int index;
	int move_count;

	bool loop;
	bool box;
	bool cancel;

	int stat;
	int count;
	int result;

	int StartUpdate();
	void StartDraw();

	int MainUpdate();
	void MainDraw();

	int EndUpdate();
	void EndDraw();

public :
	Menu(int ax, int ay, char** item, int a_num, bool a_loop, bool a_box, bool a_cancel);
	~Menu();

	int Update();
	void Draw();
};

#endif