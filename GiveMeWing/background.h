#ifndef BACKGROUND_DEF
#define BACKGROUND_DEF

class BackGround
{
private:
	int backimg_index;
	int frontimg_index;
	int bimg_height;
	int fimg_height;
	int b_velocity;
	int f_velocity;

public :
	BackGround(int fimg_index, int bimg_index, int f_velo, int b_velo);

	void FrontDraw(int count);
	void BackDraw(int count);
};

#endif