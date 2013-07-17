#include "PersonSprite.h"

PersonSprite::PersonSprite(Bitmap *pBitmap,RECT &rcBounds,BOUNDSACTION baBoundsAction)
	:Sprite(pBitmap,rcBounds,baBoundsAction){
}

PersonSprite::~PersonSprite(){
}

//-----------------------------------------------------------------
// PersonSprite Virtual Methods
//-----------------------------------------------------------------
inline void PersonSprite::UpdateFrame()
{
	//������дUpdateFrame�������Դ�϶���֡Ĭ���Զ�����
}

//-----------------------------------------------------------------
// PersonSprite General Methods
//-----------------------------------------------------------------
void PersonSprite::Walk()
{
  // Toggle between the two frames to give the illusion of walking
  m_iCurFrame = 1 - m_iCurFrame;
}