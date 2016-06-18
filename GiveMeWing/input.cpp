#include <DxLib.h>
#include "input.h"

Input input;

void Input::Update()
{
	char tempKey[256];
	GetHitKeyStateAll(tempKey);

	if(tempKey[KEY_INPUT_UP] != 0 || (GetJoypadInputState( DX_INPUT_PAD1 ) & PAD_INPUT_UP)){
		input[INPUT_UP]++;
	} else input[INPUT_UP] = 0;

	if(tempKey[KEY_INPUT_DOWN] != 0 || (GetJoypadInputState( DX_INPUT_PAD1 ) & PAD_INPUT_DOWN)){
		input[INPUT_DOWN]++;
	} else input[INPUT_DOWN] = 0;

	if(tempKey[KEY_INPUT_RIGHT] != 0 || (GetJoypadInputState( DX_INPUT_PAD1 ) & PAD_INPUT_RIGHT)){
		input[INPUT_RIGHT]++;
	} else input[INPUT_RIGHT] = 0;

	if(tempKey[KEY_INPUT_LEFT] != 0 || (GetJoypadInputState( DX_INPUT_PAD1 ) & PAD_INPUT_LEFT)){
		input[INPUT_LEFT]++;
	} else input[INPUT_LEFT] = 0;


	if(tempKey[KEY_INPUT_Z] != 0 || (GetJoypadInputState( DX_INPUT_PAD1 ) & PAD_INPUT_4)){
		input[INPUT_SHOT]++;
	} else input[INPUT_SHOT] = 0;

	if(tempKey[KEY_INPUT_LCONTROL] != 0 || (GetJoypadInputState( DX_INPUT_PAD1 ) & PAD_INPUT_3)){
		input[INPUT_SLOW]++;
	} else input[INPUT_SLOW] = 0;

	if((tempKey[KEY_INPUT_X] != 0 && tempKey[KEY_INPUT_RIGHT] != 0) || (GetJoypadInputState( DX_INPUT_PAD1 ) & PAD_INPUT_6) || (GetJoypadInputState( DX_INPUT_PAD1 ) & PAD_INPUT_8)){
		input[INPUT_RIGHTDASH]++;
	} else input[INPUT_RIGHTDASH] = 0;

	if((tempKey[KEY_INPUT_X] != 0 && tempKey[KEY_INPUT_LEFT] != 0) || (GetJoypadInputState( DX_INPUT_PAD1 ) & PAD_INPUT_5) || (GetJoypadInputState( DX_INPUT_PAD1 ) & PAD_INPUT_7)){
		input[INPUT_LEFTDASH]++;
	} else input[INPUT_LEFTDASH] = 0;

	if((tempKey[KEY_INPUT_LSHIFT] != 0) || GetJoypadInputState( DX_INPUT_PAD1 ) & PAD_INPUT_2)
	{
		input[INPUT_SATECHANGE]++;
	} else input[INPUT_SATECHANGE] = 0;

	/*
	if((tempKey[KEY_INPUT_LSHIFT] != 0) || GetJoypadInputState( DX_INPUT_PAD1 ) & PAD_INPUT_2){
		input[INPUT_SATESMASH]++;
	} else input[INPUT_SATESMASH] = 0;
	*/

	if(tempKey[KEY_INPUT_C] != 0 || (GetJoypadInputState( DX_INPUT_PAD1 ) & PAD_INPUT_1)){
		input[INPUT_MENU]++;
	} else input[INPUT_MENU] = 0;
}

int Input::GetInput(int code)
{
	return input[code];
}