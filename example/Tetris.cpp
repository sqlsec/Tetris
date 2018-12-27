#include "stdafx.h"
#include "tetris.h"
#include <windows.h>
#include <mmsystem.h>//包含windows中与多媒体有关的大多数接口
#pragma comment(lib, "WINMM.LIB")//导入winmm库：WINMM.LIB是Windows多媒体相关应用程序接口
#define MAX_LOADSTRING 100 

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);//窗口注册函数
BOOL                InitInstance(HINSTANCE, int);//在启动时初始化应用程序
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);//窗口过程处理函数
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// 全局变量: 
HINSTANCE hInst;                               //当前实例 
WCHAR szTitle[MAX_LOADSTRING];                  //标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            //主窗口类名
HMENU diff;//难度菜单
HMENU lay;//布局菜单

int APIENTRY wWinMain(HINSTANCE hInstance,//应用程序当前实例
	HINSTANCE hPrevInstance,//应用程序其他实例
	LPWSTR    lpCmdLine,//指向程序命令行参数的指针
	int       nCmdShow)//应用程序开始执行时窗口显示方式的整数值标识
{
	init_game();//初始化游戏
	UNREFERENCED_PARAMETER(hPrevInstance);//展开传递的参数或表达式
	UNREFERENCED_PARAMETER(lpCmdLine);//避免编译器关于未引用参数的警告

									  //初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_TETRIS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);//注册窗口类，用于兼容

							   //执行应用程序初始化
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TETRIS));//

	MSG msg;

	//主消息循环: 
	while (1)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))//消息检查线程消息队列
		{
			TranslateMessage(&msg);//用于将虚拟键消息转换为字符消息
			DispatchMessage(&msg);//该函数分发一个消息给窗口程序
			if (msg.message == WM_QUIT)//退出
			{
				break;
			}
		}
		else
		{
			if ((GAME_STATE & 2) != 0)
			{
				tCur = GetTickCount();//返回（retrieve）从操作系统启动所经过（elapsed）的毫秒数
				if (tCur - tPre>g_speed)//超过反应时间时
				{
					int flag = CheckValide(curPosX, curPosY + 1, bCurTeris);//判断下一行的情况，返回给flag
					if (flag == 1)//下降一行
					{
						curPosY++;//方块坐标y加1
						tPre = tCur;//重新计算反应时间
						HWND hWnd = GetActiveWindow();
						//这里两次调用实现了双缓冲的作用，改善了视觉效果
						InvalidateRect(hWnd, &rc_left, FALSE);//向指定的窗体更新区域添加一个矩形，然后窗口客户区域的这一部分将被重新绘制
						InvalidateRect(hWnd, &rc_right_top, FALSE);//系统不会像窗口发送WM_PAINT消息，来重绘
					}
					else if (flag == -2)//到底时
					{
						g_speed = t_speed;//重置速度
						fillBlock();//填充方块
						checkComplite(); //查看能否消去
						setRandomT();//随机产生新的方块
						curPosX = (NUM_X - 4) >> 1;//重新设置方块的坐标x
						curPosY = 0;//重新设置方块的坐标y
						HWND hWnd = GetActiveWindow();//获得与调用线程的消息队列相关的活动窗口的窗口句柄
						InvalidateRect(hWnd, &rc_main, FALSE);//刷新窗口
					}
					else if (flag == -3)//游戏结束
					{
						HWND hWnd = GetActiveWindow();//获得与调用线程的消息队列相关的活动窗口的窗口句柄
						if (MessageBox(hWnd, L"胜败乃兵家常事，菜鸡请重新来过", L":时光机", MB_YESNO) == IDYES)//
						{
							init_game();//初始化游戏
						}
						else
						{
							break;//退出
						}
					}
				}
			}
		}
	}

	return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;//定义窗口类对象

	wcex.cbSize = sizeof(WNDCLASSEX);//窗口大小
	wcex.style = CS_HREDRAW | CS_VREDRAW;//窗口风格
	wcex.lpfnWndProc = WndProc;//过程处理函数
	wcex.cbClsExtra = 0;//窗口类无扩展
	wcex.cbWndExtra = 0;//窗口实例无扩展
	wcex.hInstance = hInstance;//当前实例句柄
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TETRIS));//窗口的图标为默认图标
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);//箭头光标
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//白色背景
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TETRIS);//使用系统自定义的菜单资源如文件，关于
	wcex.lpszClassName = szWindowClass;//设置窗口类名
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));//小图标为默认图标

	return RegisterClassExW(&wcex);//返回注册窗口
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; //将实例句柄存储在全局变量中 创建窗口
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	//WS_OVERLAPPEDWINDOW：创建一带边框、标题栏、系统菜单及最大最小化按钮的窗口 
	//CW_USEDEFAULT:系统选择的默认位置窗口的左上角,而忽略了y参数
	//nullptr:空指针类型的文字
	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);//显示窗口
	UpdateWindow(hWnd);//更新窗口，即绘制新的用户区域

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;//包含某应用程序用来绘制他所拥有的窗口客户区所需要的信息
	HDC hdc;//标识设备环境句柄
	int nWinx, nWiny, nClientX, nClientY;
	int posX, posY;
	RECT rect;//存储成对出现的参数
	HMENU hSysmenu;//菜单窗口的句柄
	switch (message)
	{
	case WM_CREATE://当一个应用程序通过CreateWindowEx函数或者CreateWindow函数请求创建窗口时发送此消息
		GetWindowRect(hWnd, &rect);
		//窗口的位置
		nWinx = 530;//窗口宽度
		nWiny = 680;//窗口高度
		posX = GetSystemMetrics(SM_CXSCREEN);//获取屏幕宽度
		posY = GetSystemMetrics(SM_CYSCREEN);//获取屏幕高度
		posX = (posX - nWinx) >> 1;//计算出居中放置的X位置
		posY = (posY - nWiny) >> 1;//计算出居中放置的Y位置
		GetClientRect(hWnd, &rect);//获取窗口大小
		nClientX = rect.right - rect.left;//客户端宽度
		nClientY = rect.bottom - rect.top;//客户端高度

		MoveWindow(hWnd, posX, posY, 530, 680, TRUE);//设置窗口位置和大小
		hSysmenu = GetMenu(hWnd);//获取菜单句柄
		AppendMenu(hSysmenu, MF_SEPARATOR, 0, NULL);//添加一条水平分割线
													//此处的菜单句柄要全局向前初始化
		diff = CreatePopupMenu();//创建一个难度菜单
		AppendMenu(hSysmenu, MF_POPUP, (UINT_PTR)diff, L"难度选择");//添加菜单diff
		AppendMenu(diff, MF_STRING, ID_dif1, L"难度1");//创建难度1子菜单
		AppendMenu(diff, MF_STRING, ID_dif2, L"难度2");//创建难度2子菜单
		AppendMenu(diff, MF_STRING, ID_dif3, L"难度3");//创建难度3子菜单
		lay = CreatePopupMenu();//创建一个布局菜单
		AppendMenu(hSysmenu, MF_POPUP, (UINT_PTR)lay, L"布局选择");//添加菜单lay
		AppendMenu(lay, MF_STRING, ID_LAYOUT1, L"布局1");//创建布局1子菜单
		AppendMenu(lay, MF_STRING, ID_LAYOUT2, L"布局2");//创建布局2子菜单
		SetMenu(hWnd, hSysmenu);//设置系统自定义菜单
		SetMenu(hWnd, diff);//设置新添的难度菜单
		SetMenu(hWnd, lay);//设置新添的布局菜单
		break;
	case WM_COMMAND://当用户从菜单选中一个命令项目、当一个控件发送通知消息给去父窗口或者按下一个快捷键将发送 WM_COMMAND 消息
		wmId = LOWORD(wParam);//从此宏从指定的32位值，检索低顺序单词
		wmEvent = HIWORD(wParam);//获取高位字节
								 //分析菜单选择
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);//弹出关于消息框
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);//退出程序
			break;
		case ID_dif1:
			selectDiffculty(hWnd, 1);//难度1
			break;
		case ID_dif2:
			selectDiffculty(hWnd, 2);//难度2
			break;
		case ID_dif3:
			selectDiffculty(hWnd, 3);//难度3
			break;
		case ID_LAYOUT1:
			selectLayOut(hWnd, 1);//布局1
			break;
		case ID_LAYOUT2:
			selectLayOut(hWnd, 2);//布局2
			break;
		default://该函数调用缺省的窗口过程来为应用程序没有处理的任何窗口提供缺省的处理,该函数确保每一个消息得到处理
			return DefWindowProc(hWnd, message, wParam, lParam);//调用缺省的窗口过程来为应用程序没有处理的任何窗口消息提供缺省的处理
		}
		break;
	case WM_KEYDOWN://键盘消息
		hdc = GetDC(hWnd);
		InvalidateRect(hWnd, NULL, false);
		switch (wParam)
		{
		case VK_LEFT:
			curPosX--;
			if (CheckValide(curPosX, curPosY, bCurTeris) != 1)
			{
				curPosX++;//左移
			}
			break;
		case VK_RIGHT:
			curPosX++;
			if (CheckValide(curPosX, curPosY, bCurTeris) != 1)
			{
				curPosX--;//右移
			}
			break;
		case VK_UP:
			RotateTeris(bCurTeris);//旋转改变形状
			break;
		case VK_DOWN://加速到底
			if (g_speed == t_speed)
				g_speed = 10;
			else
				g_speed = t_speed;
			//outPutBoxInt(g_speed);
			break;
		case 'W'://同 VK_UP
			RotateTeris(bCurTeris);
			break;
		case 'A'://同 VK_LEFT
			curPosX--;
			if (CheckValide(curPosX, curPosY, bCurTeris) != 1)
			{
				curPosX++;
			}
			break;
		case 'D'://同 VK_RIGHT
			curPosX++;
			if (CheckValide(curPosX, curPosY, bCurTeris) != 1)
			{
				curPosX--;
			}
			break;

		case 'S'://同 VK_DOWN
			if (g_speed == t_speed)
				g_speed = 10;
			else
				g_speed = t_speed;
			//outPutBoxInt(g_speed);
			break;
		default:
			break;
		}

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		DrawBackGround(hdc);//绘制背景
		EndPaint(hWnd, &ps);//停止绘制
		break;
	case WM_DESTROY://退出
		PostQuitMessage(0);
		break;
	default:
		//函数调用缺省的窗口过程来为应用程序没有处理的任何窗口消息提供缺省的处理。该函数确保每一个消息得到处理
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));//该函数清除一个模态对话框,并使系统中止对对话框的任何处理
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void drawBlocked(HDC mdc)//绘制当前已经存在砖块的区域
{
	int i, j;
	//在应用程序调用CreateSolidBrush创建刷子以后，可以通过调用SelectObject函数把该刷子选入设备环境
	HBRUSH hBrush = (HBRUSH)CreateSolidBrush(RGB(255, 255, 0));//画刷句柄，黄色

	SelectObject(mdc, hBrush);//函数选择一对象到指定的设备上下文环境中，新对象替换先前的相同类型的对象

	for (i = 0; i<NUM_Y; i++)
	{
		for (j = 0; j<NUM_X; j++)
		{
			if (g_hasBlocked[i][j])
			{
				Rectangle(mdc, BORDER_X + j*BLOCK_SIZE, BORDER_Y + i*BLOCK_SIZE,
					BORDER_X + (j + 1)*BLOCK_SIZE, BORDER_Y + (i + 1)*BLOCK_SIZE
				);//绘画矩形的函数，并用当前颜色的画刷进行颜色的填充
			}
		}
	}
	DeleteObject(hBrush);//该函数删除一个逻辑笔、画笔、字体、 位图、区域或者调色板，释放所有与该对象有关的系统资源
}

int CheckValide(int startX, int startY, BOOL bTemp[4][4])//给定一个矩阵，查看是否合法
{
	int i, j;
	for (i = 3; i >= 0; i--)
	{
		for (j = 3; j >= 0; j--)
		{
			if (bTemp[i][j])
			{
				if (j + startX<0 || j + startX >= NUM_X)
				{
					return -1;
				}
				if (i + startY >= NUM_Y)
				{
					return -2;
				}
				if (g_hasBlocked[i + startY][j + startX])
				{
					//outPutBoxInt(j+startY);
					if (curPosY == 0)
					{
						return -3;
					}
					return -2;
				}
			}
		}
	}
	//MessageBox(NULL,L"这里",L"as",MB_OK);
	//outPutBoxInt(curPosY);
	return 1;
}

void checkComplite()//查看一行是否能消去
{
	int i, j, k, count = 0;
	for (i = 0; i<NUM_Y; i++)
	{
		bool flag = true;
		for (j = 0; j<NUM_X; j++)
		{
			if (!g_hasBlocked[i][j])
			{
				flag = false;//有一个为空时，不能消去
			}
		}
		if (flag)
		{
			count++;
			for (j = i; j >= 1; j--)//从上往下把每行下移
			{
				for (k = 0; k<NUM_X; k++)
				{
					g_hasBlocked[j][k] = g_hasBlocked[j - 1][k];
				}

			}
			drawCompleteParticle(i);//在消去的那行画一行空白做出消去的效果
			Sleep(300);//暂停300ms

			PlaySound(_T("coin.wav"), NULL, SND_FILENAME | SND_ASYNC);//消去一行时播放系统自带提示音
		}
	}
	GAME_SCORE += int(count*1.5);//单行+1分，两行+3分，三行+4分，四行6分，以此类推
}

VOID outPutBoxInt(int num)//自定义的弹窗函数  用于调试，已经被注释掉了
{
	TCHAR szBuf[1024];
	LPCTSTR str = TEXT("%d");
	wsprintf(szBuf, str, num);//将一系列的字符和数值输入到缓冲区。输出缓冲区里的值取决于格式说明符（即"%"）
	MessageBox(NULL, szBuf, L"aasa", MB_OK);//显示消息框
}

VOID outPutBoxString(TCHAR str[1024])//自定义的弹窗函数  用于调试，已经被注释掉了
{
	TCHAR szBuf[1024];
	LPCTSTR cstr = TEXT("%s");
	wsprintf(szBuf, cstr, str);
	MessageBox(NULL, szBuf, L"aasa", MB_OK);
}



//设置随机方块形状
void setRandomT()
{
	int rd_start = RandomInt(0, sizeof(state_teris) / sizeof(state_teris[0]));
	int rd_next = RandomInt(0, sizeof(state_teris) / sizeof(state_teris[0]));
	//outPutBoxInt(rd_start);
	//outPutBoxInt(rd_next);
	//outPutBoxInt(rd_start);
	if (GAME_STATE == 0)
	{
		GAME_STATE = GAME_STATE | 0x0001;
		//outPutBoxInt(GAME_STATE);
		memcpy(bCurTeris, state_teris[rd_start], sizeof(state_teris[rd_start]));
		memcpy(bNextCurTeris, state_teris[rd_next], sizeof(state_teris[rd_next]));
	}
	else
	{
		memcpy(bCurTeris, bNextCurTeris, sizeof(bNextCurTeris));
		memcpy(bNextCurTeris, state_teris[rd_next], sizeof(state_teris[rd_next]));
	}

}

//游戏初始化
void init_game()
{
	GAME_SCORE = 0;
	setRandomT();//随机生成一个方块用作下一次掉落
	curPosX = (NUM_X - 4) >> 1;//设置初始坐标
	curPosY = 0;
	//memset，是计算机语言中的函数。起功能是将s所指向的某一块内存中的每个字节的内容全部设置为ch指定的ASCII值，最后行清零
	memset(g_hasBlocked, 0, sizeof(g_hasBlocked));
	rc_left.left = 0;
	rc_left.right = SCREEN_LEFT_X;
	rc_left.top = 0;
	rc_left.bottom = SCREEN_Y;

	rc_right.left = rc_left.right + BORDER_X;
	rc_right.right = 180 + rc_right.left;
	rc_right.top = 0;
	rc_right.bottom = SCREEN_Y;

	rc_main.left = 0;
	rc_main.right = SCREEN_X;
	rc_main.top = 0;
	rc_main.bottom = SCREEN_Y;

	rc_right_top.left = rc_right.left;
	rc_right_top.top = rc_right.top;
	rc_right_top.right = rc_right.right;
	rc_right_top.bottom = (rc_right.bottom) / 2;

	rc_right_bottom.left = rc_right.left;
	rc_right_bottom.top = rc_right_top.bottom + BORDER_Y;
	rc_right_bottom.right = rc_right.right;
	rc_right_bottom.bottom = rc_right.bottom;

	g_speed = t_speed = 1000 - GAME_DIFF * 280;
}

void fillBlock()//到达底部后填充矩阵
{
	int i, j;
	for (i = 0; i<4; i++)
	{
		for (j = 0; j<4; j++)
		{
			if (bCurTeris[i][j])
			{
				g_hasBlocked[curPosY + i][curPosX + j] = 1;
			}
		}
	}
}

void RotateTeris(BOOL bTeris[4][4])
{
	BOOL bNewTeris[4][4];
	int x, y;
	for (x = 0; x<4; x++)
	{
		for (y = 0; y<4; y++)
		{//旋转角度
			//bNewTeris[x][y] = bTeris[3 - y][x];
			//逆时针：
			bNewTeris[x][y] = bTeris[y][3-x];
		}
	}
	if (CheckValide(curPosX, curPosY, bNewTeris) == 1)
	{
		memcpy(bTeris, bNewTeris, sizeof(bNewTeris));//成功则将变换后的形状保存
	}

}

int RandomInt(int _min, int _max)
{
	srand((rd_seed++) % 65532 + GetTickCount());
	return _min + rand() % (_max - _min);
}

VOID DrawTeris(HDC mdc)
{

	int i, j;
	HPEN hPen = (HPEN)GetStockObject(BLACK_PEN);//画笔句柄
	HBRUSH hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	SelectObject(mdc, hPen);//SelectObject是计算机编程语言函数，该函数选择一对象到指定的设备上下文环境中，新对象替换先前的相同类型的对象
	SelectObject(mdc, hBrush);
	for (i = 0; i<4; i++)
	{
		for (j = 0; j<4; j++)
		{
			if (bCurTeris[i][j])
			{
				Rectangle(mdc, (j + curPosX)*BLOCK_SIZE + BORDER_X, (i + curPosY)*BLOCK_SIZE + BORDER_Y, (j + 1 + curPosX)*BLOCK_SIZE + BORDER_X, (i + 1 + curPosY)*BLOCK_SIZE + BORDER_Y);
			}
		}
	}
	drawBlocked(mdc);
	DeleteObject(hPen);
	DeleteObject(hBrush);
}

VOID DrawBackGround(HDC hdc)//绘制背景
{

	HBRUSH hBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
	//适用于支持光栅操作的设备，应用程序可以通过调用GetDeviceCaps函数来确定一个设备是否支持这些操作
	HDC mdc = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, SCREEN_X, SCREEN_Y);

	SelectObject(mdc, hBrush);
	SelectObject(mdc, hBitmap);

	HBRUSH hBrush2 = (HBRUSH)GetStockObject(WHITE_BRUSH);
	FillRect(mdc, &rc_main, hBrush2);
	Rectangle(mdc, rc_left.left + BORDER_X, rc_left.top + BORDER_Y, rc_left.right, rc_left.bottom);
	Rectangle(mdc, rc_right.left + BORDER_X, rc_right.top + BORDER_Y, rc_right.right, rc_right.bottom);
	DrawTeris(mdc);
	drawNext(mdc);
	drawScore(mdc);
	::BitBlt(hdc, 0, 0, SCREEN_X, SCREEN_Y, mdc, 0, 0, SRCCOPY);
	DeleteObject(hBrush);
	DeleteDC(mdc);
	DeleteObject(hBitmap);
	DeleteObject(hBrush2);


	//  int x,y;
	//  HPEN hPen = (HPEN)GetStockObject(NULL_PEN);
	//  HBRUSH hBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
	//  SelectObject(hdc,hPen);
	//  SelectObject(hdc,hBrush);
	//  for (x = 0;x<NUM_X;x++)
	//  {
	//      for(y=0;y<NUM_Y;y++)
	//      {
	//          Rectangle(hdc,BORDER_X+x*BLOCK_SIZE,BORDER_Y+y*BLOCK_SIZE,
	//              BORDER_X+(x+1)*BLOCK_SIZE,
	//              BORDER_Y+(y+1)*BLOCK_SIZE);
	//      }
	//  }
}

void drawNext(HDC hdc)//绘制下一个将要掉落的方块
{
	int i, j;
	HBRUSH hBrush = (HBRUSH)CreateSolidBrush(RGB(188, 0, 0));
	HPEN hPen = (HPEN)CreatePen(PS_SOLID, 2, RGB(0, 0, 233));
	SelectObject(hdc, hBrush);
	SelectObject(hdc, hPen);
	for (i = 0; i<4; i++)
	{
		for (j = 0; j<4; j++)
		{
			if (bNextCurTeris[i][j])
			{
				Rectangle(hdc, rc_right_top.left + BLOCK_SIZE*(j + 1), rc_right_top.top + BLOCK_SIZE*(i + 1), rc_right_top.left + BLOCK_SIZE*(j + 2), rc_right_top.top + BLOCK_SIZE*(i + 2));
			}
		}
	}
	HFONT hFont = CreateFont(30, 0, 0, 0, FW_THIN, 0, 0, 0, UNICODE, 0, 0, 0, 0, L"微软雅黑");
	SelectObject(hdc, hFont);
	SetBkMode(hdc, TRANSPARENT);
	SetBkColor(hdc, RGB(255, 255, 0));
	SetTextColor(hdc, RGB(152, 206, 0));
	RECT rect;
	rect.left = rc_right_top.left + 40;
	rect.top = rc_right_top.bottom - 150;
	rect.right = rc_right_top.right;
	rect.bottom = rc_right_top.bottom;
	DrawTextW(hdc, TEXT("下一个"), _tcslen(TEXT("下一个")), &rect, 0);
	DeleteObject(hFont);
	DeleteObject(hBrush);
}

void drawScore(HDC hdc)
{
	HFONT hFont = CreateFont(30, 0, 0, 0, FW_THIN, 0, 0, 0, UNICODE, 0, 0, 0, 0, L"微软雅黑");
	SelectObject(hdc, hFont);
	SetBkMode(hdc, TRANSPARENT);
	SetBkColor(hdc, RGB(255, 255, 0));
	SetTextColor(hdc, RGB(0, 200, 150));
	RECT rect;
	rect.left = rc_right_bottom.left;
	rect.top = rc_right_bottom.top;
	rect.right = rc_right_bottom.right;
	rect.bottom = rc_right_bottom.bottom;
	TCHAR szBuf[30];
	LPCTSTR cstr = TEXT("当前难度：%d");
	wsprintf(szBuf, cstr, GAME_DIFF);
	DrawTextW(hdc, szBuf, _tcslen(szBuf), &rect, DT_CENTER | DT_VCENTER);

	RECT rect2;
	rect2.left = rc_right_bottom.left;
	rect2.top = rc_right_bottom.bottom / 2 + 100;
	rect2.right = rc_right_bottom.right;
	rect2.bottom = rc_right_bottom.bottom;
	TCHAR szBuf2[30];
	LPCTSTR cstr2 = TEXT("得分：%d");
	wsprintf(szBuf2, cstr2, GAME_SCORE);
	//outPutBoxInt(sizeof(szBuf));
	SetTextColor(hdc, RGB(15, 0, 150));
	DrawTextW(hdc, szBuf2, _tcslen(szBuf2), &rect2, DT_CENTER | DT_VCENTER);

	DeleteObject(hFont);
}

int selectDiffculty(HWND hWnd, int diff)
{
	TCHAR szBuf2[30];
	LPCTSTR cstr2 = TEXT("确认选择难度 %d 吗？");
	wsprintf(szBuf2, cstr2, diff);
	if (MessageBox(hWnd, szBuf2, L"难度选择", MB_YESNO) == IDYES)
	{
		GAME_DIFF = diff;
		InvalidateRect(hWnd, &rc_right_bottom, false);
		GAME_STATE |= 2;
		init_game();
		return GAME_DIFF;
	}
	return -1;
}

int selectLayOut(HWND hWnd, int layout)//选择布局
{
	NUM_X = 10 * layout;
	NUM_Y = 20 * layout;
	BLOCK_SIZE = 30 / layout;
	GAME_STATE |= 2;
	InvalidateRect(hWnd, &rc_right_bottom, false);
	init_game();
	return layout;
}

void drawCompleteParticle(int line)
{
	HWND hWnd = GetActiveWindow();
	HDC hdc = GetDC(hWnd);
	HBRUSH hBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
	HPEN hPen = (HPEN)CreatePen(PS_DOT, 1, RGB(255, 255, 0));
	SelectObject(hdc, hBrush);
	SelectObject(hdc, hPen);
	Rectangle(hdc, BORDER_X,
		BORDER_Y + line*BLOCK_SIZE,
		BORDER_X + NUM_X*BLOCK_SIZE,
		BORDER_Y + BLOCK_SIZE*(1 + line));
	DeleteObject(hBrush);
	DeleteObject(hPen);
}