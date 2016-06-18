#ifndef ITEM_DEF
#define ITEM_DEF

class Character;

class Item : public Character
{
private:
	int value;
	int count;

public :
	Item(double a_x, double a_y, int a_w, int a_h, int a_life, int a_imgnum, int a_value);

	int Update(MyCharacter* mychar);
	virtual void Draw();
};

#define EXISTITEM_MAX 100
class ItemMgr
{
private:
	Item* item[EXISTITEM_MAX];
public :
	ItemMgr();
	~ItemMgr();

	void Update(MyCharacter* mychar);
	void Draw();
	void SetItem(double a_x, double a_y, int a_value);
};

#endif