#pragma once

#include <windows.h>
#include "resource.h"
#include "GameEngine.h"
#include "Bitmap.h"
#include "Sprite.h"

HINSTANCE g_hInstance;
GameEngine* g_pGame;
HDC g_hOffscreenDC;
HBITMAP g_hOffscreenBitmap;
Bitmap* g_pGalaxyBitmap;
Bitmap* g_pPlanetBitmap[3];
Sprite* g_pDragSprite;