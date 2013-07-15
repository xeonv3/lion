#pragma once

#include<windows.h>
#include<list>
#include"Sprite.h"
using namespace std;

int WINAPI WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow);

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/*
���º������ض�ʵ������Ϸ���еģ�������ʹ�ø���Ϸ����ĸ�����Ϸ�ṩ
*/
BOOL GameInitialize(HINSTANCE hInstance);
void GameStart(HWND hWindow);
void GameEnd();
void GameActivate(HWND hWindow);
void GameDeactivate(HWND hWindow);
void GamePaint(HDC hDC);
void GameCycle();
void HandleKeys();
void MouseButtonDown(int x,int y,BOOL bLeft);
void MouseButtonUp(int x,int y,BOOL bLeft);
void MouseMove(int x,int y);
BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee);
/*
GameEngine��
*/
class GameEngine{
protected:
	static GameEngine *m_pGameEngine;
	HINSTANCE m_hInstance;
	HWND m_hWindow;
	TCHAR m_szWindowClass[32];
	TCHAR m_szTitle[32];
	WORD m_wIcon,m_wSmallIcon;
	int m_iWidth,m_iHeight;
	int m_iFrameDelay;
	BOOL m_bSleep;
	list<Sprite*> m_vSprites;
	UINT m_uiMIDIPlayerID;
	// Helper Methods
	BOOL CheckSpriteCollision(Sprite* pTestSprite);
public:
	//���캯������������
	GameEngine(HINSTANCE hInstance, LPTSTR szWindowClass, LPTSTR szTitle,
		WORD wIcon, WORD wSmallIcon, int iWidth = 640, int iHeight = 480);
	virtual ~GameEngine();
	//���淽��
	static GameEngine* GetEngine(){return m_pGameEngine;}
	BOOL Initialize(int iCmdShow);
	LRESULT HandleEvent(HWND hWindow,UINT msg, WPARAM wParam, LPARAM lParam);
	void AddSprite(Sprite* pSprite);
	void DrawSprites(HDC hDC);
	void UpdateSprites();
	void CleanupSprites();
	Sprite* IsPointInSprite(int x, int y);
	void PlayMIDISong(LPTSTR szMIDIFileName = TEXT(""),BOOL bRestart = TRUE);
	void PauseMIDISong();
	void CloseMIDIPlayer();
	//���ʷ���
	HINSTANCE GetInstace(){return m_hInstance;}
	HWND GetWindow(){return m_hWindow;}
	void SetWindow(HWND hWindow){m_hWindow = hWindow;}
	LPTSTR GetTitle(){return m_szTitle;}
	WORD GetIcon(){return m_wIcon;}
	WORD GetSmallIcon(){return m_wSmallIcon;}
	int GetWidth(){return m_iWidth;}
	int GetHeight(){return m_iHeight;}
	int GetFrameDelay(){return m_iFrameDelay;}
	void SetFrameRate(int iFrameRate){m_iFrameDelay = 1000/iFrameRate;}
	BOOL GetSleep(){return m_bSleep;}
	void SetSleep(BOOL bSleep){m_bSleep = bSleep;}
};