#pragma once

#include "resource.h"

//函数声明
void checkComplite();  //查看是否能消去 
void drawBlocked(HDC hdc);      //绘制当前已经存在砖块的区域
void DrawBackGround(HDC hdc);       //绘制背景
void outPutBoxInt(int num);     //弹窗信息
void outPutBoxString(TCHAR str[1024]);
void setRandomT();      //随机生成一个方块用作下一次掉落
void init_game();       //初始化游戏
void fillBlock();       //到达底部后填充
void RotateTeris(BOOL bTeris[4][4]);        //旋转矩阵
void DrawTeris(HDC mdc);    //绘制正在下落的方块
void drawNext(HDC hdc); //绘制下一个将要掉落的方块
void drawScore(HDC hdc);    //绘制分数
void drawCompleteParticle(int line);

int RandomInt(int _min, int _max);       //获取一个随机数
int CheckValide(int curPosX, int curPosY, BOOL bCurTeris[4][4]);   //给定一个矩阵，查看是否合法
int selectDiffculty(HWND hWnd, int dif);
int selectLayOut(HWND hWnd, int layout);

//常量声明
const int BORDER_X = 10;//定义边框长度为10
const int BORDER_Y = 10;//定义边框宽度为10
const int SCREEN_LEFT_X = 300 + BORDER_X;//左半部分310
const int SCREEN_Y = 600 + BORDER_Y;//总高610
const int SCREEN_RIGHT_X = 180 + BORDER_X * 2;//最右侧右半部分200
const int SCREEN_X = SCREEN_LEFT_X + SCREEN_RIGHT_X;//总长度510
const BOOL state_teris[][4][4] =
{
	{ { 1,1,1,1 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 0,1,1,0 },{ 0,1,1,0 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 0,1,1,1 },{ 0,0,0,1 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 0,1,1,0 },{ 0,0,1,1 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 0,1,0,0 },{ 1,1,1,0 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 0,1,1,1 },{ 0,1,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 0,1,1,0 },{ 1,1,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } }
};//定义了俄罗斯方块的一些状态



//全局变量声明
bool g_hasBlocked[50][50];//被填充的矩阵
RECT rc_left, rc_right, rc_right_top, rc_right_bottom, rc_main;//定义各个模块的方块
int g_speed = 300;//间隔时间
int t_speed = 300;//间隔时间
BOOL bCurTeris[4][4];//当前方块
BOOL bNextCurTeris[4][4];//下一个方块
int curPosX, curPosY;//方块位置
int rd_seed = 1995421;//随机数种子
int tPre = 0, tCur;//时间
int GAME_STATE = 0;//游戏状态
int GAME_SCORE = 0;//分数
int GAME_DIFF = 1;//难度
int NUM_X = 10;//最多10个横的
int NUM_Y = 20;//最多20个竖的
int BLOCK_SIZE = 30;//方块大小