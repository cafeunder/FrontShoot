#include <DxLib.h>
#include "image.h"
#include "main.h"
#include "gameloop.h"
#include "game.h"
#include "database.h"
#include "input.h"
#include "menu.h"
#include "sound.h"

/*
�N���X�� : GameLoop
	�Q�[�����[�v

	�L�q�t�@�C�� : gameloop.h
*/

GameLoop::GameLoop()
{
	title = new Title();
	g_stat = GAMELOOP_TITLE;

	st_select = NULL;
	credit = NULL;
	game = NULL;
}

GameLoop::~GameLoop()
{
	delete credit;
	delete game;
	delete title;
	delete st_select;
}


#define TITLEMENU_NUM 4
char* TitleMenu[TITLEMENU_NUM] = {"�Q�[���X�^�[�g","�X�e�[�W�Z���N�g","�N���W�b�g","�V���b�g�_�E��"};
GameLoop::Title::Title()
{
	menu = new Menu(400, 300, TitleMenu, TITLEMENU_NUM, FALSE, FALSE, FALSE);
	result = -1;
	count = 0;
}

GameLoop::Title::~Title()
{
	delete menu;
}

#define TITLEFADE_COUNT 60
int GameLoop::Title::Update()
{
	if(result == -1){
		switch(menu->Update()){
		case 0 :
			result = 0; break;
		case 1 :
			result = 1; break;
		case 2 :
			result = 2; break;
		case 3 :
			result = -2; break;
		}
	}

	if(result != -1) count++;
	if(count == TITLEFADE_COUNT) return result;

	return -1;
}

void GameLoop::Title::Draw()
{
	image.DrawImage(0,0,IMG_TITLEBACK2,TRUE,0,FALSE);
	image.DrawImage(490,0,IMG_TITLEBACK,TRUE,0,FALSE);
	menu->Draw();

	if(result != -1){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(255*((double)count/TITLEFADE_COUNT)));
		DrawBox(0,0,640,480,GetColor(0,0,0),TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}



GameLoop::StageSelect::StageSelect()
{
	stage_num = STAGEST_STAGE1;
	game_level = 1;

	count = 0;
	move_count = 0;
	fade_count = 0;
	decide = FALSE;
}

#define STS_MOVEINTERVAL 10
int GameLoop::StageSelect::Update(Game** game)
{
	if(decide){
		if(fade_count > 0) {fade_count--; return 0;}

		*game = new Game(stage_num, game_level, FALSE);
		return 1;
	}
	if(move_count > 0) {move_count--; return 0;}

	if(input.GetInput( INPUT_RIGHT ) == 1 && !(input.GetInput( INPUT_RIGHTDASH ) == 1)){
		stage_num++;
		if(stage_num > STAGEST_STGNUM-1) stage_num = STAGEST_STGNUM-1;
		else sound.PlaySoundHandle(SOUNDEFECT_MENUCURSOR);
	}
	if(input.GetInput( INPUT_LEFT ) == 1 && !(input.GetInput( INPUT_LEFTDASH ) == 1)){
		stage_num--;
		if(stage_num < 0) stage_num = 0;
		else sound.PlaySoundHandle(SOUNDEFECT_MENUCURSOR);
	}
	if(input.GetInput( INPUT_RIGHTDASH ) == 1){
		game_level++;
		if(game_level > 2) game_level = 2;
		else {
			sound.PlaySoundHandle(SOUNDEFECT_DASH);
			move_count = STS_MOVEINTERVAL;
		}
	}
	if(input.GetInput( INPUT_LEFTDASH ) == 1){
		game_level--;
		if(game_level < 0) game_level = 0;
		else {
			sound.PlaySoundHandle(SOUNDEFECT_DASH);
			move_count = STS_MOVEINTERVAL;
		}
	}

	if(input.GetInput( INPUT_SHOT ) == 1){
		sound.PlaySoundHandle(SOUNDEFECT_MENUDECIDE);
		decide = TRUE;
		fade_count = 60;
	}
	if(input.GetInput( INPUT_MENU ) == 1){
		sound.PlaySoundHandle(SOUNDEFECT_MENUCANCEL);
		return -1;
	}

	count++;
	return 0;
}

#define STSBACK_WIDTH 150
#define STSBACK_XSPACE 8
void GameLoop::StageSelect::Draw()
{
	image.DrawImage(10,10,IMG_STRSTAGESELECT,TRUE,0,FALSE);
	image.DrawImage(0,0,IMG_STSBACKEASY+game_level,TRUE,0,FALSE);

	int move_x = 0;
	int draw_b = 100;	//�ϐ��̍ė��p
	if(move_count > 5) {
		draw_b = (int)(100*((double)(move_count-5)/5));
		move_x = (10-move_count)*10;
	} else if(move_count > 0){
		draw_b = 100 - (int)(100*((double)(move_count)/5));
		move_x = move_count*10;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, draw_b);
	image.DrawImage(470+move_x,10,IMG_STSLMEASY+game_level,TRUE,0,FALSE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 100);

	for(int i = 0; i < STAGEST_STGNUM; i++){
		image.DrawImage(STSBACK_WIDTH*i + (i+1)*STSBACK_XSPACE, 100, IMG_STSBACKST1+i, FALSE, 0, FALSE);
	}

	if(count%9 < 4) SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	image.DrawImage(STSBACK_WIDTH*stage_num + (stage_num+1)*STSBACK_XSPACE-5, 100-5,IMG_STSFRAME,TRUE,0,FALSE);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	image.DrawImage(10, 310, IMG_STSHOWTO, TRUE, 0, FALSE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 100);

	if(decide){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (int)(((double)fade_count/60)*255));
		DrawBox(0,0,640,480,GetColor(0,0,0),TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
}

int GameLoop::Credit::Update()
{
	count++;

	if(input.GetInput(INPUT_SHOT) == 1 || input.GetInput(INPUT_SLOW) == 1){
		return 1;
	}

	return 0;
}

void GameLoop::Credit::Draw()
{
	image.DrawImage(0,0,IMG_CREDIT,FALSE,0,FALSE);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255-(int)(((double)count/60)*255));
	DrawBox(0,0,WINDOWSIZE_X,WINDOWSIZE_Y,GetColor(0,0,0),TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

int GameLoop::Loop()
{	//�Q�[�����[�v
	int result = 0; //���̒l���A1�ɂȂ��Ă�����A�Q�[���I��
	
	switch(g_stat){
	case GAMELOOP_TITLE :

		switch(title->Update()){
		case 0 :	//�^�C�g������̕ԓ���0�Ȃ�
			delete title;	//�^�C�g���I�u�W�F�N�g��j��
			title = NULL;	//NULL���߂�

			game = new Game(0, 1, TRUE);	//�f�t�H���g��ԂŊJ�n
			g_stat = GAMELOOP_MAIN;	//�Q�[�����C����Ԃ�
	
			break;

		case 1 : delete title;title=NULL;
			st_select = new StageSelect();
			g_stat = GAMELOOP_STAGESELECT;
			
			break;

		case 2 :
			g_stat = GAMELOOP_CREDIT;
			credit = new Credit();
			delete title;title=NULL;break;

		case -2:	//�^�C�g������̕ԓ���-2�Ȃ�
			result = 1; delete title;title=NULL; break;
		default :
			title->Draw();break;
		}
		break;

	case GAMELOOP_STAGESELECT :

		switch(st_select->Update(&game)){
		case 0 :
			st_select->Draw();
			break;

		case 1 : delete st_select;st_select=NULL;
			g_stat = GAMELOOP_MAIN;
			
			break;

		case -1: delete st_select;st_select=NULL;
			title = new Title();
			g_stat = GAMELOOP_TITLE;

			break;
		}

		break;

	case GAMELOOP_CREDIT :
		if(credit->Update() == 1){
			delete credit;
			credit = NULL;
			g_stat = GAMELOOP_TITLE;
			title = new Title();
		} else credit->Draw();

		break;

	case GAMELOOP_MAIN :

		switch(game->Update()){
		case 1 :	//�Q�[���{�̂���̕ԓ���1�Ȃ�
			g_stat = GAMELOOP_FINAL; break;	//�Q�[���I�[�o�[��Ԃ�
		case 2 :	//�Q�[���{�̂���̕ԓ���2�Ȃ�
			g_stat = GAMELOOP_FINAL;
			break;	//�Q�[���N���A��Ԃ�
		case 3 :
			g_stat = GAMELOOP_FINAL; break;
		case 0:
			game->Draw();break;	//0�Ȃ�`������ăQ�[���{�̑��s
		}
		break;

		/*
	case GAMELOOP_GAMECLEAR :

		if(g_clear->Update() == 1){
			delete g_clear;
			g_clear = NULL;
			g_stat = GAMELOOP_FINAL;
		} else g_clear->Draw();
		break;
		*/

	case GAMELOOP_FINAL :
		delete game;	//�Q�[���I�u�W�F�N�g��j��
		game = NULL;	//NULL���l�߂�
		
		m_result = -1;	//�߂�l��-1�Ɖ���
		m_count = 0;	//�J�E���^��������

		title = new Title();
		g_stat = GAMELOOP_TITLE;	//�^�C�g����Ԃ�
		break;
	}

	return result;
}

