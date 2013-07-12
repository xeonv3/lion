#include "Game.h"

/*
游戏引擎函数
*/
BOOL GameInitialize(HINSTANCE hInstance){
	//创建游戏引擎
	g_pGame = new GameEngine(hInstance,TEXT("Crop Circles"),TEXT("Crop Circles"),IDI_ICON1,IDI_ICON2,500,400);
	if(g_pGame == NULL) 
		return FALSE;

	g_pGame->SetFrameDelay(30);
	g_hInstance = hInstance;
	return TRUE;
}

void GameStart(HWND hWindow){
	//创建并加载背景和飞碟位图
	HDC hDC = GetDC(hWindow);
	g_pBackground = new Bitmap(hDC,IDB_BACKGROUND,g_hInstance);
	g_pSaucer = new Bitmap(hDC,IDB_SAUCER,g_hInstance);

	g_iSaucerX = 250 - (g_pSaucer->GetWidth()/ 2);
	g_iSaucerY = 200 - (g_pSaucer->GetHeight()/2);
	g_iSpeedX = 0;
	g_iSpeedY = 0;
}

void GameEnd(){
	//清理游戏引擎
	delete g_pBackground;
	delete g_pSaucer;

	delete g_pGame;
}

void GameActivate(HWND hWindow){

}

void GameDeactivate(HWND hWindow){

}

void GamePaint(HDC hDC){
	g_pBackground->Draw(hDC,0,0);
	g_pSaucer->Draw(hDC,g_iSaucerX,g_iSaucerY,TRUE);
}

void GameCycle(){
	g_iSaucerX = min(500-g_pSaucer->GetWidth(),max(0,g_iSaucerX+g_iSpeedX));
	g_iSaucerY = min(320,max(0,g_iSaucerY+g_iSpeedY));
	InvalidateRect(g_pGame->GetWindow(),NULL,FALSE);
}

void HandleKeys(){
	if(GetAsyncKeyState(VK_LEFT) < 0)
		g_iSpeedX = max(-g_iMAXSPEED,--g_iSpeedX);
	else if(GetAsyncKeyState(VK_RIGHT) < 0)
		g_iSpeedX = min(g_iMAXSPEED,++g_iSpeedX);
	else if(GetAsyncKeyState(VK_UP) < 0)
		g_iSpeedY = max(-g_iMAXSPEED,--g_iSpeedY);
	else if(GetAsyncKeyState(VK_DOWN) < 0)
		g_iSpeedY = min(g_iMAXSPEED,++g_iSpeedY);
}

void MouseButtonDown(int x,int y, BOOL bLeft){
	if(bLeft){
		g_iSaucerX = x - (g_pSaucer->GetWidth()/2);
		g_iSaucerY = y - (g_pSaucer->GetHeight()/2);
	}else{
		g_iSpeedX = 0;
		g_iSpeedY = 0;
	}
}

void MouseButtonUp(int x,int y,BOOL bLeft){
	
}
void MouseMove(int x,int y){
}