//-----------------------------------------------------------------
// Game Application
// C++ Source - Game.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Game.h"

//-----------------------------------------------------------------
// Game Engine Functions
//-----------------------------------------------------------------
BOOL GameInitialize(HINSTANCE hInstance)
{
	// Create the game engine
	g_pGame = new GameEngine(hInstance, TEXT("GAME PROGRAMMING"),
		TEXT("TOM GAME"), IDI_ICON, IDI_ICON_SM, 500, 400);
	if (g_pGame == NULL)
		return FALSE;

	// Set the frame rate
	g_pGame->SetFrameRate(30);

	// Store the instance handle
	g_hInstance = hInstance;

	return TRUE;
}

void GameStart(HWND hWindow)
{
	// Seed the random number generator
	srand(GetTickCount());

	//创建屏幕外设备环境和位图
	g_hOffscreenDC = CreateCompatibleDC(GetDC(hWindow));
	g_hOffscreenBitmap  = CreateCompatibleBitmap(GetDC(hWindow),g_pGame->GetWidth(),g_pGame->GetHeight());
	SelectObject(g_hOffscreenDC,g_hOffscreenBitmap);

	//创建并加载行星位图
	HDC hDC = GetDC(hWindow);
	g_pAsteroidBitmap = new Bitmap(hDC,IDB_ASTEROID,g_hInstance);

	g_pBackground = new StarryBackground(500,400);
	RECT rcBounds = {0,0,500,400};
	Sprite* pSprite;
	pSprite = new Sprite(g_pAsteroidBitmap,rcBounds,BA_WRAP);
	pSprite->SetNumFrames(14);
	pSprite->SetFrameDelay(1);
	pSprite->SetPosition(150,200);
	pSprite->SetVelocity(-3,1);
	g_pGame->AddSprite(pSprite);
	pSprite = new Sprite(g_pAsteroidBitmap, rcBounds, BA_WRAP);
	pSprite->SetNumFrames(14);
	pSprite->SetFrameDelay(2);
	pSprite->SetPosition(250, 200);
	pSprite->SetVelocity(3, -2);
	g_pGame->AddSprite(pSprite);
	pSprite = new Sprite(g_pAsteroidBitmap, rcBounds, BA_WRAP);
	pSprite->SetNumFrames(14);
	pSprite->SetFrameDelay(3);
	pSprite->SetPosition(250, 100);
	pSprite->SetVelocity(-2, -4);
	g_pGame->AddSprite(pSprite);
}

void GameEnd()
{
  // Cleanup the offscreen device context and bitmap
  DeleteObject(g_hOffscreenBitmap);
  DeleteDC(g_hOffscreenDC);  

  // Cleanup the asteroid bitmap
  delete g_pAsteroidBitmap;

  // Cleanup the background
  delete g_pBackground;

  // Cleanup the sprites
  g_pGame->CleanupSprites();

  // Cleanup the game engine
  delete g_pGame;
}

void GameActivate(HWND hWindow)
{

}

void GameDeactivate(HWND hWindow)
{

}

void GamePaint(HDC hDC)
{
  // Draw the background
  g_pBackground->Draw(hDC);

  // Draw the sprites
  g_pGame->DrawSprites(hDC);
}

void GameCycle()
{
	g_pBackground->Update();
	g_pGame->UpdateSprites();

	HWND hWindow = g_pGame->GetWindow();
	HDC hDC = GetDC(hWindow);

	GamePaint(g_hOffscreenDC);

	BitBlt(hDC,0,0,g_pGame->GetWidth(),g_pGame->GetHeight(),
		g_hOffscreenDC,0,0,SRCCOPY);
	ReleaseDC(hWindow,hDC);
}

void HandleKeys()
{
}

void MouseButtonDown(int x, int y, BOOL bLeft)
{
	
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{
}

void MouseMove(int x, int y)
{

}

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
	/*
	POINT v = pSpriteHitter->GetVelocity();
	pSpriteHitter->SetVelocity(pSpriteHittee->GetVelocity());
	pSpriteHittee->SetVelocity(v);
	return TRUE;
	*/
	return FALSE;
}
