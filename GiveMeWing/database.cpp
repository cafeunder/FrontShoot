#include <DxLib.h>
#include "main.h"
#include "database.h"
#include "sht_ope.h"

ImgDataBase img_db;	//イメージデータベース定義
FontDataBase font_db;
EnemyDataBase ene_db;	//エネミーデータベース定義
ShotDataBase sht_db;	//ショットデータベース定義
StageDataBase stg_db;	//イメージデータベース定義
SoundDataBase sound_db;

//---------------------------
//|		画像データベース	|
//---------------------------

/*
特別な説明
	イメージハンドルについては、「Dxライブラリ 画像 ハンドル」でggrks

	このデータベースは、画像データパスと、
	その画像データに対するハンドルを格納するものである。
	
	通常、データベースはソースに直接書かれるが、
	ハンドルはDxライブラリの実行中にしか受け取れないので、
	データベースの値を-1で初期化しておき、
	プログラム実行中にロードして値を更新する。

	イメージハンドルを受け取るときは、クエリ番号に対応する定数を、
	クエリ関数に入れて呼び出せばよい。
*/

ImgData_t ImgData[IMGDATA_NUM] =	//イメージデータ（実体）
{//　画像ファイルパス		イメージハンドル格納部（-1固定）
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
	img_database = ImgData;	//イメージデータポインタを実体と関連付ける
}

void ImgDataBase::LoadImgData()
{	//この関数は、ゲームロード時にのみ呼び出される
	for(int i = 0; i < IMGDATA_NUM; i++){
		img_database[i].image.img_han = LoadGraph(img_database[i].img_name);
		//イメージデータをロードして、イメージハンドルを格納
	}
}

Image_t* ImgDataBase::QueryImgData(int index)
{
	assert(index >= 0);	//indexは0以上でなければならない
	assert(index < IMGDATA_NUM);	//indexはデータの総数値未満でなければならない

	return &(img_database[index].image);	//指定されたインデックスのイメージハンドルを返す
}



//-------------------------------
//|		フォントデータベース	|
//-------------------------------

FontData_t FontData[FONT_NUM] = 
{
	{"ＭＳ Ｐゴシック", 30,DEFAULT_FONT_THINCK,DX_FONTTYPE_ANTIALIASING_EDGE,-1},
	{"ＭＳ ゴシック", 22,5,DX_FONTTYPE_ANTIALIASING,-1},
	{"ＭＳ Ｐゴシック", 20, 4, DX_FONTTYPE_ANTIALIASING_EDGE, -1}
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
	assert(index >= 0);	//indexは0以上でなければならない
	assert(index < FONT_NUM);	//indexはデータの総数値未満でなければならない

	return fontdata[index].font_han;	//指定されたインデックスのイメージハンドルを返す
}

//-------------------------------
//|		エネミーデータベース	|
//-------------------------------

EnemyData_t EnemyData[ENEMYDATA_NUM] =	//エネミーデータ（実体）
{//　画像クエリ番号	幅　高さ 耐久	turn			item		exp	score	counter				col_type
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
	assert(index >= 0);	//indexは0以上でなければならない	
	assert(index < ENEMYDATA_NUM);	//indexはデータの総数値未満でなければならない

	return &ene_database[index];	//指定されたインデックスのエネミーデータを返す
}



//-------------------------------
//|		ショットデータベース	|
//-------------------------------

ShotData_t ShotData[SHOTDATA_NUM] =	//ショットデータ（実体）
{//　画像クエリ番号			幅　高さ 耐久 威力	貫通性		破棄条件			エフェクト
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
	assert(index >= 0);	//indexは0以上でなければならない
	assert(index < SHOTDATA_NUM);	//indexはデータの総数値未満でなければならない

	return &shot_database[index];	//指定されたインデックスのショットデータを返す
}



//-------------------------------
//|		ステージデータベース	|
//-------------------------------

StageMessageData_t Stage1_MessageData[] =
{
	{-1, "はじめまして。\nこれより作戦を展開します。",0,120},

	{10, "今回の作戦内容は、敵軍の海上基地襲撃です。",0,-1},
	{10, "敵の防衛ライン内を通過するので、\n敵襲は避けられないでしょう。",0,-1},
	{10, "撃墜されては元も子もありません。\n無事任務を遂行し、帰還してください。",0,-1},

	{2540,"後方より敵接近。\n画面下のアサートに注意してください。",0,-1},

	{5800,"敵の防衛ラインを\n突破しました。\n敵軍の海上基地に\n突入します。",0, -1},	//+300
	{-3,"",0,-1}
};

StageMessageData_t Stage2_MessageData[] =
{
	{-1, "敵の海上基地に突入しました。", 0,-1},

	{10, "襲撃の本命は基地内で停泊中の大型爆撃機です。",0,-1},
	{10, "これの撃破に成功すれば、戦況が大きく我が軍に傾くでしょう。",0,-1},
	{10, "敵軍も全力で守りを固めてくる筈です。どうか油断なさらずに。",0,-1},

	{2400,"前方の物体から爆発物反応を検出しました。",0,-1},
	{2400,"ショットに反応する機雷のようです。不用意に攻撃しないほうが良いでしょう。",0,200},

	{3250,"タイマー式爆破装置が接近しています。\n起爆する前に破壊してください。",0,-1},

	{5318,"緊急事態です。",0,-1},
	{5318,"停泊中と思われていた大型爆撃機が、\n部隊を携えて離陸しています。",0,210},
	{5318,"進路を変え、直ちに敵部隊を攻撃してください。",0,-1},

	{6198,"敵部隊の主要戦闘機が接近しています。\n警戒してください。",0,210},

	{-4,"司令部から連絡です。",0,-1},
	{-4,"基地を出発した敵部隊が、本国に向けて侵攻を開始しました。",0,200},
	{-4,"作戦は中止です。今後の指令に従ってください。",0,-1},
	
	{-3,"",0,-1}
};

StageMessageData_t Stage3_MessageData[] =
{
	{-1,"指令の変更をお伝えします。", 0,-1},

	{10,"敵軍の部隊は、現在\n境界を越えて侵攻中です。", 0,-1},
	{10,"直ちに本国へ帰還し、首都没落を防いでください。",0,-1},
	{10,"悪天候ですが、事態は一刻を争います。\nどうかご無事で。",0,-1},

	{1050,"周囲の電荷が増幅しています。",0,-1},
	{1050,"今後放電が予想されるエリアを表示しますので、回避行動をとってください。",0,-1},

	{2600,"レーダーに目視できない反応があります。おそらく光学兵器でしょう。",0,-1},
	{2600,"現在解析中です。\n可視化したところを狙ってください。",0,-1},

	{6998,"間もなく本国の首都に到着します。急いでください。",0,-1},
	{-3,"",0,-1},
};

StageMessageData_t Stage4_MessageData[] =
{
	{-1,"首都の防衛ラインに到着しました。",0,-1},

	{10,"現在、敵軍の侵攻部隊と交戦中です。",0,-1},
	{10,"敵の最新兵器に我が軍は劣勢です。このままでは首都陥落は免れません。",0,-1},
	{10,"今あなたの力が必要です。\nご武運をお祈りします。",0,-1},

	{1598,"エネルギー感知式特攻機が接近しています。",0,-1},
	{1598,"捕捉されると迎撃は困難です。チャージショットで倒すのが良いでしょう。",0,-1},

	{2798,"前方からレーザー出力を確認しました。軌道を良く見て回避してください。",0,-1},

	{6498,"大型爆撃機を前方に捉えました。攻撃を開始してください",0,-1},

	{-4,"敵軍の撤退を確認しました。",0,250},
	{-4,"これで、首都を防衛したのみならず、敵軍は大きな損害を受けたことでしょう。",0,220},
	{-4,"あなたのおかげで、戦局は我が軍の優勢になりました。",0,-1},
	{-4,"お疲れ様です。お気をつけて帰還してください。",0,-1},

	{-3,"",0,-1},
};

StageData_t StageData[STAGEST_STGNUM] =
{//　画像クエリ番号			後景スクロール速度		ファイルポインタ															ボスカウント	ボスデータ	
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
	assert(index >= 0);	//indexは0以上でなければならない
	assert(index < SHOTDATA_NUM);	//indexはデータの総数値未満でなければならない

	return &stage_database[index];	//指定されたインデックスのショットデータを返す
}



//-------------------------------
//|		サウンドデータベース	|
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
	assert(index >= 0);	//indexは0以上でなければならない
	assert(index < SOUNDDATA_NUM);	//indexはデータの総数値未満でなければならない

	return &sound_database[index];	//指定されたインデックスのショットデータを返す
}
