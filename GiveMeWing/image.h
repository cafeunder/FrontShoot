#ifndef IMAGE_DEF
#define IMAGE_DEF

class Image
{
public :
	void DrawImageF(float x, float y, int GrIndex, bool TransFlag, int div, bool adj = TRUE);
	void DrawImage(int x, int y, int GrIndex, bool TransFlag, int div, bool adj = TRUE);
	void DrawRotaImageF(float x, float y, double ExRate, double Angle, int GrIndex, bool TransFlag, int div, bool adj = TRUE, bool corner = FALSE);
	void DrawRotaImage(int x, int y, double ExRate, double Angle, int GrIndex, bool TransFlag, int div, bool adj = TRUE, bool corner = FALSE);
	void GetImageSize(int index, int* BufXSize, int* BufYSize);
};

extern Image image;

#endif