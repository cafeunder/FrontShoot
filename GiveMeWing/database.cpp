#include <DxLib.h>
#include "main.h"
#include "database.h"
#include "sht_ope.h"

ImgDataBase img_db;	//�C���[�W�f�[�^�x�[�X��`
FontDataBase font_db;
EnemyDataBase ene_db;	//�G�l�~�[�f�[�^�x�[�X��`
ShotDataBase sht_db;	//�V���b�g�f�[�^�x�[�X��`
StageDataBase stg_db;	//�C���[�W�f�[�^�x�[�X��`
SoundDataBase sound_db;

//---------------------------
//|		�摜�f�[�^�x�[�X	|
//---------------------------

/*
���ʂȐ���
	�C���[�W�n���h���ɂ��ẮA�uDx���C�u���� �摜 �n���h���v��ggrks

	���̃f�[�^�x�[�X�́A�摜�f�[�^�p�X�ƁA
	���̉摜�f�[�^�ɑ΂���n���h�����i�[������̂ł���B
	
	�ʏ�A�f�[�^�x�[�X�̓\�[�X�ɒ��ڏ�����邪�A
	�n���h����Dx���C�u�����̎��s���ɂ����󂯎��Ȃ��̂ŁA
	�f�[�^�x�[�X�̒l��-1�ŏ��������Ă����A
	�v���O�������s���Ƀ��[�h���Ēl���X�V����B

	�C���[�W�n���h�����󂯎��Ƃ��́A�N�G���ԍ��ɑΉ�����萔���A
	�N�G���֐��ɓ���ČĂяo���΂悢�B
*/

ImgData_t ImgData[IMGDATA_NUM] =	//�C���[�W�f�[�^�i���́j
{//�@�摜�t�@�C���p�X		�C���[�W�n���h���i�[���i-1�Œ�j
	{"DATA/img/orange.png",	{-1,3}},	//IMG_MYCHARACTER
	{"DATA/img/ene_weak.png", {-1,2}}, //IMG_ENEWEAK
	{"DATA/img/ene_weak2.png",{-1,2}},	//IMG_ENEWEAK2
	{"DATA/img/ene_weak3.png",{-1,2}},
	{"DATA/img/ene_weak4.png",{-1,2}},	//IMG_ENEWEAK4
	{"DATA/img/ene_weak5.png",{-1,2}},	//IMG_ENEWEAK4
	{"DATA/img/enemy_cn1.png", {-1,1}},		//IMG_ENEMY_1
	{"DATA/img/enemy_cn2.png",{-1,1}},
	{"DATA/img/enemy_cn3.png",{-1,1}},
	{"DATA/img/ene_st.png",{-1,2}},	//IMG_ENEST
	{"DATA/img/hex.png",{-1,1}},
	{"DATA/img/hex2.png",{-1,1}},
	{"DATA/img/head.png",{-1,1}},
	{"DATA/img/ene_midb.png",{-1,1}},	//IMG_ENEMIDB
	{"DATA/img/shot.png",		{-1,1}},	//IMG_SHOTNSHOT
	{"DATA/img/shot2.png",		{-1,1}},	//IMG_SHOTNSHOT2
	{"DATA/img/shot3.png",		{-1,1}},	//IMG_SHOTNSHOT3
	{"DATA/img/item.png", {-1,1}},			//IMG_ITEM
	{"DATA/img/sate.png", {-1,2}},
	{"DATA/img/sea.png", {-1,1}},	//IMG_BGSEA
	{"DATA/img/cloud.png",{-1,1}},	//IMG_BGCLOUD
	{"DATA/img/seabase.png", {-1,1}},	//IMG_BGSEABASE
	{"DATA/img/blick.png",{-1,1}},		//IMG_BGBLICK
	{"DATA/img/ncloud.png",{-1,1}},
	{"DATA/img/rainy.png",{-1,1}},
	{"DATA/img/night.png",{-1,1}},
	{"DATA/img/right.png",{-1,1}},
	{"DATA/img/board.png",{-1,1}},	//IMG_BOARD
	{"DATA/img/mesboard.png",{-1,1}},	//IMG_MESBOARD
	{"DATA/img/asrmesboard.png",{-1,1}},//IMG_ASRMESBOARD
	{"DATA/img/str_ready.png",{-1,1}},	//IMG_STRREADY
	{"DATA/img/str_stage1.png",{-1,1}},	//IMG_STRSTAGE1
	{"DATA/img/str_stage2.png",{-1,1}},	//IMG_STRSTAGE2
	{"DATA/img/str_stage3.png",{-1,1}},	//IMG_STRSTAGE3	
	{"DATA/img/str_stage4.png",{-1,1}},	//IMG_STRSTAGE3	
	{"DATA/img/str_gameover.png",{-1,1}},//IMG_STRGAMEOVER
	{"DATA/img/str_retry.png",{-1,1}},
	{"DATA/img/str_yesno.png",{-1,1}},
	{"DATA/img/str_gclear.png",{-1,1}},
	{"DATA/img/str_yours.png",{-1,1}},
	{"DATA/img/shot3rev.png",{-1,1}},	//IMG_SHOTNSHOTREV
	{"DATA/img/scorenum.png",{-1,1}},	//IMG_SCORENUM
	{"DATA/img/statenum.png",{-1,10}},	//IMG_STATENUM
	{"DATA/img/sate_box.png",{-1,4}},	//IMG_SATEBOX
	{"DATA/img/grdsate_br.png",{-1,1}},	//IMG_GRDSATEBR
	{"DATA/img/atk_sate_shot.png",{-1,1}},//IMG_ATKSATELAS
	{"DATA/img/str_shield.png",{-1,1}},	//IMG_STRSHIELD
	{"DATA/img/ene_sq.png",{-1,4}},
	{"DATA/img/ene_las.png",{-1,4}},
	{"DATA/img/ene_sc.png",{-1,1}},
	{"DATA/img/ene_lasen.png",{-1,1}},
	{"DATA/img/ene_lasen2.png",{-1,1}},
	{"DATA/img/atk_sate_smash.png",{-1,1}},
	{"DATA/img/grd_sate_smash.png",{-1,1}},
	{"DATA/img/ope_1.png",{-1,1}},
	{"DATA/img/str_stageclear.png",{-1,1}},
	{"DATA/img/str_leftbonus.png",{-1,1}},
	{"DATA/img/bonusnum.png",{-1,10}},
	{"DATA/img/str_nextstage.png",{-1,1}},
	{"DATA/img/display.png",{-1,1}},
	{"DATA/img/ene_bomb.png",{-1,2}},
	{"DATA/img/ene_laser.png",{-1,1}},
	{"DATA/img/ene_flame.png",{-1,2}},
	{"DATA/img/ene_shotm.png",{-1,1}},
	{"DATA/img/ene_rush.png",{-1,1}},
	{"DATA/img/ene_mad.png",{-1,2}},
	{"DATA/img/st2gim_d2.png",{-1,1}},
	{"DATA/img/ene_sm.png",{-1,4}},
	{"DATA/img/ene_en.png",{-1,4}},
	{"DATA/img/ene_sp.png",{-1,2}},
	{"DATA/img/ene_timer.png",{-1,6}},
	{"DATA/img/s2_boss.png",{-1,2}},
	{"DATA/img/site.png",{-1,2}},
	{"DATA/img/bossasrboard.png",{-1,1}},
	{"DATA/img/str_caution.png",{-1,2}},
	{"DATA/img/frame.png",{-1,1}},
	{"DATA/img/levelup.png",{-1,2}},
	{"DATA/img/leveldown.png",{-1,2}},
	{"DATA/img/menuback.png",{-1,1}},
	{"DATA/img/cursor.png",{-1,1}},
	{"DATA/img/titleback.png",{-1,1}},
	{"DATA/img/lightning.png",{-1,1}},
	{"DATA/img/str_stageselect.png",{-1,1}},
	{"DATA/img/sts_lmeasy.png",{-1,1}},
	{"DATA/img/sts_lmstand.png",{-1,1}},
	{"DATA/img/sts_lmhard.png",{-1,1}},
	{"DATA/img/sts_easy.png",{-1,1}},
	{"DATA/img/sts_stand.png",{-1,1}},
	{"DATA/img/sts_hard.png",{-1,1}},
	{"DATA/img/sts_st1back.png",{-1,1}},
	{"DATA/img/sts_st2back.png",{-1,1}},
	{"DATA/img/sts_st3back.png",{-1,1}},
	{"DATA/img/sts_st4back.png",{-1,1}},
	{"DATA/img/sts_frame.png",{-1,1}},
	{"DATA/img/sts_howto.png",{-1,1}},
	{"DATA/img/credit.png",{-1,1}},
	{"DATA/img/titleback2.png",{-1,1}},
	{"DATA/img/tobe.png",{-1,1}},
	{"DATA/img/boss4.png",{-1,2}},
	{"", {-1,1}}
};

ImgDataBase::ImgDataBase()
{
	img_database = ImgData;	//�C���[�W�f�[�^�|�C���^�����̂Ɗ֘A�t����
}

void ImgDataBase::LoadImgData()
{	//���̊֐��́A�Q�[�����[�h���ɂ̂݌Ăяo�����
	for(int i = 0; i < IMGDATA_NUM; i++){
		img_database[i].image.img_han = LoadGraph(img_database[i].img_name);
		//�C���[�W�f�[�^�����[�h���āA�C���[�W�n���h�����i�[
	}
}

Image_t* ImgDataBase::QueryImgData(int index)
{
	assert(index >= 0);	//index��0�ȏ�łȂ���΂Ȃ�Ȃ�
	assert(index < IMGDATA_NUM);	//index�̓f�[�^�̑����l�����łȂ���΂Ȃ�Ȃ�

	return &(img_database[index].image);	//�w�肳�ꂽ�C���f�b�N�X�̃C���[�W�n���h����Ԃ�
}



//-------------------------------
//|		�t�H���g�f�[�^�x�[�X	|
//-------------------------------

FontData_t FontData[FONT_NUM] = 
{
	{"�l�r �o�S�V�b�N", 30,DEFAULT_FONT_THINCK,DX_FONTTYPE_ANTIALIASING_EDGE,-1},
	{"�l�r �S�V�b�N", 22,5,DX_FONTTYPE_ANTIALIASING,-1},
	{"�l�r �o�S�V�b�N", 20, 4, DX_FONTTYPE_ANTIALIASING_EDGE, -1}
};

FontDataBase::FontDataBase()
{
	fontdata = FontData;
}

void FontDataBase::LoadFontData()
{
	for(int i = 0; i < FONT_NUM; i++){
		fontdata[i].font_han = CreateFontToHandle(fontdata[i].font_name,fontdata[i].size,fontdata[i].thick,fontdata[i].fonttype);
	}
}

int FontDataBase::QueryFontData(int index)
{
	assert(index >= 0);	//index��0�ȏ�łȂ���΂Ȃ�Ȃ�
	assert(index < FONT_NUM);	//index�̓f�[�^�̑����l�����łȂ���΂Ȃ�Ȃ�

	return fontdata[index].font_han;	//�w�肳�ꂽ�C���f�b�N�X�̃C���[�W�n���h����Ԃ�
}

//-------------------------------
//|		�G�l�~�[�f�[�^�x�[�X	|
//-------------------------------

EnemyData_t EnemyData[ENEMYDATA_NUM] =	//�G�l�~�[�f�[�^�i���́j
{//�@�摜�N�G���ԍ�	���@���� �ϋv	turn			item		exp	score	counter				col_type
	{IMG_ENEWEAK, 	50, 30,  5,		ENEDIR_FRONT,	3,	100,	ENECOUNTER_NOCOUNT,	ENECTYPE_NORMAL, TRUE, EF_ANIME,2},	//ENEMY_ZAKOWK
	{IMG_ENEWEAK2,  50, 30,  20,	ENEDIR_FRONT,	5,	150,	ENECOUNTER_NOCOUNT,	ENECTYPE_NORMAL, TRUE, EF_ANIME,2},	//ENEMY_ZAKOWK2
	{IMG_ENEWEAK3,	50, 30,	 40,	ENEDIR_FRONT,	10,	200,	ENECOUNTER_NOCOUNT,	ENECTYPE_NORMAL, TRUE, EF_ANIME,2},
	{IMG_ENEWEAK4,	50,	30,	 60,	ENEDIR_FRONT,	20, 300,	ENECOUNTER_NOCOUNT,	ENECTYPE_NORMAL, TRUE, EF_ANIME,2},
	{IMG_ENEWEAK5,	50,	50,	 80,	ENEDIR_FRONT,	30, 600,	ENECOUNTER_NOCOUNT,	ENECTYPE_NORMAL, TRUE, EF_ANIME,2},
	{IMG_ENEST, 	60, 40,  100,	ENEDIR_FRONT,	40,	800,	ENECOUNTER_NOCOUNT,	ENECTYPE_NORMAL, TRUE, EF_ANIME,2},	//ENEMY_ZAKOST
	{IMG_ENEMYCN1,	40, 40,  60,	ENEDIR_MYCHAR,	20,	300,	ENECOUNTER_NOCOUNT,	ENECTYPE_NORMAL, TRUE, EF_NOEFECT,-1},	//ENEMY_ZAKOCN
	{IMG_ENEMYCN2,	40, 40,	 80,	ENEDIR_MYCHAR,	30,	600,	ENECOUNTER_NOCOUNT,	ENECTYPE_NORMAL, TRUE, EF_NOEFECT,-1},	//ENEMY_ZAKOCN2
	{IMG_ENEMYCN3,	40,	40,	 100,	ENEDIR_MYCHAR,	40, 800,	ENECOUNTER_NOCOUNT,	ENECTYPE_NORMAL, TRUE, EF_NOEFECT,-1},
	{IMG_ENEHEX,	40, 40,	 60,	ENEDIR_FRONT,	20, 300,	ENECOUNTER_NOCOUNT,	ENECTYPE_NORMAL, TRUE, EF_NOEFECT,-1},
	{IMG_ENEHEX2,	40, 40,	 60,	ENEDIR_FRONT,	20, 300,	ENECOUNTER_NOCOUNT,	ENECTYPE_NORMAL, TRUE, EF_NOEFECT,-1},
	{IMG_ENEHEAD,	40, 40,	 80,	ENEDIR_FRONT,	30,	600,	ENECOUNTER_NOCOUNT,	ENECTYPE_NORMAL, TRUE, EF_NOEFECT,-1},

	{IMG_ENEMIDB,	120,90,	 250,	ENEDIR_FRONT,	60,	2000,	ENECOUNTER_NOCOUNT,	ENECTYPE_NORMAL, TRUE,EF_NOEFECT,-1},	//ENEMY_MIDBOSS
	{IMG_ENEWEAK,	50, 30,	 5,		ENEDIR_FRONT,	3,	10,		ENECOUNTER_NOCOUNT,	ENECTYPE_MYCHARNOCOL, TRUE,EF_NOEFECT,-1},	//ENEMY_STOPZAKO2
	{IMG_ENEWEAK2,	50, 30,	 20,	ENEDIR_FRONT,	1,	20,		ENECOUNTER_NOCOUNT,	ENECTYPE_MYCHARNOCOL, TRUE,EF_NOEFECT,-1},	//ENEMY_STOPZAKO2
	{IMG_ENEBOMB,	30, 30,  1,		ENEDIR_FRONT,	0,	0,		ENECOUNTER_DEAD,	ENECTYPE_MYCHARNOCOL, FALSE,EF_ANIME,20},
	{IMG_ENESHOTM,	50, 50,	 50,	ENEDIR_MYCHAR,	10,	300,	ENECOUNTER_NOCOUNT,	ENECTYPE_MYCHARNOCOL, TRUE,EF_NOEFECT,-1},
	{IMG_ENEMYCN1,	40, 40,  60,	ENEDIR_MYCHAR,	20,	300,	ENECOUNTER_NOCOUNT,	ENECTYPE_NORMAL, TRUE, EF_NOEFECT,-1},	//ENEMY_ZAKOCN
	{IMG_ENETIMER,	50, 50,	 125,	ENEDIR_NOTURN,	20, 1500,	ENECOUNTER_NOCOUNT,	ENECTYPE_NORMAL, TRUE,EF_NOEFECT,-1},
	{IMG_S2BOSS,	50, 250, 1,		ENEDIR_FRONT,	1,	10000,	ENECOUNTER_NOCOUNT,	ENECTYPE_NORMAL, TRUE,EF_NOEFECT,-1},
	{IMG_ST2GIMD2,	339,150, 1,		ENEDIR_NOTURN,	0,	0,	0,	ENECTYPE_ALLNOCOL,	FALSE, EF_NOEFECT, -1},
	{IMG_ENERUSH,	50, 50,  20,	ENEDIR_FRONT,	5,	150,	ENECOUNTER_NOCOUNT,	ENECTYPE_NORMAL, TRUE, EF_NOEFECT, -1},
	{IMG_ENEMAD,	50, 50,	 30,	ENEDIR_FRONT,	10,	200,	ENECOUNTER_NOCOUNT,	ENECTYPE_NORMAL, TRUE, EF_NOEFECT, -1},
	{IMG_DUMMY,		1,	1,	 1,		ENEDIR_NOTURN,	0,	0,	0,	ENECTYPE_ALLNOCOL, FALSE, EF_NOEFECT, -1}
};

EnemyDataBase::EnemyDataBase()
{
	ene_database = EnemyData;
}

EnemyData_t* EnemyDataBase::QueryEnemyData(int index)
{
	assert(index >= 0);	//index��0�ȏ�łȂ���΂Ȃ�Ȃ�	
	assert(index < ENEMYDATA_NUM);	//index�̓f�[�^�̑����l�����łȂ���΂Ȃ�Ȃ�

	return &ene_database[index];	//�w�肳�ꂽ�C���f�b�N�X�̃G�l�~�[�f�[�^��Ԃ�
}



//-------------------------------
//|		�V���b�g�f�[�^�x�[�X	|
//-------------------------------

ShotData_t ShotData[SHOTDATA_NUM] =	//�V���b�g�f�[�^�i���́j
{//�@�摜�N�G���ԍ�			���@���� �ϋv �З�	�ђʐ�		�j������			�G�t�F�N�g
	{IMG_SHOTNSHOT2,		20, 30,  1,   8,	SHOTTH_NOT,	SHOTDF_NORMAL, TRUE,	EF_NOEFECT,-1},//SHOT_NSHOT1
	{IMG_SHOTNSHOT2,		20, 30,  1,   6,	SHOTTH_NOT, SHOTDF_NORMAL, TRUE,	EF_NOEFECT,-1},//SHOT_NSHOT3_1
	{IMG_SHOTNSHOT3,		8,  30,  1,   2,	SHOTTH_NOT, SHOTDF_NORMAL, TRUE,	EF_NOEFECT,-1},//SHOT_NSHOT3_2
	{IMG_SHOTNSHOT2,		20, 30,  1,   3,	SHOTTH_NOT, SHOTDF_NORMAL, TRUE,	EF_NOEFECT,-1},//SHOT_NSHOT3_1
	{IMG_SHOTNSHOT3,		20, 30,  1,   1,	SHOTTH_NOT, SHOTDF_NORMAL, TRUE,	EF_NOEFECT,-1},//SHOT_NSHOT3_1
	{IMG_SHOTNSHOTREV, 		8,  8,   1,   1,	SHOTTH_NOT,	SHOTDF_NORMAL, TRUE,	EF_ADDBLEND,255},//SHOT_NSHOTREV
	{IMG_GRDSATEBR,			30, 30,	 1,   1,	SHOTTH_SHOT,SHOTDF_NORMAL, TRUE,	EF_NOEFECT,-1},//SHOT_SATEBAR
	{IMG_DUMMY,				380,480, 1,	  4,	SHOTTH_ALL, SHOTDF_NORMAL, TRUE,	EF_NOEFECT,-1},//SHOT_BOMB
	{IMG_SHOTNSHOT,			20, 30,	 1,	  30,	SHOTTH_ENEMY,SHOTDF_NORMAL, TRUE,	EF_NOEFECT,-1},
	{IMG_DUMMY,				130,130, 1,	  30,	SHOTTH_ENEMY,SHOTDF_NORMAL, TRUE,	EF_NOEFECT,-1},
	{IMG_DUMMY,				130,130, 1,	  30,	SHOTTH_ENEMY,SHOTDF_NORMAL, TRUE,	EF_NOEFECT,-1},
	{IMG_DUMMY,				130,130, 1,	  30,	SHOTTH_ENEMY,SHOTDF_NORMAL, TRUE,	EF_NOEFECT,-1},
	{IMG_ATKSATESHOT,		10, 10,	 1,	  1,	SHOTTH_NOT,	SHOTDF_NOUNDER, TRUE,	EF_ALPHABLEND,126},
	{IMG_ENESQ,				15, 15,  1,   1,	SHOTTH_NOT,	SHOTDF_NORMAL, TRUE,	EF_NOEFECT,-1}, //SHOT_ENESQ
	{IMG_ENELASER,			10, 10,	 1,	  1,	SHOTTH_NOT,	SHOTDF_NORMAL, TRUE,	EF_NOEFECT,255},//SHOT_ENELASER
	{IMG_ENESMALL,			10, 16,	 1,	  1,	SHOTTH_NOT,	SHOTDF_NORMAL, TRUE,	EF_NOEFECT,-1},
	{IMG_ENEEN,				15, 15,	 1,	  1,	SHOTTH_NOT,	SHOTDF_NORMAL, TRUE,	EF_NOEFECT,-1},
	{IMG_ENESP,				20, 20,	 1,	  1,	SHOTTH_NOT, SHOTDF_NORMAL, FALSE,	EF_ANIME,  4},
	{IMG_ATKSATESMASH,		16, 20,	 1,	  4,	SHOTTH_NOT,	SHOTDF_NOUNDER,TRUE,	EF_NOEFECT,255},
	{IMG_GRDSATESMASH,		120,120, 1,	  1,	SHOTTH_ALL,	SHOTDF_NORMAL, TRUE,	EF_NOEFECT,-1},
	{IMG_ENEBIGLASER,		30,30,	 1,	  1,	SHOTTH_NOT,	SHOTDF_NORMAL, FALSE,	EF_ADDBLEND,255},
	{IMG_ENEFLAME,			20,20,	 1,	  1,	SHOTTH_NOT,	SHOTDF_NOSIDE, TRUE,	EF_ALPHAMOTION,3},
	{IMG_ENEEN,				15, 15,	 1,	  1,	SHOTTH_NOT,	SHOTDF_NODEATH,TRUE,	EF_NOEFECT,-1},
	{IMG_ENESC,				15, 15,  1,   1,	SHOTTH_NOT,	SHOTDF_NORMAL, FALSE,	EF_NOEFECT,-1},
	{IMG_ENELASEN2,			20,	20,	 1,   1,	SHOTTH_NOT,	SHOTDF_NORMAL, TRUE,	EF_NOEFECT,255},
	{IMG_DUMMY,				200,480, 1,	  1,	SHOTTH_NOT,	SHOTDF_NORMAL, FALSE,	EF_NOEFECT,-1},
	{IMG_ENELASEN,			20, 20,	 1,	  1,	SHOTTH_NOT,	SHOTDF_NORMAL, FALSE,	EF_ADDBLEND,255},
	{IMG_ENEMIDB,			100,100, 30,  1,	SHOTTH_SHOTED,SHOTDF_NORMAL, FALSE,	EF_NOEFECT,-1},
};

ShotDataBase::ShotDataBase()
{
	shot_database = ShotData;
}

ShotData_t* ShotDataBase::QueryShotData(int index)
{
	assert(index >= 0);	//index��0�ȏ�łȂ���΂Ȃ�Ȃ�
	assert(index < SHOTDATA_NUM);	//index�̓f�[�^�̑����l�����łȂ���΂Ȃ�Ȃ�

	return &shot_database[index];	//�w�肳�ꂽ�C���f�b�N�X�̃V���b�g�f�[�^��Ԃ�
}



//-------------------------------
//|		�X�e�[�W�f�[�^�x�[�X	|
//-------------------------------

StageMessageData_t Stage1_MessageData[] =
{
	{-1, "�͂��߂܂��āB\n���������W�J���܂��B",0,120},

	{10, "����̍����e�́A�G�R�̊C���n�P���ł��B",0,-1},
	{10, "�G�̖h�q���C������ʉ߂���̂ŁA\n�G�P�͔������Ȃ��ł��傤�B",0,-1},
	{10, "���Ă���Ă͌����q������܂���B\n�����C���𐋍s���A�A�҂��Ă��������B",0,-1},

	{2540,"������G�ڋ߁B\n��ʉ��̃A�T�[�g�ɒ��ӂ��Ă��������B",0,-1},

	{5800,"�G�̖h�q���C����\n�˔j���܂����B\n�G�R�̊C���n��\n�˓����܂��B",0, -1},	//+300
	{-3,"",0,-1}
};

StageMessageData_t Stage2_MessageData[] =
{
	{-1, "�G�̊C���n�ɓ˓����܂����B", 0,-1},

	{10, "�P���̖{���͊�n���Œ┑���̑�^�����@�ł��B",0,-1},
	{10, "����̌��j�ɐ�������΁A�틵���傫���䂪�R�ɌX���ł��傤�B",0,-1},
	{10, "�G�R���S�͂Ŏ����ł߂Ă��锤�ł��B�ǂ������f�Ȃ��炸�ɁB",0,-1},

	{2400,"�O���̕��̂��甚�������������o���܂����B",0,-1},
	{2400,"�V���b�g�ɔ�������@���̂悤�ł��B�s�p�ӂɍU�����Ȃ��ق����ǂ��ł��傤�B",0,200},

	{3250,"�^�C�}�[�����j���u���ڋ߂��Ă��܂��B\n�N������O�ɔj�󂵂Ă��������B",0,-1},

	{5318,"�ً}���Ԃł��B",0,-1},
	{5318,"�┑���Ǝv���Ă�����^�����@���A\n�������g���ė������Ă��܂��B",0,210},
	{5318,"�i�H��ς��A�����ɓG�������U�����Ă��������B",0,-1},

	{6198,"�G�����̎�v�퓬�@���ڋ߂��Ă��܂��B\n�x�����Ă��������B",0,210},

	{-4,"�i�ߕ�����A���ł��B",0,-1},
	{-4,"��n���o�������G�������A�{���Ɍ����ĐN�U���J�n���܂����B",0,200},
	{-4,"���͒��~�ł��B����̎w�߂ɏ]���Ă��������B",0,-1},
	
	{-3,"",0,-1}
};

StageMessageData_t Stage3_MessageData[] =
{
	{-1,"�w�߂̕ύX�����`�����܂��B", 0,-1},

	{10,"�G�R�̕����́A����\n���E���z���ĐN�U���ł��B", 0,-1},
	{10,"�����ɖ{���֋A�҂��A��s�v����h���ł��������B",0,-1},
	{10,"���V��ł����A���Ԃ͈ꍏ�𑈂��܂��B\n�ǂ����������ŁB",0,-1},

	{1050,"���͂̓d�ׂ��������Ă��܂��B",0,-1},
	{1050,"������d���\�z�����G���A��\�����܂��̂ŁA����s�����Ƃ��Ă��������B",0,-1},

	{2600,"���[�_�[�ɖڎ��ł��Ȃ�����������܂��B�����炭���w����ł��傤�B",0,-1},
	{2600,"���݉�͒��ł��B\n���������Ƃ����_���Ă��������B",0,-1},

	{6998,"�Ԃ��Ȃ��{���̎�s�ɓ������܂��B�}���ł��������B",0,-1},
	{-3,"",0,-1},
};

StageMessageData_t Stage4_MessageData[] =
{
	{-1,"��s�̖h�q���C���ɓ������܂����B",0,-1},

	{10,"���݁A�G�R�̐N�U�����ƌ�풆�ł��B",0,-1},
	{10,"�G�̍ŐV����ɉ䂪�R�͗򐨂ł��B���̂܂܂ł͎�s�ח��͖Ƃ�܂���B",0,-1},
	{10,"�����Ȃ��̗͂��K�v�ł��B\n�����^�����F�肵�܂��B",0,-1},

	{1598,"�G�l���M�[���m�����U�@���ڋ߂��Ă��܂��B",0,-1},
	{1598,"�ߑ������ƌ}���͍���ł��B�`���[�W�V���b�g�œ|���̂��ǂ��ł��傤�B",0,-1},

	{2798,"�O�����烌�[�U�[�o�͂��m�F���܂����B�O����ǂ����ĉ�����Ă��������B",0,-1},

	{6498,"��^�����@��O���ɑ����܂����B�U�����J�n���Ă�������",0,-1},

	{-4,"�G�R�̓P�ނ��m�F���܂����B",0,250},
	{-4,"����ŁA��s��h�q�����݂̂Ȃ炸�A�G�R�͑傫�ȑ��Q���󂯂����Ƃł��傤�B",0,220},
	{-4,"���Ȃ��̂������ŁA��ǂ͉䂪�R�̗D���ɂȂ�܂����B",0,-1},
	{-4,"�����l�ł��B���C�����ċA�҂��Ă��������B",0,-1},

	{-3,"",0,-1},
};

StageData_t StageData[STAGEST_STGNUM] =
{//�@�摜�N�G���ԍ�			��i�X�N���[�����x		�t�@�C���|�C���^															�{�X�J�E���g	�{�X�f�[�^	
	{IMG_BGSEA,IMG_BGCLOUD,					5,	10,	"DATA/stage/stage1_e.dat", "DATA/stage/stage1_s.dat", "DATA/stage/stage1.dat",		5802,			-1,		SOUND_BGMSTAGE1,5545,Stage1_MessageData,  STAGEST_STAGE2,-1,-1}, //STAGEST_STAGE1
	{IMG_BGSEABASE,IMG_BGBLICK,				1,	2,	"DATA/stage/stage2_e.dat", "DATA/stage/stage2_s.dat", "DATA/stage/stage2_h.dat",6200,			BOSS_ST2,	SOUND_BGMSTAGE2,-1,Stage2_MessageData,  STAGEST_STAGE3,SOUND_BOSS,5320}, //STG_TEST2
	{IMG_BGNCLOUD,IMG_BGRAINY,				5,	8,	"DATA/stage/stage3_e.dat", "DATA/stage/stage3_s.dat", "DATA/stage/stage3_h.dat",7000,			-1,			SOUND_BGMSTAGE3,6700,Stage3_MessageData,  STAGEST_STAGE4,-1,-1}, //STG_TEST2
	{IMG_BGLIGHT,IMG_DUMMY,				7,	7,	"DATA/stage/stage4_e.dat", "DATA/stage/stage4_s.dat", "DATA/stage/stage4_h.dat",	6500,			BOSS_ST4,	SOUND_BGMSTAGE4,-1,Stage4_MessageData,  STAGEST_CLEAR,-1,-1}
};

StageDataBase::StageDataBase()
{
	stage_database = StageData;
}

StageData_t* StageDataBase::QueryStageData(int index)
{
	assert(index >= 0);	//index��0�ȏ�łȂ���΂Ȃ�Ȃ�
	assert(index < SHOTDATA_NUM);	//index�̓f�[�^�̑����l�����łȂ���΂Ȃ�Ȃ�

	return &stage_database[index];	//�w�肳�ꂽ�C���f�b�N�X�̃V���b�g�f�[�^��Ԃ�
}



//-------------------------------
//|		�T�E���h�f�[�^�x�[�X	|
//-------------------------------

SoundData_t SoundData[SOUNDDATA_NUM] =
{
	{"DATA/sound/1st.ogg",DX_PLAYTYPE_LOOP,-1},
	{"DATA/sound/2nd.ogg",DX_PLAYTYPE_LOOP,-1},
	{"DATA/sound/3rd.ogg",DX_PLAYTYPE_LOOP,-1},
	{"DATA/sound/noir_m.ogg",DX_PLAYTYPE_LOOP,-1},
	{"DATA/sound/boss.ogg",DX_PLAYTYPE_LOOP,-1},
	{"DATA/soundefect/gun09_r.wav",DX_PLAYTYPE_BACK,-1},
	{"DATA/soundefect/hit81.wav",DX_PLAYTYPE_BACK,-1},
	{"DATA/soundefect/alarm00.wav",DX_PLAYTYPE_BACK,-1},
	{"DATA/soundefect/cursor09.wav",DX_PLAYTYPE_BACK,-1},
	{"DATA/soundefect/cursor10.wav",DX_PLAYTYPE_BACK,-1},
	{"DATA/soundefect/shoot19.ogg",DX_PLAYTYPE_BACK,-1},
	{"DATA/soundefect/push59_b.ogg",DX_PLAYTYPE_BACK,-1},
	{"DATA/soundefect/shoot09.wav",DX_PLAYTYPE_BACK,-1},
	{"DATA/soundefect/gun23.wav",DX_PLAYTYPE_BACK,-1},
	{"DATA/soundefect/shoot16.wav",DX_PLAYTYPE_BACK,-1},
	{"DATA/soundefect/cursor05.ogg",DX_PLAYTYPE_BACK,-1},
	{"DATA/soundefect/shoot16_b.ogg",DX_PLAYTYPE_BACK,-1},
	{"DATA/soundefect/push55.ogg",DX_PLAYTYPE_BACK,-1},
	//{"DATA/soundefect/cursor32.wav",DX_PLAYTYPE_BACK,-1},
	{"DATA/soundefect/loop04.wav",DX_PLAYTYPE_LOOP,-1},
	{"DATA/soundefect/sha00.ogg",DX_PLAYTYPE_BACK,-1},
	{"DATA/soundefect/pi73.wav",DX_PLAYTYPE_BACK,-1},
	{"DATA/soundefect/hit62.ogg",DX_PLAYTYPE_BACK,-1},
	{"DATA/soundefect/gun22.wav",DX_PLAYTYPE_BACK,-1},
	{"DATA/soundefect/cursor36.ogg",DX_PLAYTYPE_BACK,-1},
	{"DATA/soundefect/kachi20.ogg",DX_PLAYTYPE_BACK,-1},
	{"DATA/soundefect/kachi23.ogg",DX_PLAYTYPE_BACK,-1},
	{"DATA/soundefect/kachi10.ogg",DX_PLAYTYPE_BACK,-1},
	{"DATA/soundefect/goro03.wav",DX_PLAYTYPE_BACK,-1}
};

SoundDataBase::SoundDataBase()
{
	sound_database = SoundData;
}

void SoundDataBase::LoadSoundData()
{
	for(int i = 0; i < SOUNDDATA_NUM; i++){
		sound_database[i].handle = LoadSoundMem(sound_database[i].file_name);
	}
}

SoundData_t* SoundDataBase::QuerySoundData(int index)
{
	assert(index >= 0);	//index��0�ȏ�łȂ���΂Ȃ�Ȃ�
	assert(index < SOUNDDATA_NUM);	//index�̓f�[�^�̑����l�����łȂ���΂Ȃ�Ȃ�

	return &sound_database[index];	//�w�肳�ꂽ�C���f�b�N�X�̃V���b�g�f�[�^��Ԃ�
}
