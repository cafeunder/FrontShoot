#ifndef MY_MAIN_DEF
#define MY_MAIN_DEF

//ÉVÉXÉeÉÄèÓïÒÇÇ±Ç±Ç…Ç©Ç≠

//#define DEBUG_CODE

#include "assert.h"

#define WINDOWSIZE_X 640
#define WINDOWSIZE_Y 480
#define GAME_WIDTH 380
#define GAME_HEIGHT 480
#define GAME_X	20
#define GAME_Y	((WINDOWSIZE_Y-GAME_HEIGHT)/2)
#define PI 3.1415926535897932384626

void _DrawBox(int x1, int y1, int x2, int y2, int Color, bool FillFlag);

#endif