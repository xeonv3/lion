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
Bitmap* g_pAsteroidBitmap;//这个图像实际上是行星的一系列帧图像
StarryBackground* g_pBackground;
