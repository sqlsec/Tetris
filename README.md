# Tetris
南京邮电大学Windows高级软件开发最后一次大作业，C++语言实现win32下俄罗斯方块，附上详细注释。
# 第一部分
一、将本代码仓库clone到本地
二、运行`examle/tetris.sln`代码，开发环境为`visual studio 2017`
三、对`tetris.h`和`Tetris.cpp`分别补充完整注释（在**//**后面），对如下**每一步**commit一次：
- 1、注释完tetris.h中的函数声明
- 2、注释完tetris.h中的常量声明
- 3、注释完tetris.h中的全局变量声明
- 4、注释完Tetris.cpp中全局变量和wWinMain函数内部
- 5、注释完Tetris.cpp中MyRegisterClass和InitInstance函数 内部
- 6、注释完Tetris.cpp中WndProc函数内部
- 7、注释完Tetris.cpp中其它部分
四、将本地代码仓库push到远程仓库
# 第二部分
一、将本代码仓库clone到本地
二、运行`MyTetris/Tetris.sln`代码，开发环境为`visual studio 2017`
三、参照`example/Tetris.cpp`来按照如下步骤来完善`Tetris.cpp`，对如下每一步编译通过，然后commit一次，commit信息填写对应的步骤
- 1、在项目中动态添加菜单"开始游戏、结束游戏"，在窗口处理函数WM_CREATE 中创建菜单
- 2、加入对菜单的响应，在窗口处理函数的WM_COMMAND 中处理， 以MessageBox显示点击了哪个菜单
```c
MessageBox(
        NULL,
        (LPCWSTR)L"Menu File is pressed",
        (LPCWSTR)L"Debug",
        MB_OK
         );
```
- 3、加入对键盘消息（4个方向键）的响应，WM_KEYDOWN中处理，以MessageBox显示敲击了哪个键
```c
MessageBox(
        NULL,
        (LPCWSTR)L"Key A is pressed",
        (LPCWSTR)L"Debug",
        MB_OK
        );
```
- 4、在Tetris.cpp中实现头文件中声明的函数
- 5、加入对WM_PAINT的响应，画背景
- 6、加入对WM_PAINT的响应，画NEXT
- 7、加入对WM_PAINT的响应，画SCORE
- 8、将菜单跟对应的函数对接起来，取消之前的MessageBox
- 9、将键盘跟对应的函数对接起来，取消之前的MessageBox
- 10、添加菜单“旋转方向”，参考example/Tetris.cpp中205行~216行
```c
        rotate = CreatePopupMenu();//
        AppendMenu(hSysmenu, MF_POPUP, (UINT_PTR)rotate, L"旋转选择");//
		AppendMenu(rotate, MF_STRING, ID_ROATE1, L"顺时针");//
		AppendMenu(rotate, MF_STRING, ID_ROATE2, L"逆时针");//
		SetMenu(hWnd, rotate);//	
```
- 11、添加菜单“顺时针”和“逆时针”的响应
```c
//参考Tetris.cpp中239~244行
        case ID_ROATE1:
			selectRotate(hWnd, 0);//顺时针
			break;
		case ID_ROATE2:
			selectRotate(hWnd, 1);//逆时针
			break;
```
- 12、修改Tetris.cpp525行RotateTeris的实现代码，根据GAME_ROTATE的值来确定旋转方向，0：顺时针，1：逆时针
- 13、填写"MyTetris/大作业报告.doc" 按照该模板撰写大作业报告(需要填写“学号”、“姓名”和报告正文，报告中不需要代码截图，也不需要运行结果截图)
四、将本地代码仓库push到远程仓库