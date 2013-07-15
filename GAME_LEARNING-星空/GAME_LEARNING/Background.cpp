#include "Background.h"

Background::Background(int iWidth,int iHeight,COLORREF crColor){
	//初始化成员变量
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_crColor = crColor;
	m_pBitmap = NULL;//纯色背景不需要位图成员变量
}

Background::Background(Bitmap* pBitmap){
	//初始化成员变量
	m_crColor = 0;
	m_pBitmap = pBitmap;
	m_iWidth = pBitmap->GetWidth();
	m_iHeight = pBitmap->GetHeight();
}
/*
因为基本背景没有使用动画，所以不做任何事情
主为是作为以后派生的背景类的占位程序
*/
void Background::Update(){
}

void Background::Draw(HDC hDC){
	//绘制背景
	if(m_pBitmap != NULL)
		m_pBitmap->Draw(hDC,0,0);
	else{
		RECT rect = {0,0,m_iWidth,m_iHeight};
		HBRUSH hBrush = CreateSolidBrush(m_crColor);
		FillRect(hDC,&rect,hBrush);
		DeleteObject(hBrush);
	}
}
Background::~Background()
{
}

StarryBackground::StarryBackground(int iWidth,int iHeight,int iNumStars,
								   int iTwinkleDelay):Background(iWidth,iHeight,0){
	m_iNumStars = min(iNumStars,100);
	m_iTwinkleDelay = iTwinkleDelay;

	for(int i = 0; i < iNumStars; ++i){
		m_ptStars[i].x = rand() % iWidth;
		m_ptStars[i].y = rand() % iHeight;
		m_crStarColors[i] = RGB(128,128,128);
	}
}

StarryBackground::~StarryBackground(){
}

void StarryBackground::Update(){
	int iRGB;
	for(int i = 0; i < m_iNumStars; ++i){
		if((rand()%m_iTwinkleDelay)==0){
			iRGB = rand() % 256;
			m_crStarColors[i] = RGB(iRGB,iRGB,iRGB);
		}
	}
}
	
void StarryBackground::Draw(HDC hDC){
	//绘制纯黑色背景
	RECT rect  = {0,0,m_iWidth,m_iHeight};
	HBRUSH hBrush = CreateSolidBrush(RGB(0,0,0));
	FillRect(hDC,&rect,hBrush);
	DeleteObject(hBrush);

	//Draw the stars
	for(int i = 0; i < m_iNumStars; ++i){
		SetPixel(hDC,m_ptStars[i].x,m_ptStars[i].y,m_crStarColors[i]);
	}
}