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
	//创建并加载位图
	HDC hDC = GetDC(hWindow);
	g_pGalaxyBitmap = new Bitmap(hDC,IDB_GALAXY,g_hInstance);
	g_pPlanetBitmap[0] = new Bitmap(hDC,IDB_PLANET1,g_hInstance);
	g_pPlanetBitmap[1] = new Bitmap(hDC,IDB_PLANET2,g_hInstance);
	g_pPlanetBitmap[2] = new Bitmap(hDC,IDB_PLANET3,g_hInstance);

	//创建行星子画面
	RECT rcBounds = {0,0,600,400};
	g_pPlanetSprite[0] = new Sprite(g_pPlanetBitmap[0],rcBounds);
	g_pPlanetSprite[1] = new Sprite(g_pPlanetBitmap[1],rcBounds,BA_WRAP);
	g_pPlanetSprite[2] = new Sprite(g_pPlanetBitmap[2],rcBounds,BA_BOUNCE);
	g_pPlanetSprite[0]->SetPosition(0,0);
	g_pPlanetSprite[0]->SetVelocity(1,1);
	g_pPlanetSprite[1]->SetVelocity(2,-1);
	g_pPlanetSprite[2]->SetVelocity(3,-2);

	g_bDragging = FALSE;
	g_iDragPlanet = -1;
}

void GameEnd(){
	delete g_pGalaxyBitmap;
	for (int i = 0; i < 3; i++)
		delete g_pPlanetBitmap[i];

	// Cleanup the sprites
	for (int i = 0; i < 3; i++)
		delete g_pPlanetSprite[i];
	delete g_pGame;
}

void GameActivate(HWND hWindow){

}

void GameDeactivate(HWND hWindow){

}

void GamePaint(HDC hDC){
	g_pGalaxyBitmap->Draw(hDC,0,0);
	for(int i = 0; i < 3; ++i){
		g_pPlanetSprite[i]->Draw(hDC);
	}
}

void GameCycle(){
	for(int i = 0; i < 3; ++i){
		g_pPlanetSprite[i]->Update();
	}
	InvalidateRect(g_pGame->GetWindow(),NULL,FALSE);
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
	if(bLeft && !g_bDragging){
		for(int i = 0; i < 3; ++i){
			if(g_pPlanetSprite[i]->IsPointInside(x,y)){
				//捕获鼠标
				SetCapture(g_pGame->GetWindow());
				g_bDragging = TRUE;
				g_iDragPlanet = i;

				MouseMove(x,y);
				break;
			}
		}
	}
}

void MouseButtonUp(int x,int y,BOOL bLeft){
	//释放鼠标
	ReleaseCapture();
	g_bDragging = FALSE;
}

void MouseMove(int x,int y){
	if(g_bDragging){
		g_pPlanetSprite[g_iDragPlanet]->SetPosition(
			x-(g_pPlanetBitmap[g_iDragPlanet]->GetWidth()/2),
			y-(g_pPlanetBitmap[g_iDragPlanet]->GetHeight()/2)
			);
		InvalidateRect(g_pGame->GetWindow(),NULL,FALSE);
	}
}

