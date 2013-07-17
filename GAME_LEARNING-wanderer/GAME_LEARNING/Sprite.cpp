#include"Sprite.h"

Sprite::Sprite(Bitmap* pBitmap){
	//初始化成员变量
	m_pBitmap = pBitmap;
	m_iNumFrames = 1;
	m_iCurFrame = m_iFrameDelay = m_iFrameTrigger = 0;
	SetRect(&m_rcPosition,0,0,pBitmap->GetWidth(),pBitmap->GetHeight());
	CalcCollisionRect();
	m_ptVelocity.x = m_ptVelocity.y = 0;
	m_iZOrder = 0;
	SetRect(&m_rcBounds,0,0,640,480);//默认的子画面边界，它对应于默认的游戏屏幕大小
	m_baBoundsAction = BA_STOP;
	m_bHidden = FALSE;
	m_bDying = FALSE;
	m_bOneCycle = FALSE;
}

Sprite::Sprite(Bitmap* pBitmap,RECT& rcBounds, BOUNDSACTION baBoundsAction){
	//计算一个随机位置
	int iXPos = rand() % (rcBounds.right - rcBounds.left);
	int iYPos = rand() % (rcBounds.bottom - rcBounds.top);

	//初始化成员变量
	m_pBitmap = pBitmap;
	m_iNumFrames = 1;
	m_iCurFrame = m_iFrameDelay = m_iFrameTrigger = 0;
	SetRect(&m_rcPosition,iXPos,iYPos,iXPos + pBitmap->GetWidth(),iYPos + pBitmap->GetHeight());
	CalcCollisionRect();
	m_ptVelocity.x = m_ptVelocity.y = 0;
	m_iZOrder = 0;
	CopyRect(&m_rcBounds,&rcBounds);
	m_baBoundsAction = baBoundsAction;
	m_bHidden = FALSE;
	m_bDying = FALSE;
	m_bOneCycle = FALSE;
}

Sprite::Sprite(Bitmap *pBitmap, POINT ptPosition,POINT ptVelocity,int iZOrder, RECT& rcBounds,BOUNDSACTION baBoundsAction){
	m_pBitmap = pBitmap;
	m_iNumFrames = 1;
	m_iCurFrame = m_iFrameDelay = m_iFrameTrigger = 0;
	SetRect(&m_rcPosition,ptPosition.x,ptPosition.y,ptPosition.x + pBitmap->GetWidth(),ptPosition.y + pBitmap->GetHeight());
	CalcCollisionRect();
	m_ptVelocity = ptVelocity;
	m_iZOrder =iZOrder;
	CopyRect(&m_rcBounds,&rcBounds);
	m_baBoundsAction = baBoundsAction;
	m_bHidden = FALSE;
	m_bDying = FALSE;
	m_bOneCycle = FALSE;
}

Sprite::~Sprite(){
}

SPRITEACTION Sprite::Update(){
	//查看是否需要删除子画面
	if(m_bDying){
		return SA_KILL;
	}
	//更新帧
	UpdateFrame();
	//更新位置
	POINT ptNewPosition,ptSpriteSize,ptBoundsSize;
	ptNewPosition.x = m_rcPosition.left + m_ptVelocity.x;
	ptNewPosition.y = m_rcPosition.top + m_ptVelocity.y;
	ptSpriteSize.x = m_rcPosition.right - m_rcPosition.left;
	ptSpriteSize.y = m_rcPosition.bottom  - m_rcPosition.top;
	ptBoundsSize.x  = m_rcBounds.right - m_rcBounds.left;
	ptBoundsSize.y = m_rcBounds.bottom - m_rcBounds.top;

	//检查边界动作
	//环绕
	if(m_baBoundsAction == BA_WRAP){
		if((ptNewPosition.x + ptSpriteSize.x) < m_rcBounds.left){
			ptNewPosition.x = m_rcBounds.right;
		}
		else if(ptNewPosition.x > m_rcBounds.right){
			ptNewPosition.x = m_rcBounds.left - ptSpriteSize.x;
		}
		if((ptNewPosition.y + ptSpriteSize.y) < m_rcBounds.top){
			ptNewPosition.y = m_rcBounds.bottom;
		}
		else if(ptNewPosition.y > m_rcBounds.bottom){
			ptNewPosition.y = m_rcBounds.top - ptSpriteSize.y;
		}
	}	
	//弹开
	else if(m_baBoundsAction == BA_BOUNCE){
		BOOL bBounce = FALSE;
		POINT ptNewVelocity = m_ptVelocity;
		if(ptNewPosition.x < m_rcBounds.left){
			bBounce = TRUE;
			ptNewPosition.x = m_rcBounds.left;
			ptNewVelocity.x = -ptNewVelocity.x;
		}
		else if((ptNewPosition.x + ptSpriteSize.x) > m_rcBounds.right){
			bBounce = TRUE;
			ptNewPosition.x = m_rcBounds.right - ptSpriteSize.x;
			ptNewVelocity.x = -ptNewVelocity.x;
		}
		if(ptNewPosition.y < m_rcBounds.top){
			bBounce = TRUE;
			ptNewPosition.y = m_rcBounds.top;
			ptNewVelocity.y = -ptNewVelocity.y;
		}
		else if((ptNewPosition.y + ptSpriteSize.y) > m_rcBounds.bottom){
			bBounce = TRUE;
			ptNewPosition.y = m_rcBounds.bottom - ptSpriteSize.y;
			ptNewVelocity.y = -ptNewVelocity.y;
		}
		if(bBounce)
			SetVelocity(ptNewVelocity);
	}
	else if(m_baBoundsAction == BA_DIE){
		if((ptNewPosition.x + ptSpriteSize.x) < m_rcBounds.left ||
			ptNewPosition.x > m_rcBounds.right || 
			(ptNewPosition.y + ptSpriteSize.y) < m_rcBounds.top ||
			ptNewPosition.y > m_rcBounds.bottom)
			return SA_KILL;
	}
	//停止（默认）
	else
	{
		if(ptNewPosition.x  < m_rcBounds.left || ptNewPosition.x > (m_rcBounds.right-ptSpriteSize.x)){
			ptNewPosition.x = max(m_rcBounds.left,min(ptNewPosition.x,m_rcBounds.right-ptSpriteSize.x));
			SetVelocity(0,0);
		}
		if(ptNewPosition.y < m_rcBounds.top || ptNewPosition.y > (m_rcBounds.bottom - ptSpriteSize.y)){
			ptNewPosition.y = max(m_rcBounds.top,min(ptNewPosition.y,m_rcBounds.bottom-ptSpriteSize.y));
			SetVelocity(0,0);
		}
	}
	SetPosition(ptNewPosition);
	return SA_NONE;
}

Sprite* Sprite::AddSprite()
{
  return NULL;
}

void Sprite::Draw(HDC hDC){
	//如果没有隐藏，则绘制子画面
	if(m_pBitmap != NULL && !m_bHidden){
		if(m_iNumFrames == 1)
			m_pBitmap->Draw(hDC,m_rcPosition.left,m_rcPosition.top,TRUE);
		else
			m_pBitmap->DrawPart(hDC,m_rcPosition.left,m_rcPosition.top,0,
			m_iCurFrame * GetHeight(),GetWidth(),GetHeight(),TRUE);
	}
}