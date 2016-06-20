#include "DxLib.h"
#include "main.h"
#include "fps.h"
#include "database.h"
#include "gameloop.h"
#include "input.h"
#include "efect.h"

int CallKosen();

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	SetMainWindowText("Give Me Wing");
	SetOutApplicationLogValidFlag(FALSE);
	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen( DX_SCREEN_BACK ); //�E�B���h�E���[�h�ύX�Ə������Ɨ���ʐݒ�   

	font_db.LoadFontData();	//�摜�t�@�C���̃��[�h
	img_db.LoadImgData();
	sound_db.LoadSoundData();

	Fps fps;
	GameLoop loop;
	bool game_start = FALSE;

	while( ScreenFlip()==0 && ProcessMessage()==0 && ClearDrawScreen()==0 ){
		input.Update();	//�L�[�{�[�h���̍X�V
		fps.Update();

		//�@���@�����{�̂������ɏ����@��

		if(!game_start){
			game_start = TRUE;
		}
		
		if(game_start){
			if(loop.Loop() == 1) break;
		}

		SetDrawArea(GAME_X,GAME_Y,GAME_X+GAME_WIDTH,GAME_Y+GAME_HEIGHT);//�`��G���A���Q�[���͈͂ɐݒ�
		efect.Update();
		SetDrawArea(0,0,WINDOWSIZE_X,WINDOWSIZE_Y);	//�`��G���A�����ɖ߂�

		//�@���@�����{�̂������ɏ����@��

		fps.Draw();
		fps.Wait();			//fps����
	}
	
	DxLib_End();	//�I������
	return 0;
}

void _DrawBox(int x1, int y1, int x2, int y2, int Color, bool FillFlag)
{
	DrawBox(x1+GAME_X, y1+GAME_Y, x2+GAME_X, y2+GAME_Y, Color, FillFlag);
}