#ifndef CHARACTER_DEF
#define CHARACTER_DEF

class Character
{
protected:
	double px;	//����x
	double py;	//����y
	int width;	//�������`�̕�	
	int height; //�������`�̍���

	int img_index;//�C���[�W�n���h��
	int img_w;	//�摜��
	int img_h;	//�摜����
	int img_div;//�`��u���b�N��

	int life;	//�ϋv��

public:
	Character(){};
	Character(double a_x, double a_y, int a_w, int a_h, int a_life, int a_imgindex);
	void Initialize(){px=0;py=0;};

	virtual bool Damage(int damage);	//�_���[�W����
	virtual void Draw();	//�`��n
	virtual bool Collition(double a_x0, int a_w, double a_y0, int a_h);
		//�����蔻��

	double GetPosX(){return px;}
	double GetPosY(){return py;}
	void SetPosX(double a_x){px = a_x;}
	void SetPosY(double a_y){py = a_y;}
	void SetImgDiv(int a_div){img_div=a_div;}

	int GetWidth(){return width;}
	int GetHeight(){return height;}
	int GetImgWidth(){return img_w;}
	int GetImgHeight(){return img_h;}
	int GetLife(){return life;}
};

#endif