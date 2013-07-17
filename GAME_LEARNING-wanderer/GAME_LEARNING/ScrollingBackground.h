#pragma once

#include<Windows.h>
#include"Background.h"

//-----------------------------------------------------------------
// Custom Data Types
//-----------------------------------------------------------------
typedef WORD SCROLLDIR;
const SCROLLDIR SD_UP = 0,
	SD_RIGHT = 1,
	SD_DOWN = 2,
	SD_LEFT = 3;

//-----------------------------------------------------------------
// Background Layer Class
//-----------------------------------------------------------------
class BackgroundLayer:Bitmap{
protected:
	//成员变量
	RECT m_rcViewport;
	int m_iSpeed;
	SCROLLDIR m_sdDirection;

public:
	//构造函数和析构函数
	BackgroundLayer(HDC hDC,LPTSTR szFileName,int iSpeed,SCROLLDIR sdDirection);
	BackgroundLayer(HDC hDC, UINT uiResID,HINSTANCE hInstance,int iSpeed = 0,
		SCROLLDIR sdDirection = SD_LEFT);

	//常规方法
	virtual void Update();
	virtual void Draw(HDC hDC,int x,int y,BOOL bTrans = FALSE,
		COLORREF crTransColor = RGB(255,0,255));
	void SetSpeed(int iSpeed){m_iSpeed = iSpeed;}
	void SetDirection(SCROLLDIR sdDirection){m_sdDirection = sdDirection;}
	void SetViewport(RECT &rcViewport){
		CopyRect(&m_rcViewport,&rcViewport);
	}
};

//-----------------------------------------------------------------
// Scrolling Background Class
//-----------------------------------------------------------------
class ScrollingBackground:Background{
protected:
	int m_iNumLayers;
	BackgroundLayer* m_pLayers[10]; //这里的最大图层数是10，但是确实需要更多图层时可以增加这个数字

public:
	ScrollingBackground(int iWidth,int iHeight);
	virtual ~ScrollingBackground();

	virtual void Update();
	virtual void Draw(HDC hDC,BOOL bTrans = FALSE,COLORREF crTransColor = RGB(255,0,255));
	void AddLayer(BackgroundLayer* pLayer);
};