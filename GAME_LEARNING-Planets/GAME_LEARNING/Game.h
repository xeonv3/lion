#pragma once

#include <windows.h>
#include "resource.h"
#include "GameEngine.h"
#include "Bitmap.h"
#include "Sprite.h"

HINSTANCE g_hInstance;
GameEngine* g_pGame;
Bitmap* g_pGalaxyBitmap;
Bitmap* g_pPlanetBitmap[3];
Sprite* g_pPlanetSprite[3];
BOOL g_bDragging;
int g_iDragPlanet;//这个变量记录拖动了哪一颗行星