#pragma once
#include<windows.h>
#include"Bitmap.h"
/*
Background������������ɫ������ͼ�񱳾�
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
	//���淽��
	virtual void Update();
	virtual void Draw(HDC hDC);

	//���ʷ���
	int GetWidth(){return m_iWidth;}
	int GetHeight(){return m_iHeight;}
};
/*
����һ���ǿյĶ�������
*/
class StarryBackground : Background{
protected:
	//��Ա����
	int m_iNumStars;//������ʾ����������
	int m_iTwinkleDelay;//������˸��Ƶ��
	POINT m_ptStars[100];//�洢�������ǵ�λ��
	COLORREF m_crStarColors[100];//����������ǵ���ɫ
public:
	StarryBackground(int iWidth,int iHeight,int iNumStars = 100, int iTwinkleDelay = 50);
	virtual ~StarryBackground();
	//���淽��
	virtual void Update();
	virtual void Draw(HDC hDC);
};