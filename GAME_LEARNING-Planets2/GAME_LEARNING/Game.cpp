#include "Game.h"

/*
游戏引擎调用函数
*/
BOOL GameInitialize(HINSTANCE hInstance){
	//创建游戏引擎
	g_pGame = new GameEngine(hInstance,TEXT("Crop Circles"),TEXT("Crop Circles"),IDI_ICON1,IDI_ICON2,600,400);
	if(g_pGame == NULL) 
		return FALSE;

	g_pGame->SetFrameRate(30);
	g_hInstance = hInstance;
	return TRUE;
}

void GameStart(HWND hWindow){
	srand(GetTickCount());
	//创建屏幕外设备环境和位图
	g_hOffscreenDC = CreateCompatibleDC(GetDC(hWindow));
	g_hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow),
		g_pGame->GetWidth(),g_pGame->GetHeight());
	SelectObject(g_hOffscreenDC,g_hOffscreenBitmap);

	// Create and load the bitmaps
	HDC hDC = GetDC(hWindow);
	g_pGalaxyBitmap = new Bitmap(hDC, IDB_GALAXY, g_hInstance);
	g_pPlanetBitmap[0] = new Bitmap(hDC, IDB_PLANET1, g_hInstance);
	g_pPlanetBitmap[1] = new Bitmap(hDC, IDB_PLANET2, g_hInstance);
	g_pPlanetBitmap[2] = new Bitmap(hDC, IDB_PLANET3, g_hInstance);

	// Create the planet sprites
	RECT    rcBounds = { 0, 0, 600, 400 };
	Sprite* pSprite;
	pSprite = new Sprite(g_pPlanetBitmap[0], rcBounds, BA_WRAP);
	pSprite->SetVelocity(3, 2);
	g_pGame->AddSprite(pSprite);
	pSprite = new Sprite(g_pPlanetBitmap[1], rcBounds, BA_WRAP);
	pSprite->SetVelocity(4, 1);
	g_pGame->AddSprite(pSprite);
	rcBounds.right = 200; rcBounds.bottom = 160;
	pSprite = new Sprite(g_pPlanetBitmap[2], rcBounds, BA_BOUNCE);
	pSprite->SetVelocity(-4, 2);
	g_pGame->AddSprite(pSprite);
	rcBounds.left = 400; rcBounds.top = 240;
	rcBounds.right = 600; rcBounds.bottom = 400;
	pSprite = new Sprite(g_pPlanetBitmap[2], rcBounds, BA_BOUNCE);
	pSprite->SetVelocity(7, -3);
	g_pGame->AddSprite(pSprite);

	// Set the initial drag info
	g_pDragSprite = NULL;

}

void GameEnd(){
	DeleteObject(g_hOffscreenBitmap);
	DeleteObject(g_hOffscreenDC);
	delete g_pGalaxyBitmap;
	for(int i = 0; i < 3; ++i){
		delete g_pPlanetBitmap[i];
	}
	g_pGame->CleanupSprites();
	delete g_pGame;
}

void GameActivate(HWND hWindow){

}

void GameDeactivate(HWND hWindow){

}

void GamePaint(HDC hDC){
	// Draw the background galaxy
	g_pGalaxyBitmap->Draw(hDC, 0, 0);

	// Draw the sprites
	g_pGame->DrawSprites(hDC);
}

void GameCycle(){
	g_pGame->UpdateSprites();
	//获得用于重新绘制游戏的设备环境
	HWND hWnd = g_pGame->GetWindow();
	HDC hDC = GetDC(hWnd);
	//在屏幕外设备环境上绘制游戏
	GamePaint(g_hOffscreenDC);

	//将屏幕外位图块传送到游戏屏幕
	BitBlt(hDC,0,0,g_pGame->GetWidth(),g_pGame->GetHeight(),
		g_hOffscreenDC,0,0,SRCCOPY);

	//clean up
	ReleaseDC(hWnd,hDC);
}

void HandleKeys(){
	//if(GetAsyncKeyState(VK_LEFT) < 0)
	//	g_iSpeedX = max(-g_iMAXSPEED,--g_iSpeedX);
	//else if(GetAsyncKeyState(VK_RIGHT) < 0)
	//	g_iSpeedX = min(g_iMAXSPEED,++g_iSpeedX);
	//else if(GetAsyncKeyState(VK_UP) < 0)
	//	g_iSpeedY = max(-g_iMAXSPEED,--g_iSpeedY);
	//else if(GetAsyncKeyState(VK_DOWN) < 0)
	//	g_iSpeedY = min(g_iMAXSPEED,++g_iSpeedY);
}

void MouseButtonDown(int x,int y, BOOL bLeft){
	if(bLeft && (g_pDragSprite == NULL)){
		if((g_pDragSprite=g_pGame->IsPointInSprite(x,y))!=NULL){
			SetCapture(g_pGame->GetWindow());
			MouseMove(x,y);
		}
	}
}

void MouseButtonUp(int x,int y,BOOL bLeft){
	//释放鼠标
	ReleaseCapture();
	g_pDragSprite = NULL;
}

void MouseMove(int x,int y){
	if(g_pDragSprite != NULL){
		g_pDragSprite->SetPosition(
			x-(g_pDragSprite->GetWidth()/2),
			y-(g_pDragSprite->GetHeight()/2)
			);
		InvalidateRect(g_pGame->GetWindow(),NULL,FALSE);
	}
}

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee){
	//交互子画面的速度，这样它们看起来就是彼此弹开的
	POINT ptSwapVelocity = pSpriteHitter->GetVelocity();
	pSpriteHitter->SetVelocity(pSpriteHittee->GetVelocity());
	pSpriteHittee->SetVelocity(ptSwapVelocity);
	return TRUE;
}