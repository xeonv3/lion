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
//-----------------------------------------------------------------
// Game Engine Helper Methods
//-----------------------------------------------------------------
BOOL GameEngine::CheckSpriteCollision(Sprite* pTestSprite)
{
	// See if the sprite has collided with any other sprites
	list<Sprite*>::iterator siSprite;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
	{
		// Make sure not to check for collision with itself
		if (pTestSprite == (*siSprite))
			continue;

		// Test the collision
		if (pTestSprite->TestCollision(*siSprite))
			// Collision detected
				return SpriteCollision((*siSprite), pTestSprite);
	}
	// No collision
	return FALSE;
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
	m_uiMIDIPlayerID = 0;
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
	int iWindowHeight = m_iHeight + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);;
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


void GameEngine::AddSprite(Sprite* pSprite){
	// Add a sprite to the sprite list
	if (pSprite != NULL){
		// See if there are sprites already in the sprite list
		if (m_vSprites.size() > 0){
			// Find a spot in the sprite list to insert the sprite by its z-order
			list<Sprite*>::iterator siSprite;
			for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++){
				if (pSprite->GetZOrder() < (*siSprite)->GetZOrder()){
					// Insert the sprite into the sprite vector
					m_vSprites.insert(siSprite, pSprite);
					return;
				}
			}
		}

		// The sprite's z-order is highest, so add it to the end of the list
		m_vSprites.push_back(pSprite);
	}
}

void GameEngine::DrawSprites(HDC hDC)
{
	// Draw the sprites in the sprite list
	list<Sprite*>::iterator siSprite;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
		(*siSprite)->Draw(hDC);
}

void GameEngine::UpdateSprites()
{
	// Update the sprites in the sprite list
	RECT rcOldSpritePos;
	SPRITEACTION  saSpriteAction;
	list<Sprite*>::iterator siSprite;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end();)
	{
		// Save the old sprite position in case we need to restore it
		rcOldSpritePos = (*siSprite)->GetPosition();

		// Update the sprite
		saSpriteAction = (*siSprite)->Update();

		// Handle the SA_KILL sprite action
		if (saSpriteAction & SA_KILL)
		{
			//Notify the game that the sprite is dying
			SpriteDying(*siSprite);
			//Kill the sprite
			delete (*siSprite);
			siSprite = m_vSprites.erase(siSprite);
			continue;
		}

		// See if the sprite collided with any others
		if (CheckSpriteCollision(*siSprite)){
			// Restore the old sprite position
			(*siSprite)->SetPosition(rcOldSpritePos);
		}
		++siSprite;
	}
}

void GameEngine::CleanupSprites(){
	// Delete and remove the sprites in the sprite list
	/*
	vector<Sprite*>::iterator siSprite;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
	{
	delete (*siSprite);
	m_vSprites.erase(siSprite);
	siSprite--;
	}
	*/
	while(!m_vSprites.empty()){
		delete(m_vSprites.back());
		m_vSprites.pop_back();
	}
}

Sprite* GameEngine::IsPointInSprite(int x, int y){
	// See if the point is in a sprite in the sprite list
	list<Sprite*>::reverse_iterator siSprite;
	for (siSprite = m_vSprites.rbegin(); siSprite != m_vSprites.rend(); siSprite++)
		if (!(*siSprite)->IsHidden() && (*siSprite)->IsPointInside(x, y))
			return (*siSprite);

	// The point is not in a sprite
	return NULL;
}

void GameEngine::PlayMIDISong(LPTSTR szMIDIFileName, BOOL bRestart)
{
	// See if the MIDI player needs to be opened
	if (m_uiMIDIPlayerID == 0){
		// Open the MIDI player by specifying the device and filename
		MCI_OPEN_PARMS mciOpenParms;
		mciOpenParms.lpstrDeviceType = TEXT("sequencer");
		mciOpenParms.lpstrElementName = szMIDIFileName;
		if (mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
			(DWORD_PTR)&mciOpenParms) == 0)
			// Get the ID for the MIDI player
			m_uiMIDIPlayerID = mciOpenParms.wDeviceID;
		else
			// There was a problem, so just return
			return;
	}

	// Restart the MIDI song, if necessary
	if (bRestart)
	{
		MCI_SEEK_PARMS mciSeekParms;
		if (mciSendCommand(m_uiMIDIPlayerID, MCI_SEEK, MCI_SEEK_TO_START,
			(DWORD_PTR)&mciSeekParms) != 0)
			// There was a problem, so close the MIDI player
			CloseMIDIPlayer();
	}

	// Play the MIDI song
	MCI_PLAY_PARMS mciPlayParms;
	if (mciSendCommand(m_uiMIDIPlayerID, MCI_PLAY, 0,
		(DWORD_PTR)&mciPlayParms) != 0)
		// There was a problem, so close the MIDI player
		CloseMIDIPlayer();
}

void GameEngine::PauseMIDISong()
{
	// Pause the currently playing song, if possible
	if (m_uiMIDIPlayerID != 0)
		mciSendCommand(m_uiMIDIPlayerID, MCI_PAUSE, 0, NULL);
}

void GameEngine::CloseMIDIPlayer()
{
	// Close the MIDI player, if possible
	if (m_uiMIDIPlayerID != 0)
	{
		mciSendCommand(m_uiMIDIPlayerID, MCI_CLOSE, 0, NULL);
		m_uiMIDIPlayerID = 0;
	}
}