#include "Blizzard.h"

/*
游戏引擎函数
*/
BOOL GameInitialize(HINSTANCE hInstance){
	//创建游戏引擎
	g_pGame = new GameEngine(hInstance,TEXT("Blizzard"),TEXT("Blizzard"),IDI_ICON1,IDI_ICON2);
	if(g_pGame == NULL) 
		return FALSE;

	g_pGame->SetFrameDelay(15);
	return TRUE;
}

void GameStart(HWND hWindow){
	//生成随机数生成器种子
	srand(GetTickCount());
}

void GameEnd(){
	//清理游戏引擎
	delete g_pGame;
}

void GameActivate(HWND hWindow){
	HDC hDC;
	RECT rect;
	//在游戏屏幕上绘制文本
	GetClientRect(hWindow,&rect);
	hDC = GetDC(hWindow);
	DrawText(hDC,TEXT("Here comes the blizzard!"),-1,&rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	ReleaseDC(hWindow,hDC);
}

void GameDeactivate(HWND hWindow){
	HDC hDC;
	RECT rect;
	//在游戏屏幕上停用文本
	GetClientRect(hWindow,&rect);
	hDC = GetDC(hWindow);
	DrawText(hDC,TEXT("The blizzard has passed."),-1,&rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	ReleaseDC(hWindow,hDC);
}
/*
这个例子中的所有绘制工作都发生在GameCycle()函数中，因此不必在GamePaint()中做任何事情，不过这种情况很少见
*/
void GamePaint(HDC hDC){

}

void GameCycle(){
	HDC hDC;
	HWND hWindow = g_pGame->GetWindow();
	//在游戏屏幕上的随机位绘制雪片图标
	hDC = GetDC(hWindow);
	DrawIcon(hDC,rand() % g_pGame->GetWidth(),rand()%g_pGame->GetHeight(),
		(HICON)(WORD)GetClassLong(hWindow,GCL_HICON));
	ReleaseDC(hWindow,hDC);
}