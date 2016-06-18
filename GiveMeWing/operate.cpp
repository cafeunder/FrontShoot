#define _CRT_SECURE_NO_WARNINGS

#include <DxLib.h>
#include <math.h>
#include "operate.h"
#include "database.h"
#include "image.h"
#include "main.h"
#include "sound.h"


#define OPERATEAREA_X 400	//�I�y���[�^�[�G���A��\������x���W
#define OPERATEAREA_Y 174	//y���W
#define OPERATEAREA_W (WINDOWSIZE_X-OPERATEAREA_X)	//�I�y���[�g�G���A�̕�
#define OPERATEAREA_H (WINDOWSIZE_Y-OPERATEAREA_Y)	//�I�y���[�g�G���A�̍���
#define STANDBY_COUNT 35



void OperateArea::Update()
{
	switch(state){
	case OPEAREA_SHUTDOWN : break;	//�V���b�g�_�E����ԂȂ�Ȃɂ����Ȃ�

	case OPEAREA_STANDBY :	//�X�^���o�C��ԂȂ牽�����Ȃ����A
		if(count == STANDBY_COUNT){	//�X�^���o�C�K�v�J�E���g���o�߂�����A
			count = 0;	//�J�E���g�����������āA
			state = OPEAREA_STANDING;	//�N����Ԃ�
		}
		break;

	case OPEAREA_STANDING:	//�N����ԂȂ�
		switch(mesbox.Update()){//���b�Z�[�W�{�b�N�X�̍X�V���s���A
		case 0 : draw_mes = TRUE; break;	//0���A���Ă����烁�b�Z�[�W�\����
		case 1 :	//1���A���Ă����玟�̃��b�Z�[�W��
			for(int i = 0; i < STRING_MAXNUM-1; i++){
				operate_type[i] = operate_type[i+1];	//�I�y���[�q�\���^�C�v���P�V�[�N
			}
			operate_type[STRING_MAXNUM-1] = -1;
			break;
		case -1 : draw_mes = FALSE;	break;
		break;
		}
	}

	count++;	//�J�E���g��i�߂�
}

void OperateArea::Draw()
{
	switch(state){
	case OPEAREA_SHUTDOWN : break;	//�V���b�g�_�E����ԂȂ牽�����Ȃ�
	case OPEAREA_STANDBY :	//�X�^���o�C��ԂȂ�A
		DrawBox(OPERATEAREA_X, OPERATEAREA_Y, OPERATEAREA_X+OPERATEAREA_W, WINDOWSIZE_Y+OPERATEAREA_H, GetColor(10,10,10),TRUE);
		//�f�B�X�v���C�̓d���������Ƃ����ۂ����o
		if(count >= 15 && count < 25){
			int center_x = OPERATEAREA_X + OPERATEAREA_W/2;	//���Sx
			int w_toc = (int)(((double)(count-15)/15)*(OPERATEAREA_W/2));//���S����̕�

			DrawLine(center_x-w_toc, OPERATEAREA_Y+OPERATEAREA_H/2, center_x+w_toc, OPERATEAREA_Y+OPERATEAREA_H/2, GetColor(120,120,120), 5);
				//�f�B�X�v���C�������Ƃ��̐���`��
		} else if(count > 30){
			int center_y = OPERATEAREA_Y + OPERATEAREA_H/2;	//���Sy
			int h_toc = (int)(sin((PI/2)*(((double)count-30)/5))*(OPERATEAREA_H/2)); //���S����̍���

			DrawBox(OPERATEAREA_X, center_y-h_toc, OPERATEAREA_X+OPERATEAREA_W, center_y+h_toc, GetColor(0,20,0), TRUE);
				//�f�B�X�v���C�̉摜���W�J���鉉�o
		}
		//

		break;
	case OPEAREA_STANDING :;

		DrawBox(OPERATEAREA_X, OPERATEAREA_Y, OPERATEAREA_X+OPERATEAREA_W, WINDOWSIZE_Y+OPERATEAREA_H, GetColor(0,20,0),TRUE);
			//�I�y���[�^�G���A�w�i�̕`��

		if(operate_type[0] != -1){
			image.DrawImage(OPERATEAREA_X,OPERATEAREA_Y,IMG_OPE1,FALSE,operate_type[0],FALSE);
			//�I�y���[�q�̕`��
		}

		if(draw_mes){//���W���[����`�悷��Ȃ�A
			mesbox.Draw();	//���b�Z�[�W�{�b�N�X�̕`��
		}

		/*
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, 63);
		//tag
		DrawBox(OPERATEAREA_X, OPERATEAREA_Y+(count*2)%OPERATEAREA_H, OPERATEAREA_X+OPERATEAREA_W, OPERATEAREA_Y+(count*2)%OPERATEAREA_H+5, GetColor(60,120,60),TRUE);
		DrawBox(OPERATEAREA_X, OPERATEAREA_Y+(count*2)%OPERATEAREA_H+7, OPERATEAREA_X+OPERATEAREA_W, OPERATEAREA_Y+(count*2)%OPERATEAREA_H+10, GetColor(60,120,60),TRUE);

		DrawBox(OPERATEAREA_X, OPERATEAREA_Y+(count*2)%OPERATEAREA_H+15, OPERATEAREA_X+OPERATEAREA_W, OPERATEAREA_Y+(count*2)%OPERATEAREA_H+15+2, GetColor(60,120,60),TRUE);
		DrawBox(OPERATEAREA_X, OPERATEAREA_Y+(count*2)%OPERATEAREA_H+19, OPERATEAREA_X+OPERATEAREA_W, OPERATEAREA_Y+(count*2)%OPERATEAREA_H+19+1, GetColor(60,120,60),TRUE);
		//�����̉摜������Ă����\��
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 126);
		*/
	}
}

void OperateArea::SetMessage(const char* str, int operater_type, int str_brake)
{
	if(state == OPEAREA_SHUTDOWN) return;	//�N�����Ă��Ȃ��Ȃ烁�b�Z�[�W��\�񂵂Ȃ�
	//sound.PlaySoundHandle(SOUNDEFECT_OPESTART);

	mesbox.SetMessage(str, str_brake);

	for(int i = 0; i < STRING_MAXNUM; i++){
		if(operate_type[i] != -1) continue;
		operate_type[i] = operater_type;
		return;
	}
}

void OperateArea::Reset()
{
	draw_mes = FALSE;
	for(int i = 0; i < STRING_MAXNUM; i++){
		operate_type[i] = -1;
	}

	mesbox.Reset();
}



bool JudgeTwoByteChar(char c)
{
    if(((BYTE)0x81<=(BYTE)c && (BYTE)c<=(BYTE)0x9F) ||
        ((BYTE)0xE0<=(BYTE)c && (BYTE)c<=(BYTE)0xFF)) return TRUE;
	else return FALSE;
}


MesBox::MesBox()
{
	for(int i = 0; i < STRING_MAXNUM; i++){
		string[i] = NULL;
	}

	draw_mes[0]= '\0';
	brake_count = -1;
	interval_count = 0;
	index = 0;
}

MesBox::~MesBox()
{
	for(int i = 0; i < STRING_MAXNUM; i++){
		delete string[i];
	}
}

#define MESSAGE_BRAKECOUNT 180	//�f�t�H���g�̕`��I����ҋ@����
int MesBox::SetMessage(const char* str, int str_brake)
{
	for(int i = 0; i < STRING_MAXNUM; i++){
		if(string[i] != NULL) continue;	//���łɃ��b�Z�[�W���\�񂳂�Ă���Ȃ�A�\�񂵂Ȃ�
		
		int length = strlen(str);	//���b�Z�[�W�̒���������ׂ�(�I�[�����̓J�E���g���Ă��Ȃ�!

		string[i] = (char*)malloc(sizeof(char)*(length+1));//�������̊m�ہi�I�[���������m�ۂ���
		
		strcpy(string[i], str);	//������̃R�s�[

		//�`��I����ҋ@���Ԃ��Z�b�g
		if(str_brake == -1){	//�w�肪-1�Ȃ�
			string_brake[i] = MESSAGE_BRAKECOUNT;	//�f�t�H���g�̑ҋ@���Ԃ��Z�b�g
		} else string_brake[i] = str_brake;

		return 0;
	}

	return 1;
}

#define MESSAGE_INTERVAL 3	//������\�����鎞�ԊԊu
int MesBox::StrUpdate()
{
	if(interval_count > 0){interval_count--; return 0;}
		//�C���^�[�o�����Ȃ�A�`�敶������X�V���Ȃ�

	char c = string[0][index];	//�ŌẪ��b�Z�[�W�́A���݃C���f�b�N�X�̕������m��

	draw_mes[index] = c;	//�`�敶����̌��݃C���f�b�N�X�ɑ��

	if(c == '\0') return 1;	//�����A���ꂪ�I�[�����Ȃ�A�`��I��
	if(JudgeTwoByteChar(c)){	//�����A���ꂪ2byte�����Ȃ�
		index++;	//�C���f�b�N�X���ЂƂi�߁A
		draw_mes[index] = string[0][index];	//�����ЂƂ������Z�b�g
	}
	draw_mes[index+1] = '\0';	//���ݕ���+1�ԖڂɏI�[�������Z�b�g
	
	//�����łȂ����
	index++; //���݃C���f�b�N�X��i�߁A
	interval_count = MESSAGE_INTERVAL;//�C���^�[�o���J�E���g���Z�b�g

	sound.PlaySoundHandle(SOUNDEFECT_OPETYPE);//�^�C�v��

	return 0;
}

int MesBox::Update()
{
	int result = 0;
	if(brake_count == 0) {draw_mes[0] = '\0'; result = 1;}	//�`���ҋ@�I���Ȃ�A�`�敶�����������
	if(brake_count > -1) {brake_count--; return result;}

	if(string[0] == NULL) return -1;	//�`�悷�ׂ����b�Z�[�W���ЂƂ��Ȃ��Ȃ�A�������Ȃ�

	if(StrUpdate() == 1){	//�`�敶����X�V
		//�߂�l�Ƃ���1���A���Ă�����A�`��I��
	
		brake_count = string_brake[0];	//�`��I����ҋ@�J�E���g���Z�b�g
		index = 0;	//���݃C���f�b�N�X��������

		delete string[0];	//�����Ƃ��Â��Z�b�g���ꂽ���b�Z�[�W���폜
		string[0] = NULL;	//NULL���߂�

		for(int i = 0; i < STRING_MAXNUM-1; i++){
			string[i] = string[i+1];	//���b�Z�[�W�z����ЂƂV�[�N
			string_brake[i] = string_brake[i+1];
		}
		string[STRING_MAXNUM-1] = NULL;	//�Ō�̗v�f��NULL���߂�
		string_brake[STRING_MAXNUM-1] = 0;
	}

	return 0;
}

#define MESBOX_EDGESIZE 4	//���b�Z�[�W�{�b�N�X�̘g�T�C�Y
#define MESBOX_X OPERATEAREA_X		//���b�Z�[�W�{�b�N�X��`�悷��x���W
#define MESBOX_Y 371	//���b�Z�[�W�{�b�N�X��`�悷��y���W
#define MESBOX_W 240	//���b�Z�[�W�{�b�N�X�̕�
#define MESBOX_H 110	//���b�Z�[�W�{�b�N�X�̍���

#define MESLINE_HEIGHT 25 //��s�ɗv���鍂��
#define MES_X 5	//���b�Z�[�W�{�b�N�X�̍�������_�Ƃ������b�Z�[�W�`��x���W
#define MES_Y 5 //y���W
void MesBox::Draw()
{
	//���b�Z�[�W�{�b�N�X�̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(MESBOX_X,MESBOX_Y,MESBOX_X+MESBOX_W,MESBOX_Y+MESBOX_H,GetColor(150,200,150),TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 126);
	for(int i = 0; i < MESBOX_EDGESIZE; i++){
		DrawBox(MESBOX_X+i,MESBOX_Y+i,MESBOX_X+MESBOX_W-i,MESBOX_Y+MESBOX_H-i,GetColor(80,80,80),FALSE);
	}
	//

	int mes_fonthan = font_db.QueryFontData(FONT_MESSAGE);

	int line = 0;		//���ݍs

	int line_index = 0;	//���ݍs�̕����C���f�b�N�X
	int draw_index = 0;	//�`�敶����̕����C���f�b�N�X

	int color = GetColor(10,10,10);

	char line_mes[MESLINE_MAXNUM+1];	//��s�ɕ\���ł��镶����+1(�I�[����

	while(line < MESBOX_LINEMAX && draw_index < MESBOX_MAXNUM){
		char c = draw_mes[draw_index];	//�`�敶����́A���݃C���f�b�N�X�̕������m��

		line_mes[line_index] = c;	//���ݍs�̌��݃C���f�b�N�X�ɑ��
		if(c == '\0'){	//�����A���ꂪ�I�[�����Ȃ�
			DrawStringToHandle(MESBOX_X+MES_X,MESBOX_Y+line*MESLINE_HEIGHT+MES_Y,line_mes,color,mes_fonthan);
				//�������`�悵��
			return;	//�`�揈���I��
		}
		if(c == '\n'){
			//�����A���ꂪ���s�����Ȃ�
			
			line_mes[line_index] = '\0';	//�I�[������}����
			DrawStringToHandle(MESBOX_X+MES_X,MESBOX_Y+line*MESLINE_HEIGHT+MES_Y,line_mes,color,mes_fonthan);
				//�������`�悵��

			line_index = 0;	//���ݍs�̌��݃C���f�b�N�X��擪�ɖ߂��A
			line_mes[0] = '\0';	//���ݍs�̕������������
			line++;	//���̍s��
		} else if(line_index == MESLINE_MAXNUM-1){
			//���݃C���f�b�N�X���s�����񖖔��̒��O�Ȃ�Ȃ�A
			//�s������̍Ō�͕K���I�[�����Ŏg��Ȃ���΂����Ȃ��̂�

			line_mes[line_index+1] = '\0';
			DrawStringToHandle(MESBOX_X+MES_X,MESBOX_Y+line*MESLINE_HEIGHT+MES_Y,line_mes,color,mes_fonthan);
				//�������`�悵��

			line_index = 0;	//���ݍs�̌��݃C���f�b�N�X��擪�ɖ߂��A
			line_mes[0] = '\0';	//���ݍs�̕������������
			line++;	//���̍s��
		} else line_index++;	//���s�łȂ��Ȃ�A����

		draw_index++;
	}

	/*
	����ȃ��b�Z�[�W�`�悪�s��ꂽ�ꍇ�A
	���̊֐��͕K���I�[�������o�ɂ��return���ɂ���ďI�����邽�߁A
	while���I�������Ƃ������Ƃ͕s�����������ƌ������ƁA
	*/

	//ErrorExit("�s���ȕ`�惁�b�Z�[�W");	//�Ȃ̂ŁA�G���[�I��
}

void MesBox::Reset()
{
	for(int i = 0; i < STRING_MAXNUM; i++){
		string[i] = NULL;
	}

	draw_mes[0]= '\0';
	brake_count = -1;
	interval_count = 0;
	index = 0;
}
