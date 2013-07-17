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
	//必须重写UpdateFrame方法，以打断动画帧默认自动更新
}

//-----------------------------------------------------------------
// PersonSprite General Methods
//-----------------------------------------------------------------
void PersonSprite::Walk()
{
  // Toggle between the two frames to give the illusion of walking
  m_iCurFrame = 1 - m_iCurFrame;
}