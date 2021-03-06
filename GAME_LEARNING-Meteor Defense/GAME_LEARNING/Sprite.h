#pragma once

#include<windows.h>
#include"Bitmap.h"
/*
Sprite类实现游戏子画面的设计
*/
//子画面与边界矩形的边界相遇时动作
typedef WORD BOUNDSACTION;
const BOUNDSACTION BA_STOP = 0,
	BA_WRAP = 1,
	BA_BOUNCE = 2,
	BA_DIE = 3;

//子画面碰撞时的动作
typedef WORD SPRITEACTION;
const SPRITEACTION  SA_NONE   = 0x0000L,//不做任何事情
	SA_KILL   = 0x0001L;//删除一个子画面并破坏它
//根据需要添加新的动作
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
	RECT m_rcCollision;//碰撞矩形，用来确定一个子画面是否与另一个子画面相撞
	//以下两个变量，用作一种子画面：在一个周期后，自动消失，如爆炸子画面
	BOOL m_bDying;//标记为将要删除
	BOOL m_bOneCycle;//指出是否应该在显示了所有帧后删除子画面
	//Helper Methods
	void UpdateFrame();
	virtual void  CalcCollisionRect();
public:
	//consturctor(s)/destructor
	Sprite(Bitmap* pBitmap);
	Sprite(Bitmap* pBitmap,RECT &rcBounds,BOUNDSACTION baBoundsAction = BA_STOP);
	Sprite(Bitmap* pBitmap,POINT ptPosition,POINT ptVelocity, int iZOrder,
		RECT &rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);
	virtual ~Sprite();
	// General Methods
	virtual SPRITEACTION Update();//将子画面速度应用于位置并对子画面移动做出适当的反应
	void Draw(HDC hDC);//在当前位置绘制子画面
	BOOL IsPointInside(int x,int y);//这个方法便于确定鼠标是否单击了子画面，这称为击中测试
	BOOL TestCollision(Sprite* pTestSprite);//用来检查两个子画面的碰撞矩形是否有任何部分存在重叠
	void Kill(){m_bDying = TRUE;}
	// Accessor Methods
	Bitmap* GetBitmap() { return m_pBitmap; };
	void SetNumFrames(int iNumFrames,BOOL bOneCycle = FALSE);
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
	//子画面在x和y方向收缩的值首先计算为子画面大小的1/12。
	//最后的结果就是碰撞矩形比位置矩形小1/6
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
		if (++m_iCurFrame >= m_iNumFrames){
			if(m_bOneCycle){
				m_bDying = TRUE;
			}else{
				m_iCurFrame = 0;
			}
		}
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
inline void Sprite::SetNumFrames(int iNumFrames,BOOL bOneCycle)
{
	// Set the number of frames
	m_iNumFrames = iNumFrames;
	m_bOneCycle = bOneCycle;

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
