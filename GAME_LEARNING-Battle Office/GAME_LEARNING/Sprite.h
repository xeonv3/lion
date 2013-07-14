#pragma once

#include<windows.h>
#include"Bitmap.h"
/*
Sprite��ʵ����Ϸ�ӻ�������
*/
//�ӻ�����߽���εı߽�����ʱ����
typedef WORD BOUNDSACTION;
const BOUNDSACTION BA_STOP = 0,
	BA_WRAP = 1,
	BA_BOUNCE = 2,
	BA_DIE = 3;

//�ӻ�����ײʱ�Ķ���
typedef WORD SPRITEACTION;
const SPRITEACTION  SA_NONE   = 0x0000L,//�����κ�����
	SA_KILL   = 0x0001L;//ɾ��һ���ӻ��沢�ƻ���
//������Ҫ����µĶ���
class Sprite{
protected:
	Bitmap* m_pBitmap;
	int m_iNumFrames, m_iCurFrame;
	int m_iFrameDelay, m_iFrameTrigger;
	RECT m_rcPosition;
	POINT m_ptVelocity;
	int m_iZOrder;
	RECT m_rcBounds;
	BOUNDSACTION m_baBoundsAction;
	BOOL m_bHidden;
	RECT m_rcCollision;//��ײ���Σ�����ȷ��һ���ӻ����Ƿ�����һ���ӻ�����ײ

	//Helper Methods
	void UpdateFrame();
	virtual void  CalcCollisionRect();
public:
	Sprite(Bitmap* pBitmap);
	Sprite(Bitmap* pBitmap,RECT &rcBounds,BOUNDSACTION baBoundsAction = BA_STOP);
	Sprite(Bitmap* pBitmap,POINT ptPosition,POINT ptVelocity, int iZOrder,
		RECT &rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);
	virtual ~Sprite();
	// General Methods
	virtual SPRITEACTION Update();//���ӻ����ٶ�Ӧ����λ�ò����ӻ����ƶ������ʵ��ķ�Ӧ
	void Draw(HDC hDC);//�ڵ�ǰλ�û����ӻ���
	BOOL IsPointInside(int x,int y);//�����������ȷ������Ƿ񵥻����ӻ��棬���Ϊ���в���
	BOOL TestCollision(Sprite* pTestSprite);//������������ӻ������ײ�����Ƿ����κβ��ִ����ص�
	// Accessor Methods
	Bitmap* GetBitmap() { return m_pBitmap; };
	void SetNumFrames(int iNumFrames);
	void SetFrameDelay(int iFrameDelay) { m_iFrameDelay = iFrameDelay; };
	RECT& GetPosition(){return m_rcPosition;}
	void SetPosition(int x,int y);
	void SetPosition(POINT ptPosition);
	void SetPosition(RECT& rcPosition){ CopyRect(&m_rcPosition,&rcPosition);}
	void OffsetPosition(int x,int y);
	POINT GetVelocity(){return m_ptVelocity;}
	void SetVelocity(int x,int y);
	void SetVelocity(POINT ptVelocity);
	int GetZOrder(){return m_iZOrder;}
	void SetZOrder(int iZOrder){m_iZOrder = iZOrder;}
	void SetBounds(RECT &rcBounds){CopyRect(&m_rcBounds,&rcBounds);}
	void SetBoundsAction(BOUNDSACTION ba){m_baBoundsAction = ba;}
	BOOL IsHidden(){return m_bHidden;}
	void SetHidden(BOOL bHidden){m_bHidden = bHidden;}
	int GetWidth(){return m_pBitmap->GetWidth();}
	int GetHeight(){return m_pBitmap->GetHeight()/m_iNumFrames;}
	RECT& GetCollision(){return m_rcCollision;}
};

//-----------------------------------------------------------------
// Sprite Inline Helper Methods
//-----------------------------------------------------------------
inline void Sprite::CalcCollisionRect(){
	//�ӻ�����x��y����������ֵ���ȼ���Ϊ�ӻ����С��1/12��
	//���Ľ��������ײ���α�λ�þ���С1/6
	int iXShrink = (m_rcPosition.left - m_rcPosition.right) / 12;
	int iYShrink = (m_rcPosition.top - m_rcPosition.bottom) / 12;
	CopyRect(&m_rcCollision, &m_rcPosition);
	InflateRect(&m_rcCollision, iXShrink, iYShrink);
}

inline void Sprite::UpdateFrame(){
  if ((m_iFrameDelay >= 0) && (--m_iFrameTrigger <= 0)){
    // Reset the frame trigger;
    m_iFrameTrigger = m_iFrameDelay;

    // Increment the frame
    if (++m_iCurFrame >= m_iNumFrames)
        m_iCurFrame = 0;
  }
}

//-----------------------------------------------------------------
// Sprite Inline General Methods
//-----------------------------------------------------------------
inline BOOL Sprite::IsPointInside(int x, int y){
	POINT ptPoint;
	ptPoint.x = x;
	ptPoint.y = y;
	return PtInRect(&m_rcPosition, ptPoint);
}
inline BOOL Sprite::TestCollision(Sprite* pTestSprite){
	RECT& rcTest = pTestSprite->GetCollision();
	return m_rcCollision.left <= rcTest.right &&
		rcTest.left <= m_rcCollision.right &&
		m_rcCollision.top <= rcTest.bottom &&
		rcTest.top <= m_rcCollision.bottom;
}
//-----------------------------------------------------------------
// Sprite Inline Accessor Methods
//-----------------------------------------------------------------
inline void Sprite::SetNumFrames(int iNumFrames)
{
  // Set the number of frames
  m_iNumFrames = iNumFrames;

  // Recalculate the position
  RECT rect = GetPosition();
  rect.bottom = rect.top + ((rect.bottom - rect.top) / iNumFrames);
  SetPosition(rect);
}

inline void Sprite::SetPosition(int x, int y){
	OffsetRect(&m_rcPosition, x - m_rcPosition.left, y - m_rcPosition.top);
	CalcCollisionRect();
}

inline void Sprite::SetPosition(POINT ptPosition){
	OffsetRect(&m_rcPosition, ptPosition.x - m_rcPosition.left,
		ptPosition.y - m_rcPosition.top);
	CalcCollisionRect();
}

inline void Sprite::OffsetPosition(int x, int y){
	OffsetRect(&m_rcPosition, x, y);
	CalcCollisionRect();
}

inline void Sprite::SetVelocity(int x, int y){
	m_ptVelocity.x = x;
	m_ptVelocity.y = y;
}

inline void Sprite::SetVelocity(POINT ptVelocity){
	m_ptVelocity.x = ptVelocity.x;
	m_ptVelocity.y = ptVelocity.y;
}
