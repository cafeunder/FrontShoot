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
クラス名 : MyCharacter
	マイキャラ（自機）を表すクラス

	継承 : Character

	記述ファイル : mychar.h
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
	
		delete sate[i];	//オブジェクトを破棄
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
		//インターバル中なら、カウントを下げる

	Move();	//移動処理
	UpdateSatellite();
	//if(sate != NULL) sate->Update(this);

	if(dash_invin < 10) dash_invin += 0.5;	//ダッシュ無敵カウントが減っていれば、回復

	if(invin_count > 0) invin_count--;	//無敵中ならカウントを減らす
	//ダッシュ中なら
	if(dash_count > 0) dash_count--;
	if(dash_count < 0) dash_count++;
	//カウントを減らす

	return 0;
}

int need_exp[MYSHOT_MAXLEVEL] = {40,60,100,150}; //各レベルにおいて、レベルアップに必要なexp
int MyCharacter::Update(EnemyMgr* ene_mgr, ShotMgr* sht_mgr, Boss* boss)
{
	if(interval_count > 0) interval_count--;
		//インターバル中なら、カウントを下げる

	Move();	//移動処理
	UpdateSatellite(sht_mgr, ene_mgr);	//サテライトの更新
	Shot(sht_mgr);//ショット処理

	if(invin_count != 0) goto COLLITION_END;
		//無敵中なら、かすり、当たり判定を無視

	//かすり判定部
	int num = sht_mgr->JudgeGraze(px+(width-g_width)/2,g_width,py+(height-g_height)/2,g_height, SHOTSIDE_MYCHAR);
		//かすっているかどうかの判定

	if(num != 0) {
		sound.PlaySoundHandle(SOUNDEFECT_GRAZE);
	}

	charge+=num*10;	//かすった数*10チャージする
	if(charge > MYSHOT_MAXCHARGE) charge = MYSHOT_MAXCHARGE;

	interval_count-=num*5;	//かすった数*5インターバルを縮める
	if(interval_count < 0) interval_count = 0;

	if(boss == NULL){	//ボスが出現していなければ
		rate+=(double)num/10;	//かすった数/10レートを上昇させる
		if(rate > RATE_MAX) rate = RATE_MAX;
	}
	//


	if(abs(dash_count) - (16 - (int)(dash_invin)) > 0) goto COLLITION_END;
		//ダッシュ中なら、当たり判定を無視


	//当たり判定部
	bool col = FALSE;	//仮に当たっていないとしておく

	if(boss != NULL && boss->Collition((int)px, width, (int)py, height)) {Damage(1); col = TRUE;}

	//エネミーとの当たり判定
	if(ene_mgr->Collition((int)px, width, (int)py, height))  {Damage(1); col = TRUE;}
	//エネミーと当たっていれば、ダメージ

	//ショットとの当たり判定
	if(sht_mgr->ColCharCalc(this, SHOTSIDE_MYCHAR)) col = TRUE;

	if(col){	//もし、当たっていたら

		invin_count = 180;	//無敵カウントの設定
		efect.SetEfect(px+width/2+GAME_X,py+height/2,EFECT_SMALLBURN,60,GetColor(100,100,100));
		sound.PlaySoundHandle(SOUNDEFECT_MYCHARDAMAGE);
			//ダメージ時エフェクトをセット
		//if(sate != NULL && sate->GetState() == SATESTAT_HAD) sate->Parge();
			//サテライトをもっているなら、サテライトをパージ

		if(shot_level < MYSHOT_MAXLEVEL){
			shot_exp-=need_exp[shot_level]/2;	//現在必要経験値の1/3を失う
	
			if(shot_exp < 0) {	//もし、経験値が0以下になったら
				if(shot_level == 0){
					shot_exp = 0;
				} else {
					shot_level--;	//レベルを下げる

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


		ReleasePoint();	//ポイントの開放
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

	//レベルアップ処理
	if(shot_level < MYSHOT_MAXLEVEL && shot_exp >= need_exp[shot_level]){ 

		efect.SetEfect(px+1,py,EFECT_LEVELUPDOWN,IMG_LEVELUP);
		shot_exp-=need_exp[shot_level]; 
		shot_level++;
		sound.PlaySoundHandle(SOUNDEFECT_LEVELUP);
	}
	//

	if(dash_invin < 10) dash_invin += 0.5;	//ダッシュ無敵カウントが減っていれば、回復

	if(invin_count > 0) invin_count--;	//無敵中ならカウントを減らす
	//ダッシュ中なら
	if(dash_count > 0) dash_count--;
	if(dash_count < 0) dash_count++;
	//カウントを減らす

	if(life <= 0) {
		efect.SetEfect(px+width/2+GAME_X,py+height/2,EFECT_SMALLBURN, 30, GetColor(255,0,0));
		sound.PlaySoundHandle(SOUNDEFECT_ENEBURN);
		return 1;	//耐久度がなければ1を返す
	}
	return 0;
}

void MyCharacter::Draw(int count)
{
	//当たり矩形が、画像の中心に来るように補正
	float draw_x = (float)px - (img_w/2 - width/2);
	float draw_y = (float)py - (img_h/2 - height/2);
	//

	if(invin_count%20 < 10){
		if(abs(dash_count) - (16 - (int)(dash_invin)) > 0) //ダッシュ無敵状態なら
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 126);	//アルファブレンドで描画

		if(JudgeMaxCharge() && count%20 < 10) SetDrawBright(255,255,0);
			//チャージがたまっていれば、一定周期で黄色く点滅

		//一定周期で、２つの画像を交互に描画する
		if(count%6 > 3) img_div = 1;
		else img_div = 2;
		//

		image.DrawImageF(draw_x, draw_y, img_index, TRUE, img_div);
			//描画

		//描画モードを元に戻す
		SetDrawBright(255,255,255);
		if(abs(dash_count) - (16 - (int)(dash_invin)) > 0) SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 126);
		//
	}
	DrawSatellite();
		//サテライトの描画

#ifdef DEBUG_CODE
	_DrawBox(int(px), int(py), int(px)+width, int(py)+height, GetColor(0,0,255),FALSE);
	_DrawBox(int(px)+(width-g_width)/2, int(py)+(height-g_height)/2, int(px)+(width-g_width)/2+g_width, int(py)+(height-g_height)/2+g_height, GetColor(255,0,0),FALSE);
	//あたり矩形範囲の表示
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

#define MYCHAR_ACCEL 4 //マイキャラの加速度
#define MYCHAR_SACCEL 2	//低速時の加速度
#define DASH_COUNT 15	//ダッシュするカウント
#define DASH_MOVE 120	//ダッシュする距離
void MyCharacter::Move()
{
	if(dash_count != 0) {	//ダッシュ状態なら
		double fsin = SinFract(PI/2, (DASH_COUNT-abs(dash_count)), DASH_COUNT, DASH_MOVE);
					//sinによる「移動量」の計算

		if(dash_count > 0) px+=fsin;	//ダッシュカウントが正なら右へ
		if(dash_count < 0) px-=fsin;	//負なら左へ移動させる

		goto INPUTEND;	//ダッシュ中はキー入力を受け付けない
	}

	double d_accel = MYCHAR_ACCEL;	//一時的に加速度を設定

	if( (input.GetInput( INPUT_SLOW ) > 0) ) d_accel = MYCHAR_SACCEL;
		//低速キーが押されていたら加速度を低速に
	if(sate[0] != NULL && sate[0]->GetSmash()) d_accel = sate[0]->QuerySmashAccel();
		//サテライトスマッシュ中なら、決められた加速度に固定

	if( (input.GetInput( INPUT_UP ) > 0 || input.GetInput( INPUT_DOWN ) > 0)
											&&
		(input.GetInput( INPUT_LEFT ) > 0 || input.GetInput( INPUT_RIGHT ) > 0))
		//上下どちらかと左右どちらかが同時に押されていれば
				d_accel = d_accel/sqrt(2.0);	//加速度をルート２で割る

	//キーに対応した動作を行う
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
	if((input.GetInput( INPUT_RIGHTDASH ) == 1)) {	//ダッシュキーが押されていたら
		sound.PlaySoundHandle(SOUNDEFECT_DASH);
		dash_count = DASH_COUNT;	//ダッシュカウンタを設定
		dash_invin -= 3;	//ダッシュ無敵時間を3減らす
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

	//画面外移動禁止
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
		//サテライトスマッシュ中なら、ショットは撃てない

	if(input.GetInput( INPUT_SHOT ) > 0){	//ショットキーが押されていて、

		if(input.GetInput( INPUT_SHOT ) == 1){	//初回入力時なら、貯め撃ち判定

			if(JudgeMaxCharge()){	//マックスまでチャージされていれば、
				sht_mgr->SetShot(px+width/2, py, 10, -PI/2, SHOT_CHARGESHOT, SHOTOPE_CHARGESHOT, SHOTSIDE_MYCHAR, 0);
					//チャージショットを撃つ
				interval_count = 40;
					//インターバルを設定して、
				result = 1;
					//撃ったことを知らせる
				sound.PlaySoundHandle(SOUNDEFECT_CHARGESHOT);
			}
			
			charge = 0;	//ショットキー初回入力時は、チャージをリセットする
		}

		if(interval_count == 0) //インターバルでなければ
		{
			interval_count = MyShotOperate(shot_level, this, sht_mgr);	//ショットを予約
			sound.PlaySoundHandle(SOUNDEFECT_SHOT);
			result = 1;
		}
	} else {
		charge++;	//ショットキーが押されていなければチャージ
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
	if(rate > 1.0) efect.SetEfect(300, 25, EFECT_BONUS, 0, (int)(point*rate)/10 * 10, rate);	//スコア加算をエフェクトで知らせる
	score += (int)(point*rate)/10 * 10;	//10で割って10をかけているのは、一桁目を０にするため（個人的趣味

	//ポイントと倍率をリセット
	point = 0;
	rate = 1;
	//
}