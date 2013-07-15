//-----------------------------------------------------------------
// Game Application
// C++ Header - Game.h
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include "Resource.h"
#include "GameEngine.h"
#include "Bitmap.h"
#include "Sprite.h"
#include "Background.h"
//-----------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------
HINSTANCE   g_hInstance;
GameEngine* g_pGame;
HDC         g_hOffscreenDC;
HBITMAP g_hOffscreenBitmap;
Bitmap* g_pAsteroidBitmap;//���ͼ��ʵ���������ǵ�һϵ��֡ͼ��
StarryBackground* g_pBackground;
