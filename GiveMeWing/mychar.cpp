#include <DxLib.h>
#include <math.h>
#include "input.h"
#include "mychar.h"
#include "enemy.h"
#include "shot.h"
#include "sht_ope.h"
#include "myshot.h"
#include "satellite.h"
#include "efect.h"
#include "main.h"
#include "image.h"
#include "sound.h"
#include "boss.h"

/*
�N���X�� : MyCharacter
	�}�C�L�����i���@�j��\���N���X

	�p�� : Character

	�L�q�t�@�C�� : mychar.h
*/

double SinFract(double point, double count, double cycle, double fract);

MyCharacter::MyCharacter(double a_x, double a_y, int a_w, int a_h, int a_gw, int a_gh, int a_life, int a_imgnum):Character(a_x, a_y, a_w, a_h, a_life, a_imgnum)
{
	g_width = a_gw;
	g_height = a_gh;
	rate=1;
	charge=0;
	point=0;
	score=0;
	interval_count=0;
	shot_level=0;
	shot_exp=0;
	invin_count=0;
	dash_count=0;
	dash_invin=11.0;

	for(int i = 0; i < 2; i++){
		sate[i] = NULL;
	}
}

MyCharacter::~MyCharacter()
{
	for(int i = 0; i < 2; i++){
		if(sate[i] == NULL) continue;
	
		delete sate[i];	//�I�u�W�F�N�g��j��
	}
}

void MyCharacter::Initialize()
{
	life = 6;

	px = GAME_WIDTH/2-7.0;
	py = 440.0;

	charge = 0;
	interval_count=0;
	invin_count=0;
	dash_count=0;
	dash_invin=11.0;
}

int MyCharacter::Update()
{
	if(interval_count > 0) interval_count--;
		//�C���^�[�o�����Ȃ�A�J�E���g��������

	Move();	//�ړ�����
	UpdateSatellite();
	//if(sate != NULL) sate->Update(this);

	if(dash_invin < 10) dash_invin += 0.5;	//�_�b�V�����G�J�E���g�������Ă���΁A��

	if(invin_count > 0) invin_count--;	//���G���Ȃ�J�E���g�����炷
	//�_�b�V�����Ȃ�
	if(dash_count > 0) dash_count--;
	if(dash_count < 0) dash_count++;
	//�J�E���g�����炷

	return 0;
}

int need_exp[MYSHOT_MAXLEVEL] = {40,60,100,150}; //�e���x���ɂ����āA���x���A�b�v�ɕK�v��exp
int MyCharacter::Update(EnemyMgr* ene_mgr, ShotMgr* sht_mgr, Boss* boss)
{
	if(interval_count > 0) interval_count--;
		//�C���^�[�o�����Ȃ�A�J�E���g��������

	Move();	//�ړ�����
	UpdateSatellite(sht_mgr, ene_mgr);	//�T�e���C�g�̍X�V
	Shot(sht_mgr);//�V���b�g����

	if(invin_count != 0) goto COLLITION_END;
		//���G���Ȃ�A������A�����蔻��𖳎�

	//�����蔻�蕔
	int num = sht_mgr->JudgeGraze(px+(width-g_width)/2,g_width,py+(height-g_height)/2,g_height, SHOTSIDE_MYCHAR);
		//�������Ă��邩�ǂ����̔���

	if(num != 0) {
		sound.PlaySoundHandle(SOUNDEFECT_GRAZE);
	}

	charge+=num*10;	//����������*10�`���[�W����
	if(charge > MYSHOT_MAXCHARGE) charge = MYSHOT_MAXCHARGE;

	interval_count-=num*5;	//����������*5�C���^�[�o�����k�߂�
	if(interval_count < 0) interval_count = 0;

	if(boss == NULL){	//�{�X���o�����Ă��Ȃ����
		rate+=(double)num/10;	//����������/10���[�g���㏸������
		if(rate > RATE_MAX) rate = RATE_MAX;
	}
	//


	if(abs(dash_count) - (16 - (int)(dash_invin)) > 0) goto COLLITION_END;
		//�_�b�V�����Ȃ�A�����蔻��𖳎�


	//�����蔻�蕔
	bool col = FALSE;	//���ɓ������Ă��Ȃ��Ƃ��Ă���

	if(boss != NULL && boss->Collition((int)px, width, (int)py, height)) {Damage(1); col = TRUE;}

	//�G�l�~�[�Ƃ̓����蔻��
	if(ene_mgr->Collition((int)px, width, (int)py, height))  {Damage(1); col = TRUE;}
	//�G�l�~�[�Ɠ������Ă���΁A�_���[�W

	//�V���b�g�Ƃ̓����蔻��
	if(sht_mgr->ColCharCalc(this, SHOTSIDE_MYCHAR)) col = TRUE;

	if(col){	//�����A�������Ă�����

		invin_count = 180;	//���G�J�E���g�̐ݒ�
		efect.SetEfect(px+width/2+GAME_X,py+height/2,EFECT_SMALLBURN,60,GetColor(100,100,100));
		sound.PlaySoundHandle(SOUNDEFECT_MYCHARDAMAGE);
			//�_���[�W���G�t�F�N�g���Z�b�g
		//if(sate != NULL && sate->GetState() == SATESTAT_HAD) sate->Parge();
			//�T�e���C�g�������Ă���Ȃ�A�T�e���C�g���p�[�W

		if(shot_level < MYSHOT_MAXLEVEL){
			shot_exp-=need_exp[shot_level]/2;	//���ݕK�v�o���l��1/3������
	
			if(shot_exp < 0) {	//�����A�o���l��0�ȉ��ɂȂ�����
				if(shot_level == 0){
					shot_exp = 0;
				} else {
					shot_level--;	//���x����������

					efect.SetEfect(px+1,py,EFECT_LEVELUPDOWN,IMG_LEVELDOWN);
					sound.PlaySoundHandle(SOUNDEFECT_LEVELDOWN);

					shot_exp = need_exp[shot_level]/2;
				}
			}
		} else {
			shot_level--;

			efect.SetEfect(px+1,py,EFECT_LEVELUPDOWN,IMG_LEVELDOWN);
			sound.PlaySoundHandle(SOUNDEFECT_LEVELDOWN);

			shot_exp = need_exp[shot_level]/2;
		}


		ReleasePoint();	//�|�C���g�̊J��
	}

COLLITION_END:

	if(shot_level >= 2){
		if(sate[0] == NULL) SetSatellite(px+width/2, py+height/2, 0);
	} else if(sate[0] != NULL) {delete sate[0]; sate[0] = NULL;}

	if(shot_level >= 4){
		if(sate[1] == NULL) {
			sate[0]->Reset();
			SetSatellite(px+width/2, py+height/2, 1);
		}
	} else if(sate[1] != NULL) {delete sate[1]; sate[1] = NULL;}

	//���x���A�b�v����
	if(shot_level < MYSHOT_MAXLEVEL && shot_exp >= need_exp[shot_level]){ 

		efect.SetEfect(px+1,py,EFECT_LEVELUPDOWN,IMG_LEVELUP);
		shot_exp-=need_exp[shot_level]; 
		shot_level++;
		sound.PlaySoundHandle(SOUNDEFECT_LEVELUP);
	}
	//

	if(dash_invin < 10) dash_invin += 0.5;	//�_�b�V�����G�J�E���g�������Ă���΁A��

	if(invin_count > 0) invin_count--;	//���G���Ȃ�J�E���g�����炷
	//�_�b�V�����Ȃ�
	if(dash_count > 0) dash_count--;
	if(dash_count < 0) dash_count++;
	//�J�E���g�����炷

	if(life <= 0) {
		efect.SetEfect(px+width/2+GAME_X,py+height/2,EFECT_SMALLBURN, 30, GetColor(255,0,0));
		sound.PlaySoundHandle(SOUNDEFECT_ENEBURN);
		return 1;	//�ϋv�x���Ȃ����1��Ԃ�
	}
	return 0;
}

void MyCharacter::Draw(int count)
{
	//�������`���A�摜�̒��S�ɗ���悤�ɕ␳
	float draw_x = (float)px - (img_w/2 - width/2);
	float draw_y = (float)py - (img_h/2 - height/2);
	//

	if(invin_count%20 < 10){
		if(abs(dash_count) - (16 - (int)(dash_invin)) > 0) //�_�b�V�����G��ԂȂ�
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 126);	//�A���t�@�u�����h�ŕ`��

		if(JudgeMaxCharge() && count%20 < 10) SetDrawBright(255,255,0);
			//�`���[�W�����܂��Ă���΁A�������ŉ��F���_��

		//�������ŁA�Q�̉摜�����݂ɕ`�悷��
		if(count%6 > 3) img_div = 1;
		else img_div = 2;
		//

		image.DrawImageF(draw_x, draw_y, img_index, TRUE, img_div);
			//�`��

		//�`�惂�[�h�����ɖ߂�
		SetDrawBright(255,255,255);
		if(abs(dash_count) - (16 - (int)(dash_invin)) > 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 126);
		//
	}
	DrawSatellite();
		//�T�e���C�g�̕`��

#ifdef DEBUG_CODE
	_DrawBox(int(px), int(py), int(px)+width, int(py)+height, GetColor(0,0,255),FALSE);
	_DrawBox(int(px)+(width-g_width)/2, int(py)+(height-g_height)/2, int(px)+(width-g_width)/2+g_width, int(py)+(height-g_height)/2+g_height, GetColor(255,0,0),FALSE);
	//�������`�͈͂̕\��
#endif
}

void MyCharacter::SetSatellite(double ax, double ay, int num)
{
	if(sate[num] != NULL) return;

	int type = 0; 
	if(num == 1) type = sate[0]->GetType();

	sate[num] = new Satellite(ax, ay, 0, 0, IMG_DUMMY, num, type);
}

void MyCharacter::DrawSatellite()
{
	for(int i = 0; i < 2; i++){
		if(sate[i] == NULL) continue;

		sate[i]->Draw();
	}
}

void MyCharacter::UpdateSatellite(ShotMgr* sht_mgr, EnemyMgr* ene_mgr)
{
	for(int i = 0; i < 2; i++){
		if(sate[i] == NULL) continue;

		sate[i]->Update(this, sht_mgr, ene_mgr);
	}
}

#define MYCHAR_ACCEL 4 //�}�C�L�����̉����x
#define MYCHAR_SACCEL 2	//�ᑬ���̉����x
#define DASH_COUNT 15	//�_�b�V������J�E���g
#define DASH_MOVE 120	//�_�b�V�����鋗��
void MyCharacter::Move()
{
	if(dash_count != 0) {	//�_�b�V����ԂȂ�
		double fsin = SinFract(PI/2, (DASH_COUNT-abs(dash_count)), DASH_COUNT, DASH_MOVE);
					//sin�ɂ��u�ړ��ʁv�̌v�Z

		if(dash_count > 0) px+=fsin;	//�_�b�V���J�E���g�����Ȃ�E��
		if(dash_count < 0) px-=fsin;	//���Ȃ獶�ֈړ�������

		goto INPUTEND;	//�_�b�V�����̓L�[���͂��󂯕t���Ȃ�
	}

	double d_accel = MYCHAR_ACCEL;	//�ꎞ�I�ɉ����x��ݒ�

	if( (input.GetInput( INPUT_SLOW ) > 0) ) d_accel = MYCHAR_SACCEL;
		//�ᑬ�L�[��������Ă���������x��ᑬ��
	if(sate[0] != NULL && sate[0]->GetSmash()) d_accel = sate[0]->QuerySmashAccel();
		//�T�e���C�g�X�}�b�V�����Ȃ�A���߂�ꂽ�����x�ɌŒ�

	if( (input.GetInput( INPUT_UP ) > 0 || input.GetInput( INPUT_DOWN ) > 0)
											&&
		(input.GetInput( INPUT_LEFT ) > 0 || input.GetInput( INPUT_RIGHT ) > 0))
		//�㉺�ǂ��炩�ƍ��E�ǂ��炩�������ɉ�����Ă����
				d_accel = d_accel/sqrt(2.0);	//�����x�����[�g�Q�Ŋ���

	//�L�[�ɑΉ�����������s��
	if(input.GetInput( INPUT_UP ) > 0){
		py-=d_accel;
	}
	if(input.GetInput( INPUT_DOWN ) > 0){
		py+=d_accel;
	}
	if(input.GetInput( INPUT_LEFT ) > 0){
		px-=d_accel;
	}
	if(input.GetInput( INPUT_RIGHT ) > 0){
		px+=d_accel;
	}

	if(sate[0] != NULL && sate[0]->GetSmash()) goto INPUTEND;
	if((input.GetInput( INPUT_RIGHTDASH ) == 1)) {	//�_�b�V���L�[��������Ă�����
		sound.PlaySoundHandle(SOUNDEFECT_DASH);
		dash_count = DASH_COUNT;	//�_�b�V���J�E���^��ݒ�
		dash_invin -= 3;	//�_�b�V�����G���Ԃ�3���炷
		if(dash_invin < 0) dash_invin = 0;
	}
	if((input.GetInput( INPUT_LEFTDASH ) == 1)) {
		sound.PlaySoundHandle(SOUNDEFECT_DASH);
		dash_count = -DASH_COUNT;	
		dash_invin -= 3;
		if(dash_invin < 0) dash_invin = 0;
	}

	//

INPUTEND:

#define MYCHARADJ_W 10
#define MYCHARADJ_H 10

	//��ʊO�ړ��֎~
	if(px < MYCHARADJ_W) px = MYCHARADJ_W;
	if(px > GAME_WIDTH-MYCHARADJ_W) px = GAME_WIDTH-MYCHARADJ_W;
	if(py < MYCHARADJ_H) py = MYCHARADJ_H;
	if(py > GAME_HEIGHT-MYCHARADJ_H) py = GAME_HEIGHT-MYCHARADJ_H;
	//
}

int MyCharacter::Shot(ShotMgr* sht_mgr)
{
	int result = 0;

	if(sate[0] != NULL && sate[0]->GetSmash()) return 0;
		//�T�e���C�g�X�}�b�V�����Ȃ�A�V���b�g�͌��ĂȂ�

	if(input.GetInput( INPUT_SHOT ) > 0){	//�V���b�g�L�[��������Ă��āA

		if(input.GetInput( INPUT_SHOT ) == 1){	//������͎��Ȃ�A���ߌ�������

			if(JudgeMaxCharge()){	//�}�b�N�X�܂Ń`���[�W����Ă���΁A
				sht_mgr->SetShot(px+width/2, py, 10, -PI/2, SHOT_CHARGESHOT, SHOTOPE_CHARGESHOT, SHOTSIDE_MYCHAR, 0);
					//�`���[�W�V���b�g������
				interval_count = 40;
					//�C���^�[�o����ݒ肵�āA
				result = 1;
					//���������Ƃ�m�点��
				sound.PlaySoundHandle(SOUNDEFECT_CHARGESHOT);
			}
			
			charge = 0;	//�V���b�g�L�[������͎��́A�`���[�W�����Z�b�g����
		}

		if(interval_count == 0) //�C���^�[�o���łȂ����
		{
			interval_count = MyShotOperate(shot_level, this, sht_mgr);	//�V���b�g��\��
			sound.PlaySoundHandle(SOUNDEFECT_SHOT);
			result = 1;
		}
	} else {
		charge++;	//�V���b�g�L�[��������Ă��Ȃ���΃`���[�W
		if(charge == MYSHOT_MAXCHARGE) sound.PlaySoundHandle(SOUNDEFECT_CHARGE);
		if(charge > MYSHOT_MAXCHARGE) charge = MYSHOT_MAXCHARGE;
	}

	return result;
}

double MyCharacter::GetShotExpPer()
{
	if(shot_level == MYSHOT_MAXLEVEL) return 1; 
	return ((double)shot_exp/need_exp[shot_level]);
}

void MyCharacter::ReleasePoint()
{		
	if(rate > 1.0) efect.SetEfect(300, 25, EFECT_BONUS, 0, (int)(point*rate)/10 * 10, rate);	//�X�R�A���Z���G�t�F�N�g�Œm�点��
	score += (int)(point*rate)/10 * 10;	//10�Ŋ�����10�������Ă���̂́A�ꌅ�ڂ��O�ɂ��邽�߁i�l�I�

	//�|�C���g�Ɣ{�������Z�b�g
	point = 0;
	rate = 1;
	//
}