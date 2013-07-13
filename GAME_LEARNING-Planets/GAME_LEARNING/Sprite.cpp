#include"Sprite.h"

Sprite::Sprite(Bitmap* pBitmap){
	//��ʼ����Ա����
	m_pBitmap = pBitmap;
	SetRect(&m_rcPosition,0,0,pBitmap->GetWidth(),pBitmap->GetHeight());
	m_ptVelocity.x = m_ptVelocity.y = 0;
	m_iZOrder = 0;
	SetRect(&m_rcBounds,0,0,640,480);//Ĭ�ϵ��ӻ���߽磬����Ӧ��Ĭ�ϵ���Ϸ��Ļ��С
	m_baBoundsAction = BA_STOP;
	m_bHidden = FALSE;
}

Sprite::Sprite(Bitmap* pBitmap,RECT& rcBounds, BOUNDSACTION baBoundsAction){
	//����һ�����λ��
	int iXPos = rand() % (rcBounds.right - rcBounds.left);
	int iYPos = rand() % (rcBounds.bottom - rcBounds.top);

	//��ʼ����Ա����
	m_pBitmap = pBitmap;
	SetRect(&m_rcPosition,iXPos,iYPos,iXPos + pBitmap->GetWidth(),iYPos + pBitmap->GetHeight());
	m_ptVelocity.x = m_ptVelocity.y = 0;
	m_iZOrder = 0;
	CopyRect(&m_rcBounds,&rcBounds);
	m_baBoundsAction = baBoundsAction;
	m_bHidden = FALSE;
}

Sprite::Sprite(Bitmap *pBitmap, POINT ptPosition,POINT ptVelocity,int iZOrder,
			   RECT& rcBounds,BOUNDSACTION baBoundsAction){
				   m_pBitmap = pBitmap;
				   SetRect(&m_rcPosition,ptPosition.x,ptPosition.y,ptPosition.x + pBitmap->GetWidth(),ptPosition.y + pBitmap->GetHeight());
				   m_ptVelocity = ptVelocity;
				   m_iZOrder =iZOrder;
				   CopyRect(&m_rcBounds,&rcBounds);
				   m_baBoundsAction = baBoundsAction;
				   m_bHidden = FALSE;
}

Sprite::~Sprite(){
}

void Sprite::Update(){
	//����λ��
	POINT ptNewPosition,ptSpriteSize,ptBoundsSize;
	ptNewPosition.x = m_rcPosition.left + m_ptVelocity.x;
	ptNewPosition.y = m_rcPosition.top + m_ptVelocity.y;
	ptSpriteSize.x = m_rcPosition.right - m_rcPosition.left;
	ptSpriteSize.y = m_rcPosition.bottom  - m_rcPosition.top;
	ptBoundsSize.x  = m_rcBounds.right - m_rcBounds.left;
	ptBoundsSize.y = m_rcBounds.bottom - m_rcBounds.top;

	//���߽綯��
	//����
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
	//����
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
	//ֹͣ��Ĭ�ϣ�
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
}

void Sprite::Draw(HDC hDC){
	//���û�����أ�������ӻ���
	if(m_pBitmap != NULL && !m_bHidden){
		m_pBitmap->Draw(hDC,m_rcPosition.left,m_rcPosition.top,TRUE);
	}
}