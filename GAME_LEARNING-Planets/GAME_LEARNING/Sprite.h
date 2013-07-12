#include<windows.h>
#include"Bitmap.h"
/*
Sprite类实现游戏子画面的设计
*/
typedef WORD BOUNDSACTION;
const BOUNDSACTION BA_STOP = 0,
	BA_WRAP = 1,BA_BOUNCE = 2,BA_DIE = 3;
class Sprite{
protected:
	Bitmap* m_pBitmap;
	RECT m_rcPosition;
	POINT m_ptVelocity;
	int m_iZOrder;
	RECT m_rcBounds;
	BOUNDSACTION m_baBoundsAction;
	BOOL m_bHidden;
public:
	Sprite(Bitmap* pBitmap);
	Sprite(Bitmap* pBitmap,RECT &rcBounds,BOUNDSACTION baBoundsAction = BA_STOP);
	Sprite(Bitmap* pBitmap,POINT ptPosition,POINT ptVelocity, int iZOrder,
		RECT &rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);
	virtual ~Sprite();

	virtual void Update();//将子画面速度应用于位置并对子画面移动做出适当的反应
	void Draw(HDC hDC);//在当前位置绘制子画面
	BOOL IsPointInside(int x,int y);//这个方法便于确定鼠标是否单击了子画面，这称为击中测试

	RECT& GetPosition(){return m_rcPosition;}
	void SetPosition(int x,int y);
	void SetPosition(POINT ptPosition);
	void SetPosition(RECT& rcPosition){ CopyRect(&m_rcPosition,&rcPosition);}
	void OffsetPosition(int x,int y);
	POINT GetVelocity(){return m_ptVelocity;}
	void SetVelocity(int x,int y);
	void SetVelocity(POINT ptVelocity);
	int getZOrder(){return m_iZOrder;}
	void SetZOrder(int iZOrder){m_iZOrder = iZOrder;}
	void SetBounds(RECT &rcBounds){CopyRect(&m_rcBounds,&rcBounds);}
	void SetBoundsAction(BOUNDSACTION ba){m_baBoundsAction = ba;}
	BOOL isHidden(){return m_bHidden;}
	void SetHidden(BOOL bHidden){m_bHidden = bHidden;}
	int GetWidth(){return m_pBitmap->GetWidth();}
	int GetHeight(){return m_pBitmap->GetHeight();}
};


//-----------------------------------------------------------------
// Sprite Inline General Methods
//-----------------------------------------------------------------
inline BOOL Sprite::IsPointInside(int x, int y)
{
  POINT ptPoint;
  ptPoint.x = x;
  ptPoint.y = y;
  return PtInRect(&m_rcPosition, ptPoint);
}

//-----------------------------------------------------------------
// Sprite Inline Accessor Methods
//-----------------------------------------------------------------
inline void Sprite::SetPosition(int x, int y)
{
  OffsetRect(&m_rcPosition, x - m_rcPosition.left, y - m_rcPosition.top);
}

inline void Sprite::SetPosition(POINT ptPosition)
{
  OffsetRect(&m_rcPosition, ptPosition.x - m_rcPosition.left,
    ptPosition.y - m_rcPosition.top);
}

inline void Sprite::OffsetPosition(int x, int y)
{
  OffsetRect(&m_rcPosition, x, y);
}

inline void Sprite::SetVelocity(int x, int y)
{
  m_ptVelocity.x = x;
  m_ptVelocity.y = y;
}

inline void Sprite::SetVelocity(POINT ptVelocity)
{
  m_ptVelocity.x = ptVelocity.x;
  m_ptVelocity.y = ptVelocity.y;
}
