#include "GameEngine.h"

//静态变量初始
GameEngine *GameEngine::m_pGameEngine = NULL;

//windows函数
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR szCmdLine, int iCmdShow){
	MSG msg;
	static int iTickTrigger = 0;
	int iTickCount;
	if(GameInitialize(hInstance)){
		//初始化游戏引擎
		if(!GameEngine::GetEngine()->Initialize(iCmdShow)){
			return false;
		}

		//进入主消息循环
		while(TRUE){
			if(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
				//处理消息
				if(msg.message == WM_QUIT){
					break;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}else{
				//确保游戏引擎没有休眠
				if(!GameEngine::GetEngine()->GetSleep()){
					//检查滴答计数，查看是否过了一个游戏周期
					/*
					note:这里看不太懂，为什么iTickTrigger在iTickCount上不断加一个FrameDelay。
					iTickCount和iTickTrigger为什么不用模运算呢？
					*/
					iTickCount = GetTickCount();
					if(iTickCount > iTickTrigger){
						iTickTrigger = iTickCount + GameEngine::GetEngine()->GetFrameDelay();
						HandleKeys();
						GameCycle();
					}
				}
			}
		}
		return (int)msg.wParam;
	}
	GameEnd();
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam){
	//将所有windows消息都传递给游戏引擎
	return GameEngine::GetEngine()->HandleEvent(hWindow,msg,wParam,lParam);

}

/*
GameEngine的构造函数、析构函数
*/
GameEngine::GameEngine(HINSTANCE hInstance,LPTSTR szWindowClass,LPTSTR szTitle, WORD wIcon,WORD wSmallIcon, int iWidth, int iHeight){
	//设置游戏引擎的成员变量
	m_pGameEngine = this;
	m_hInstance = hInstance;
	/*
	note: why m_hWindow = NULL?
	*/
	m_hWindow = NULL;
	if(lstrlen(szWindowClass)>0){
		lstrcpy(m_szWindowClass, szWindowClass);
	}
	if(lstrlen(szTitle)>0){
		lstrcpy(m_szTitle,szTitle);
	}
	m_wIcon = wIcon;
	m_wSmallIcon = wSmallIcon;
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_iFrameDelay = 50; //默认为20帧/秒
	m_bSleep = TRUE;
}
GameEngine::~GameEngine(){

}

/*
游戏引擎常规方法
*/
BOOL GameEngine::Initialize(int iCmdShow){
	WNDCLASSEX wndclass;
	//创建主窗口的窗口类
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = m_hInstance;
	wndclass.hIcon = LoadIcon(m_hInstance,MAKEINTRESOURCE(GetIcon()));
	wndclass.hIconSm = LoadIcon(m_hInstance,MAKEINTRESOURCE(GetSmallIcon()));
	wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = m_szWindowClass;

	//注册窗口类
	if(!RegisterClassEx(&wndclass))
		return FALSE;

	//根据游戏大小计算窗口大小和位置
	int iWindowWidth = m_iWidth + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
	int iWindowHeight = m_iHeight + GetSystemMetrics(SM_CYFIXEDFRAME) * 2;
	if(wndclass.lpszMenuName != NULL) iWindowHeight += GetSystemMetrics(SM_CYMENU);
	int iXWindowPos = (GetSystemMetrics(SM_CXSCREEN)-iWindowWidth)/2;
	int iYWindowPos = (GetSystemMetrics(SM_CYSCREEN)-iWindowHeight)/2;

	//创建窗口
	m_hWindow = CreateWindow(m_szWindowClass,m_szTitle,WS_POPUPWINDOW|
		WS_CAPTION|WS_MINIMIZEBOX, iXWindowPos,iYWindowPos, iWindowWidth,iWindowHeight, NULL,NULL, m_hInstance, NULL);
	if(!m_hWindow) return FALSE;

	ShowWindow(m_hWindow,iCmdShow);
	UpdateWindow(m_hWindow);

	return TRUE;
}

LRESULT GameEngine::HandleEvent(HWND hWindow, UINT msg, WPARAM wParam,LPARAM lParam){
	//将Windows消息传递给游戏引擎成员函数
	switch(msg){
	case WM_CREATE:
		//设置游戏窗口并开始游戏
		SetWindow(hWindow);
		GameStart(hWindow);
		return 0;
	case WM_SETFOCUS:
		//激活游戏并更新休眠状态
		GameActivate(hWindow);
		SetSleep(FALSE);
		return 0;
	case WM_KILLFOCUS:
		//停用游戏并更新休眠状态
		GameDeactivate(hWindow);
		SetSleep(TRUE);
		return 0;
	case WM_PAINT:
		HDC hDC;
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWindow,&ps);

		//绘制游戏
		GamePaint(hDC);

		EndPaint(hWindow,&ps);
		return 0;
		//鼠标事件
	case WM_LBUTTONDOWN:
		MouseButtonDown(LOWORD(lParam),HIWORD(lParam),TRUE);
		return 0;
	case WM_LBUTTONUP:
		MouseButtonUp(LOWORD(lParam),HIWORD(lParam),TRUE);
	case WM_RBUTTONDOWN:
		MouseButtonDown(LOWORD(lParam),HIWORD(lParam),FALSE);
		return 0;
	case WM_RBUTTONUP:
		MouseButtonUp(LOWORD(lParam),HIWORD(lParam),FALSE);
		return 0;
	case WM_MOUSEMOVE:
		MouseMove(LOWORD(lParam),HIWORD(lParam));
		return 0;
	case WM_DESTROY:
		//结束游戏并退出应用程序
		GameEnd();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWindow,msg,wParam,lParam);
}