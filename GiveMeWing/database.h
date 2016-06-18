#ifndef DATABASE_DEF
#define DATABASE_DEF

enum {
	EF_NOEFECT,
	EF_ALPHABLEND,
	EF_ADDBLEND,
	EF_ANIME,
	EF_ALPHAMOTION
};

//---------------------------
//|		画像データベース	|
//---------------------------

enum IMGNUM_NAME{
	IMG_MYCHARACTER,//マイキャラ
	IMG_ENEWEAK,
	IMG_ENEWEAK2,
	IMG_ENEWEAK3,
	IMG_ENEWEAK4,
	IMG_ENEWEAK5,
	IMG_ENEMYCN1,		//キューブ
	IMG_ENEMYCN2,
	IMG_ENEMYCN3,
	IMG_ENEST,
	IMG_ENEHEX,
	IMG_ENEHEX2,
	IMG_ENEHEAD,
	IMG_ENEMIDB,
	IMG_SHOTNSHOT,
	IMG_SHOTNSHOT2,
	IMG_SHOTNSHOT3,
	IMG_ITEM,
	IMG_SATELLITE,
	IMG_BGSEA,			
	IMG_BGCLOUD,
	IMG_BGSEABASE,
	IMG_BGBLICK,
	IMG_BGNCLOUD,
	IMG_BGRAINY,
	IMG_BGNIGHT,
	IMG_BGLIGHT,
	IMG_BOARD,
	IMG_MESBOARD,
	IMG_ASRMESBOARD,
	IMG_STRREADY,
	IMG_STRSTAGE1,
	IMG_STRSTAGE2,
	IMG_STRSTAGE3,
	IMG_STRSTAGE4,
	IMG_STRGAMEOVER,
	IMG_STRRETRY,
	IMG_STRYESNO,
	IMG_STRGCLEAR,
	IMG_STRYOURS,
	IMG_SHOTNSHOTREV,
	IMG_SCORENUM,
	IMG_STATENUM,
	IMG_SATEBOX,
	IMG_GRDSATEBR,
	IMG_ATKSATESHOT,
	IMG_STRSHIELD,
	IMG_ENESQ,
	IMG_ENELASER,
	IMG_ENESC,
	IMG_ENELASEN,
	IMG_ENELASEN2,
	IMG_ATKSATESMASH,
	IMG_GRDSATESMASH,
	IMG_OPE1,
	IMG_STRSTAGECLEAR,
	IMG_STRLEFTBONUS,
	IMG_BONUSNUM,
	IMG_STRNEXTSTAGE,
	IMG_DISPLAY,
	IMG_ENEBOMB,
	IMG_ENEBIGLASER,
	IMG_ENEFLAME,
	IMG_ENESHOTM,
	IMG_ENERUSH,
	IMG_ENEMAD,
	IMG_ST2GIMD2,
	IMG_ENESMALL,
	IMG_ENEEN,
	IMG_ENESP,
	IMG_ENETIMER,
	IMG_S2BOSS,
	IMG_SITE,
	IMG_BOSSADRBOARD,
	IMG_STRCAUTION,
	IMG_DISPLAYFRAME,
	IMG_LEVELUP,
	IMG_LEVELDOWN,
	IMG_MENUBACK,
	IMG_CURSOR,
	IMG_TITLEBACK,
	IMG_THUNDER,
	IMG_STRSTAGESELECT,

	IMG_STSLMEASY,
	IMG_STSLMSTAND,
	IMG_STSLMHARD,
	IMG_STSBACKEASY,
	IMG_STSBACKSTAND,
	IMG_STSBACKHARD,
	IMG_STSBACKST1,
	IMG_STSBACKST2,
	IMG_STSBACKST3,
	IMG_STSBACKST4,
	IMG_STSFRAME,
	IMG_STSHOWTO,
	IMG_CREDIT,
	IMG_TITLEBACK2,
	IMG_TOBE,
	IMG_S4BOSS,
	IMG_DUMMY,
	IMGDATA_NUM
};

typedef struct
{
	int img_han;
	int div_num;
} Image_t;

typedef struct
{
	const char* img_name;
	Image_t image;
} ImgData_t;

class ImgDataBase
{
private:
	ImgData_t* img_database;

public :
	ImgDataBase();

	void LoadImgData();
	Image_t* QueryImgData(int index);
};

extern ImgDataBase img_db;	//イメージデータベース宣言



//-------------------------------
//|		フォントデータベース	|
//-------------------------------

enum {
	FONT_BONUS,
	FONT_MESSAGE,
	FONT_MENU,
	FONT_NUM
};

typedef struct
{
	const char* font_name;
	const int size;
	const int thick;
	const int fonttype;
	int font_han;
} FontData_t;

class FontDataBase
{
private:
	FontData_t* fontdata;
public :
	FontDataBase();

	void LoadFontData();
	int QueryFontData(int index);
};

extern FontDataBase font_db;

//-------------------------------
//|		エネミーデータベース	|
//-------------------------------

enum {
	ENEMY_ZAKOWK,
	ENEMY_ZAKOWK2,
	ENEMY_ZAKOWK3,
	ENEMY_ZAKOWK4,
	ENEMY_ZAKOWK5,
	ENEMY_ZAKOST,
	ENEMY_ZAKOCN,
	ENEMY_ZAKOCN2,
	ENEMY_ZAKOCN3,
	ENEMY_ZAKOHEX,
	ENEMY_ZAKOHEX2,
	ENEMY_ZAKOHEAD,

	ENEMY_MIDBOSS,
	ENEMY_STOPZAKO,
	ENEMY_STOPZAKO2,
	ENEMY_BOMB,
	ENEMY_SHOTM,
	ENEMY_SATEBOX,
	ENEMY_TIMER,
	ENEMY_S2BOSS,
	ST2GIM_D2,
	ENEMY_RUSH,
	ENEMY_MAD,
	ENEMY_GOAST,
	ENEMYDATA_NUM
};

enum {
	ENEDIR_NOTURN,
	ENEDIR_FRONT,
	ENEDIR_MYCHAR
};


enum {ENECTYPE_NORMAL, ENECTYPE_ALLNOCOL, ENECTYPE_MYCHARNOCOL};
enum {ENECOUNTER_NOCOUNT, ENECOUNTER_COLSHOT, ENECOUNTER_DEAD};

typedef struct 
{
	const int img_num;
	const int width;
	const int height;
	const int life;
	const int turn;
	//const int item;
	const int value;
	const int score;
	const int counter;
	const int col_type;
	const bool homed;
	const int efect;	
	const int efe_par;
} EnemyData_t;

class EnemyDataBase
{
private:
	EnemyData_t* ene_database;

public :
	EnemyDataBase();

	EnemyData_t* QueryEnemyData(int index);
};

extern EnemyDataBase ene_db;	//イメージデータベース宣言



//-------------------------------
//|		ショットデータベース	|
//-------------------------------

enum {
	SHOT_NSHOT1,
	SHOT_NSHOT2_1,
	SHOT_NSHOT2_2,
	SHOT_NSHOT3_1,
	SHOT_NSHOT3_2,
	SHOT_NSHOT3_REV,
	SHOT_SATEBAR,
	SHOT_BOMB,
	SHOT_CHARGESHOT,
	SHOT_CSHOTBURN1,
	SHOT_CSHOTBURN2,
	SHOT_CSHOTBURN3,
	SHOT_SATEATKSHOT,
	SHOT_ENESQ,
	SHOT_ENELASER,
	SHOT_ENESMALL,
	SHOT_ENEEN,
	SHOT_ENESP,
	SHOT_SATEATKSMASH,
	SHOT_SATEGRDSMASH,
	SHOT_ENEBIGLASER,
	SHOT_ENEFLAME,
	SHOT_ST2BOSS,
	SHOT_ENECOUNTERSHOT,
	SHOT_ENELASEN,
	SHOT_THUNDER,
	SHOT_ENELASENF,
	SHOT_BARRIER,
	SHOTDATA_NUM
};

enum {
	SHOTTH_ENEMY,
	SHOTTH_SHOT,
	SHOTTH_ALL,
	SHOTTH_NOT,
	SHOTTH_SHOTED,
};

enum {
	SHOTDF_NORMAL,
	SHOTDF_NOSIDE,
	SHOTDF_NOUNDER,
	SHOTDF_NODEATH
};

typedef struct 
{
	const int img_num;
	const int width;
	const int height;
	const int life;
	const int power;
	const int through;
	const int death_type;
	const bool graze; 
	const int efect;
	const int efect_par;
} ShotData_t;

class ShotDataBase
{
private:
	ShotData_t* shot_database;

public :
	ShotDataBase();

	ShotData_t* QueryShotData(int index);
};

extern ShotDataBase sht_db;	//イメージデータベース宣言



//-------------------------------
//|		ステージデータベース	|
//-------------------------------

enum {
	STAGEST_STAGE1,
	STAGEST_STAGE2,
	STAGEST_STAGE3,
	STAGEST_STAGE4,
	STAGEST_STGNUM,
	STAGEST_CLEAR
};

#define STAGEMES_MAXNUM 30

typedef struct
{
	const int count;
	const char message[73];
	const int operater_type;
	const int brake_count;
} StageMessageData_t;

enum{ BOSS_ST2, BOSS_ST4, BOSS_NUM};

typedef struct
{
	const int bgimg_index;//後景画像クエリ番号
	const int fgimg_index;//前景画像クエリ番号
	const int b_velocity;	//スクロール速度
	const int f_velocity;
	const char* efile_name;	//ステージデータのファイルポインタ
	const char* sfile_name;	//ステージデータのファイルポインタ
	const char* hfile_name;	//ステージデータのファイルポインタ
	const int boss_count;
	const int boss_index;
	const int sound_index;
	const int sfade_count;
	StageMessageData_t* mes_data;
	const int next_state;
	const int change_sound;
	const int schange_count;
} StageData_t;

class StageDataBase
{
private:
	StageData_t* stage_database;

public :
	StageDataBase();

	StageData_t* QueryStageData(int index);
};

extern StageDataBase stg_db;	//イメージデータベース宣言



//-------------------------------
//|		サウンドデータベース	|
//-------------------------------

enum {
	SOUND_BGMSTAGE1,
	SOUND_BGMSTAGE2,
	SOUND_BGMSTAGE3,
	SOUND_BGMSTAGE4,
	SOUND_BOSS,
	SOUNDEFECT_ENEBURN,
	SOUNDEFECT_BOSSBURN,
	SOUNDEFECT_CAUTION,
	SOUNDEFECT_LEVELUP,
	SOUNDEFECT_LEVELDOWN,
	SOUNDEFECT_SHOT,
	SOUNDEFECT_CHARGE,
	SOUNDEFECT_CHARGESHOT,
	SOUNDEFECT_CHARGEBURN,
	SOUNDEFECT_SITE,
	SOUNDEFECT_ITEMGAIN,
	SOUNDEFECT_ENEASERT,
	SOUNDEFECT_OPETYPE,
	//SOUNDEFECT_OPESTART,
	SOUNDEFECT_STARTEFECT,
	SOUNDEFECT_DASH,
	SOUNDEFECT_TBOMBSECOND,
	SOUNDEFECT_MYCHARDAMAGE,
	SOUNDEFECT_ENELASER,
	SOUNDEFECT_GRAZE,
	SOUNDEFECT_MENUCURSOR,
	SOUNDEFECT_MENUDECIDE,
	SOUNDEFECT_MENUCANCEL,
	SOUNDEFECT_THUNDER,
	SOUNDDATA_NUM
};

typedef struct
{
	char* file_name;
	int playtype;
	int handle;
} SoundData_t;

class SoundDataBase
{
private:
	SoundData_t* sound_database;

public :
	SoundDataBase();
	void LoadSoundData();

	SoundData_t* QuerySoundData(int index);
};

extern SoundDataBase sound_db;

#endif