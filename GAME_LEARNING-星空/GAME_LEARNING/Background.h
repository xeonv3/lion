#pragma once
#include<windows.h>
#include"Bitmap.h"
/*
Background类用来创建纯色背景及图像背景
*/
class Background{
protected:
	int m_iWidth,m_iHeight;
	COLORREF m_crColor;
	Bitmap* m_pBitmap;
public:
	Background(int iWidth,int iHeight, COLORREF crColor);
	Background(Bitmap *pBitmap);
	virtual ~Background();
	//常规方法
	virtual void Update();
	virtual void Draw(HDC hDC);

	//访问方法
	int GetWidth(){return m_iWidth;}
	int GetHeight(){return m_iHeight;}
};
/*
创建一个星空的动画背景
*/
class StarryBackground : Background{
protected:
	//成员变量
	int m_iNumStars;//背景显示的星星数量
	int m_iTwinkleDelay;//星星闪烁的频率
	POINT m_ptStars[100];//存储各个星星的位置
	COLORREF m_crStarColors[100];//储存各个星星的颜色
public:
	StarryBackground(int iWidth,int iHeight,int iNumStars = 100, int iTwinkleDelay = 50);
	virtual ~StarryBackground();
	//常规方法
	virtual void Update();
	virtual void Draw(HDC hDC);
};