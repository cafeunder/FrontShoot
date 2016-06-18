#ifndef MESSAGE_DEF
#define MESSAGE_DEF

#define STRING_MAXNUM 10 //予約できる最大メッセージ数
#define MESLINE_MAXNUM (10*2) //一行に表示できる最大半角文字数
#define MESBOX_LINEMAX	4	//ボックスの最大行数
#define MESBOX_MAXNUM (MESLINE_MAXNUM*MESBOX_LINEMAX+1)//ボックスに表示できる最大半角文字数+1(終端文字


class MesBox
{
private:
	char* string[STRING_MAXNUM];
	int string_brake[STRING_MAXNUM];

	char draw_mes[MESBOX_MAXNUM];
	int brake_count;
	int interval_count;
	int index;

	int StrUpdate();
public :
	MesBox();
	~MesBox();

	int SetMessage(const char* str, int str_brake);
	void Draw();
	int Update();
	void Reset();
};

enum {OPEAREA_SHUTDOWN, OPEAREA_STANDBY, OPEAREA_STANDING};
class OperateArea
{
private:
	MesBox mesbox;
	int operate_type[STRING_MAXNUM];

	int state;
	int count;

	bool draw_mes;

public :
	OperateArea(){state=OPEAREA_SHUTDOWN;count=0;draw_mes=FALSE;for(int i=0; i<STRING_MAXNUM; i++) operate_type[i]=-1;}

	void StartUp(){state=OPEAREA_STANDBY;count=0;}

	void Update();
	void Draw();

	bool GetDrawMes(){return draw_mes;}

	void SetMessage(const char* str, int operater_type, int str_brake = -1);
	void Reset();
};

#endif