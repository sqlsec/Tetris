#pragma once

#include "resource.h"

//函数声明
void checkComplite();  //  
void drawBlocked(HDC hdc);      //
void DrawBackGround(HDC hdc);       //
void outPutBoxInt(int num);     //
void outPutBoxString(TCHAR str[1024]);
void setRandomT();      //
void init_game();       //
void fillBlock();       //
void RotateTeris(BOOL bTeris[4][4]);        //
void DrawTeris(HDC mdc);    //
void drawNext(HDC hdc); //
void drawScore(HDC hdc);    //
void drawCompleteParticle(int line);

int RandomInt(int _min, int _max);       //
int CheckValide(int curPosX, int curPosY, BOOL bCurTeris[4][4]);   //
int selectDiffculty(HWND hWnd, int dif);
int selectLayOut(HWND hWnd, int layout);
int selectRotate(HWND hWnd, int direction);

//常量声明
const int BORDER_X = 10;//
const int BORDER_Y = 10;//
const int SCREEN_LEFT_X = 300 + BORDER_X;//
const int SCREEN_Y = 600 + BORDER_Y;//
const int SCREEN_RIGHT_X = 180 + BORDER_X * 2;//
const int SCREEN_X = SCREEN_LEFT_X + SCREEN_RIGHT_X;//
const BOOL state_teris[][4][4] =
{
	{ { 1,1,1,1 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 0,1,1,0 },{ 0,1,1,0 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 0,1,1,1 },{ 0,0,0,1 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 0,1,1,0 },{ 0,0,1,1 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 0,1,0,0 },{ 1,1,1,0 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 0,1,1,1 },{ 0,1,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 0,1,1,0 },{ 1,1,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } }
};//



//全局变量声明
bool g_hasBlocked[50][50];//
RECT rc_left, rc_right, rc_right_top, rc_right_bottom, rc_main;//
int g_speed = 300;//
int t_speed = 300;//
BOOL bCurTeris[4][4];//
BOOL bNextCurTeris[4][4];//
int curPosX, curPosY;//
int rd_seed = 1995421;//
int tPre = 0, tCur;//
int GAME_STATE = 0;//
int GAME_SCORE = 0;//
int GAME_DIFF = 1;//
int NUM_X = 10;//
int NUM_Y = 20;//
int BLOCK_SIZE = 30;//
int GAME_ROTATE = 0;