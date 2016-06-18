#ifndef EFECT_DEF
#define EFECT_DEF

#define FASTCALL_COUNT 90
#define SLOWCALL_COUNT 210

enum {
	EFECT_SMALLBURN,
	EFECT_SMALLBURNFAST,
	EFECT_SATEBURN,
	EFECT_SCORE,
	EFECT_BONUS,
	EFECT_STRCALLFAST,
	EFECT_STRCALLSLOW,
	EFECT_ALPHAMOTION,
	EFECT_MYCHARSITE,
	EFECT_STRCALLCAUTION,
	EFECT_LEVELUPDOWN,
	EFECT_LASEREFECT,
	EFECT_THUNDER,
	EFECT_LIGHTNING,
	EFECT_ASSERT,
	EFECT_NUM
};

class Efect
{
private:
	double px;
	double py;

	int option1;
	int option2;
	double option3;

	int count;
	int type;

public :
	Efect(double a_px, double a_py, int a_type, int a_op1, int a_op2, double a_op3){px=a_px;py=a_py;type=a_type;count=0;option1=a_op1;option2=a_op2;option3=a_op3;};
	
	int Update();

	double GetPosX(){return px;}
	double GetPosY(){return py;}
	double SetPosX(double a_x){px=a_x;}
	double SetPosY(double a_y){py=a_y;}
	int GetCount(){return count;}
	int GetOption1(){return option1;}
	int GetOption2(){return option2;}
	double GetOption3(){return option3;}
};

#define EFECT_NUM 100
class EfectMgr
{
private:
	Efect* efe[EFECT_NUM];
public :
	EfectMgr();
	~EfectMgr();

	void Update();
	void SetEfect(double px, double py, int type, int option1 = -1, int option2 = -1, double option3 = -1.0);
};

extern EfectMgr efect;

#endif