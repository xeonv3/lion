#include "Blizzard.h"

/*
��Ϸ���溯��
*/
BOOL GameInitialize(HINSTANCE hInstance){
	//������Ϸ����
	g_pGame = new GameEngine(hInstance,TEXT("Blizzard"),TEXT("Blizzard"),IDI_ICON1,IDI_ICON2);
	if(g_pGame == NULL) 
		return FALSE;

	g_pGame->SetFrameDelay(15);
	return TRUE;
}

void GameStart(HWND hWindow){
	//�������������������
	srand(GetTickCount());
}

void GameEnd(){
	//������Ϸ����
	delete g_pGame;
}

void GameActivate(HWND hWindow){
	HDC hDC;
	RECT rect;
	//����Ϸ��Ļ�ϻ����ı�
	GetClientRect(hWindow,&rect);
	hDC = GetDC(hWindow);
	DrawText(hDC,TEXT("Here comes the blizzard!"),-1,&rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	ReleaseDC(hWindow,hDC);
}

void GameDeactivate(HWND hWindow){
	HDC hDC;
	RECT rect;
	//����Ϸ��Ļ��ͣ���ı�
	GetClientRect(hWindow,&rect);
	hDC = GetDC(hWindow);
	DrawText(hDC,TEXT("The blizzard has passed."),-1,&rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	ReleaseDC(hWindow,hDC);
}
/*
��������е����л��ƹ�����������GameCycle()�����У���˲�����GamePaint()�����κ����飬��������������ټ�
*/
void GamePaint(HDC hDC){

}

void GameCycle(){
	HDC hDC;
	HWND hWindow = g_pGame->GetWindow();
	//����Ϸ��Ļ�ϵ����λ����ѩƬͼ��
	hDC = GetDC(hWindow);
	DrawIcon(hDC,rand() % g_pGame->GetWidth(),rand()%g_pGame->GetHeight(),
		(HICON)(WORD)GetClassLong(hWindow,GCL_HICON));
	ReleaseDC(hWindow,hDC);
}