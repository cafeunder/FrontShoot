#ifndef INPUT_DEF
#define INPUT_DEF

enum { INPUT_UP, INPUT_DOWN, INPUT_RIGHT, INPUT_LEFT, INPUT_SHOT, INPUT_SLOW, INPUT_LEFTDASH, INPUT_RIGHTDASH, INPUT_SATESMASH, INPUT_SATECHANGE, INPUT_MENU, INPUT_NUM};

class Input
{
private:
	int input[INPUT_NUM];
public :
	Input(){for(int i = 0; i < INPUT_NUM; i++) input[i] = 0;};
	void Update();
	int GetInput(int code);
};

extern Input input;

#endif