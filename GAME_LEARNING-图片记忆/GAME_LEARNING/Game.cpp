#include "Game.h"

/*
游戏引擎函数
*/
BOOL GameInitialize(HINSTANCE hInstance){
	//创建游戏引擎
	g_pGame = new GameEngine(hInstance,TEXT("Crop Circles"),TEXT("Crop Circles"),IDI_ICON1,IDI_ICON2,528,512);
	if(g_pGame == NULL) 
		return FALSE;

	g_pGame->SetFrameDelay(1);
	g_hInstance = hInstance;
	return TRUE;
}

void GameStart(HWND hWindow){
	srand(GetTickCount());
	HDC hDC = GetDC(hWindow);
	g_pTiles[0] = new Bitmap(hDC,IDB_BITMAP1,g_hInstance);
	g_pTiles[1] = new Bitmap(hDC,IDB_BITMAP2,g_hInstance);
	g_pTiles[2] = new Bitmap(hDC,IDB_BITMAP3,g_hInstance);
	g_pTiles[3] = new Bitmap(hDC,IDB_BITMAP4,g_hInstance);
	g_pTiles[4] = new Bitmap(hDC,IDB_BITMAP5,g_hInstance);
	g_pTiles[5] = new Bitmap(hDC,IDB_BITMAP6,g_hInstance);
	g_pTiles[6] = new Bitmap(hDC,IDB_BITMAP7,g_hInstance);
	g_pTiles[7] = new Bitmap(hDC,IDB_BITMAP8,g_hInstance);
	g_pTiles[8] = new Bitmap(hDC,IDB_BITMAP9,g_hInstance);

	for(int i = 0; i < 4; ++i){
		for(int j = 0; j < 4; ++j){
			g_bTileStates[i][j]=FALSE;
			g_iTiles[i][j]=0;
		}
	}

	for(int i = 0; i < 2; ++i){
		for(int j = 1; j < 9; ++j){
			int x = rand() % 4;
			int y = rand() % 4;
			while(g_iTiles[x][y] != 0){
				x = rand() % 4; 
				y = rand() % 4;
			}
			g_iTiles[x][y] = j;
		}
		g_ptTile1.x = g_ptTile1.y = -1;
		g_ptTile2.x = g_ptTile2.y = -1;
		g_iMatches = g_iTries = 0;
	}

}

void GameEnd(){
	//清理游戏引擎
	for(int i = 0; i < 9; ++i)
		delete g_pTiles[i];
	delete g_pGame;
}

void GameActivate(HWND hWindow){

}

void GameDeactivate(HWND hWindow){

}

void GamePaint(HDC hDC){
	int iTileWidth = g_pTiles[0]->GetWidth();
	int iTileHeight = g_pTiles[0]->GetHeight();
	for(int i = 0; i < 4; ++i){
		for(int j = 0; j < 4; ++j){
			if(g_bTileStates[i][j] || ((i==g_ptTile1.x)&&(j==g_ptTile1.y))||
				((i==g_ptTile2.x)&&(j==g_ptTile2.y))){
					g_pTiles[g_iTiles[i][j]]->Draw(hDC,i*iTileWidth,j*iTileHeight,TRUE);
			}else{
				g_pTiles[0]->Draw(hDC,i*iTileWidth,j*iTileHeight,TRUE);
			}
		}
	}
}

void GameCycle(){
}

void HandleKeys(){
	//if(GetAsyncKeyState(VK_LEFT) < 0)
	//	g_iSpeedX = max(-g_iMAXSPEED,--g_iSpeedX);
	//else if(GetAsyncKeyState(VK_RIGHT) < 0)
	//	g_iSpeedX = min(g_iMAXSPEED,++g_iSpeedX);
	//else if(GetAsyncKeyState(VK_UP) < 0)
	//	g_iSpeedY = max(-g_iMAXSPEED,--g_iSpeedY);
	//else if(GetAsyncKeyState(VK_DOWN) < 0)
	//	g_iSpeedY = min(g_iMAXSPEED,++g_iSpeedY);
}

void MouseButtonDown(int x,int y, BOOL bLeft){

	int iTileX = x / g_pTiles[0]->GetWidth();
	int iTileY = y / g_pTiles[0]->GetHeight();
	if(!g_bTileStates[iTileX][iTileY]){
		if(g_ptTile1.x == -1){
			PlaySound((LPCWSTR)IDR_SELECT,g_hInstance,SND_ASYNC|SND_RESOURCE);
			g_ptTile1.x = iTileX;
			g_ptTile1.y = iTileY;
		}else if(g_ptTile1.x != iTileX || g_ptTile1.y != iTileY){
			if(g_ptTile2.x == -1){
				++g_iTries;
				PlaySound((LPCWSTR)IDR_SELECT,g_hInstance,SND_ASYNC|SND_RESOURCE);
				g_ptTile2.x = iTileX;
				g_ptTile2.y = iTileY;

				if(g_iTiles[g_ptTile1.x][g_ptTile1.y] == g_iTiles[g_ptTile2.x][g_ptTile2.y]){
					g_bTileStates[g_ptTile1.x][g_ptTile1.y]=TRUE;
					g_bTileStates[g_ptTile2.x][g_ptTile2.y]=TRUE;
					PlaySound((LPCWSTR)IDR_MATCH,g_hInstance,SND_ASYNC|SND_RESOURCE);
					g_ptTile1.x = g_ptTile1.y = g_ptTile2.x = g_ptTile2.y = -1;

					if(++g_iMatches == 8){
						TCHAR szText[64];
						PlaySound((LPCWSTR)IDR_WIN,g_hInstance,SND_ASYNC|SND_RESOURCE);
						wsprintf(szText, TEXT("You won in %d tries."), g_iTries);
						MessageBox(g_pGame->GetWindow(), szText, TEXT("Brainiac"), MB_OK);
					}
				}
			}else{
				PlaySound((LPCWSTR)IDR_MISMATCH,g_hInstance,SND_ASYNC|SND_RESOURCE);
				g_ptTile1.x = g_ptTile1.y = g_ptTile2.x = g_ptTile2.y = -1;
			}
		}
		InvalidateRect(g_pGame->GetWindow(),NULL,FALSE);
	}
}

void MouseButtonUp(int x,int y,BOOL bLeft){

}
void MouseMove(int x,int y){
}