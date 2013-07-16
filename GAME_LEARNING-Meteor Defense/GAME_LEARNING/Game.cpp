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
		TEXT("TOM GAME"), IDI_ICON, IDI_ICON_SM, 600, 450);
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
	g_pGroundBitmap = new Bitmap(hDC,IDB_GROUND,g_hInstance);
	g_pTargetBitmap = new Bitmap(hDC,IDB_TARGET,g_hInstance);
	g_pCityBitmap = new Bitmap(hDC,IDB_CITY,g_hInstance);
	g_pMeteorBitmap = new Bitmap(hDC,IDB_METEOR,g_hInstance);
	g_pMissileBitmap = new Bitmap(hDC,IDB_MISSILE,g_hInstance);
	g_pExplosionBitmap = new Bitmap(hDC,IDB_EXPLOSION,g_hInstance);
	g_pGameOverBitmap = new Bitmap(hDC,IDB_GAMEOVER,g_hInstance);

	g_pBackground = new StarryBackground(600,450);
	g_pGame->PlayMIDISong(TEXT("Music.mid"));
	//开始游戏
	NewGame();
}

void GameEnd()
{
	g_pGame->CloseMIDIPlayer();
	DeleteObject(g_hOffscreenBitmap);
	DeleteDC(g_hOffscreenDC);

	delete g_pGroundBitmap;
	delete g_pTargetBitmap;
	delete g_pCityBitmap;
	delete g_pMeteorBitmap;
	delete g_pMissileBitmap;
	delete g_pExplosionBitmap;
	delete g_pGameOverBitmap;

	delete g_pBackground;

	g_pGame->CleanupSprites();
	// Cleanup the game engine
	delete g_pGame;
}

void GameActivate(HWND hWindow)
{
	 g_pGame->PlayMIDISong(TEXT(""), FALSE);
}

void GameDeactivate(HWND hWindow)
{
	 g_pGame->PauseMIDISong();
}

void GamePaint(HDC hDC)
{
	// Draw the background
	g_pBackground->Draw(hDC);
	g_pGroundBitmap->Draw(hDC,0,398,TRUE);
	// Draw the sprites
	g_pGame->DrawSprites(hDC);
	//绘制得分
	TCHAR szText[64];
	RECT rect = {275,0,325,50};
	wsprintf(szText,TEXT("%d"),g_iScore);
	SetBkMode(hDC,TRANSPARENT);
	SetTextColor(hDC,RGB(255,255,255));
	DrawText(hDC,szText,-1,&rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	if(g_bGameOver)
		g_pGameOverBitmap->Draw(hDC,170,150,TRUE);
}

void GameCycle()
{
	if(!g_bGameOver){
		if((rand()%g_iDifficulty) == 0)
			AddMeteor();

		g_pBackground->Update();
		g_pGame->UpdateSprites();

		HWND hWindow = g_pGame->GetWindow();
		HDC hDC = GetDC(hWindow);

		GamePaint(g_hOffscreenDC);

		BitBlt(hDC,0,0,g_pGame->GetWidth(),g_pGame->GetHeight(),
			g_hOffscreenDC,0,0,SRCCOPY);
		ReleaseDC(hWindow,hDC);
	}
}

void HandleKeys()
{
}

void MouseButtonDown(int x, int y, BOOL bLeft)
{
	if(!g_bGameOver && bLeft){
		RECT rcBounds = {0,0,600,450};
		int iXPos = (x<300)?144:499;
		Sprite *pSprite = new Sprite(g_pMissileBitmap,rcBounds,BA_DIE);
		pSprite->SetPosition(iXPos,365);
		int iXVel,iYVel = -6;
		y = min(y,300);
		iXVel = (iYVel * ((iXPos + 8) -x )) / (365-y);
		pSprite->SetVelocity(iXVel,iYVel);
		g_pGame->AddSprite(pSprite);
		PlaySound((LPCWSTR)IDR_FIRE,g_hInstance,SND_ASYNC|SND_RESOURCE|SND_NOSTOP);
		g_iScore = max(-g_iScore,0);
	}
	else if(g_bGameOver && !bLeft){
		NewGame();
	}
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{
}

void MouseMove(int x, int y)
{
	g_pTargetSprite->SetPosition(x-(g_pTargetSprite->GetWidth())/2,y-(g_pTargetSprite->GetHeight()/2));
}

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
	if((pSpriteHitter->GetBitmap() == g_pMissileBitmap && 
		pSpriteHittee->GetBitmap() == g_pMeteorBitmap) || 
		(pSpriteHitter->GetBitmap() == g_pMeteorBitmap &&
		pSpriteHittee->GetBitmap() == g_pMissileBitmap))
	{
		pSpriteHitter->Kill();
		pSpriteHittee->Kill();

		g_iScore += 6;
		g_iDifficulty = max(50-(g_iScore/10),5);

	}

	if(pSpriteHittee->GetBitmap() == g_pCityBitmap && 
		pSpriteHitter->GetBitmap() == g_pMeteorBitmap)
	{
		PlaySound((LPCWSTR)IDR_BIGEXPLODE,g_hInstance,SND_ASYNC|SND_RESOURCE);
		pSpriteHitter->Kill();
		pSpriteHittee->Kill();

		if(--g_iNumCities == 0)
			g_bGameOver = TRUE;
	}
	return FALSE;
}

void SpriteDying(Sprite *pSpriteDying){
	if(pSpriteDying->GetBitmap() == g_pMeteorBitmap){
		PlaySound((LPCWSTR)IDR_EXPLODE,g_hInstance,SND_ASYNC|SND_RESOURCE);
		RECT rcBounds = {0,0,600,450};
		RECT rcPos = pSpriteDying->GetPosition();
		Sprite*pSprite = new Sprite(g_pExplosionBitmap,rcBounds);
		pSprite->SetNumFrames(12,TRUE);
		pSprite->SetPosition(rcPos.left,rcPos.top);
		g_pGame->AddSprite(pSprite);
	}
}

void NewGame()
{
  // Clear the sprites
  g_pGame->CleanupSprites();

  // Create the target sprite
  RECT rcBounds = { 0, 0, 600, 450 };
  g_pTargetSprite = new Sprite(g_pTargetBitmap, rcBounds, BA_STOP);
  g_pTargetSprite->SetZOrder(10);
  g_pGame->AddSprite(g_pTargetSprite);

  // Create the city sprites
  Sprite* pSprite = new Sprite(g_pCityBitmap, rcBounds);
  pSprite->SetPosition(2, 370);
  g_pGame->AddSprite(pSprite);
  pSprite = new Sprite(g_pCityBitmap, rcBounds);
  pSprite->SetPosition(186, 370);
  g_pGame->AddSprite(pSprite);
  pSprite = new Sprite(g_pCityBitmap, rcBounds);
  pSprite->SetPosition(302, 370);
  g_pGame->AddSprite(pSprite);
  pSprite = new Sprite(g_pCityBitmap, rcBounds);
  pSprite->SetPosition(490, 370);
  g_pGame->AddSprite(pSprite);

  // Initialize the game variables
  g_iScore = 0;
  g_iNumCities = 4;
  g_iDifficulty = 50;
  g_bGameOver = FALSE;

  // Play the background music
  g_pGame->PlayMIDISong();
}


void AddMeteor()
{
  // Create a new meteor sprite and set its position
  RECT    rcBounds = { 0, 0, 600, 390 };
  int     iXPos = rand() % 600;
  Sprite* pSprite = new Sprite(g_pMeteorBitmap, rcBounds, BA_DIE);
  pSprite->SetNumFrames(14);
  pSprite->SetPosition(iXPos, 0);

  // Calculate the velocity so that it is aimed at one of the cities
  int iXVel, iYVel = (rand() % 4) + 3;
  switch(rand() % 4)
  {
  case 0:
    iXVel = (iYVel * (56 - (iXPos + 50))) / 400;
    break;
  case 1:
    iXVel = (iYVel * (240 - (iXPos + 50))) / 400;
    break;
  case 2:
    iXVel = (iYVel * (360 - (iXPos + 50))) / 400;
    break;
  case 3:
    iXVel = (iYVel * (546 - (iXPos + 50))) / 400;
    break;
  }
  pSprite->SetVelocity(iXVel, iYVel);

  // Add the meteor sprite
  g_pGame->AddSprite(pSprite);
}
