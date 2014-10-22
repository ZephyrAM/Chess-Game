#include "game.h"

LPDIRECT3DSURFACE9 titleScreen;
LPDIRECT3DSURFACE9 gameScreen;
LPDIRECT3DSURFACE9 rulesScreen;
LPDIRECT3DSURFACE9 creditsScreen;
LPDIRECT3DSURFACE9 endScreen;

LPDIRECT3DTEXTURE9 select_Image;
SPRITE playerSquare;
SPRITE *playerActive;
SPRITE *piecePtr[8][8];

LPDIRECT3DTEXTURE9 tsPieceBl_Image;
SPRITE tsKingBl;

LPDIRECT3DTEXTURE9 tsPieceWh_Image;
SPRITE tsKingWh;

#define NONE	0
#define BLACK	1
#define WHITE	2

#define PAWN	10
#define ROOK	11
#define KNIGHT	12
#define BISHOP	13
#define KING	14
#define QUEEN	15

//White 
//*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPDIRECT3DTEXTURE9 wPiece_Image;
SPRITE whitePawn[8];
SPRITE whiteRook[2];
SPRITE whiteKnight[2];
SPRITE whiteBishop[2];
SPRITE whiteKing;
SPRITE whiteQueen;

//Black
//*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPDIRECT3DTEXTURE9 bPiece_Image;
SPRITE blackPawn[8];
SPRITE blackRook[2];
SPRITE blackKnight[2];
SPRITE blackBishop[2];
SPRITE blackKing;
SPRITE blackQueen;

LPD3DXSPRITE sprite_handler;

CSound *sound_music;
CSound *sound_squareMove;
CSound *sound_pieceSelect;

int stage = 0;
int pieceY, pieceX, tempY, tempX, newY, newX;
bool activePiece = false;
int activeType = NONE;
int activeTeam = WHITE;

GAMEBOARD board;

HRESULT result;

//timing variable
long start = GetTickCount();

//initializes the game
int Game_Init(HWND hwnd)
{
	//set random number seed
	srand(time(NULL));

	// Create the game board
	Board_Init();
	for(int nY = 0; nY <= 1; nY++)
	{
		for(int nX = 0; nX <= 7; nX++)
		{
			board.cell[nY][nX].isOccupied = true;
			board.cell[nY][nX].isTeam = BLACK;
		}
	}

	for(int nY = 7; nY >= 6; nY--)
	{
		for(int nX = 0; nX <=7; nX++)
		{
			board.cell[nY][nX].isOccupied = true;
			board.cell[nY][nX].isTeam = WHITE;
		}
	}

	 //initialize keyboard
	if (!Init_Keyboard(hwnd))
	{
		MessageBox(hwnd, "Error initializing the keyboard", "Error", MB_OK);
		return 0;
	}

	//create sprite handler object
	result = D3DXCreateSprite(d3ddev, &sprite_handler);
	if (result != D3D_OK)
		return 0;
	
	//load the music wave file
	sound_music = LoadSound("Prelude.wav");
	if (sound_music == NULL)
		return 0;

	//load square selecter movement wave file
	sound_squareMove = LoadSound("squareMove.wav");
	if (sound_squareMove == NULL)
	return 0;

	//load piece select wave file
	sound_pieceSelect = LoadSound("pieceSelect.wav");
	if (sound_pieceSelect == NULL)
	return 0;

	//Load the title screen 
	titleScreen = LoadSurface("titleScreen.png", NULL);
	if(titleScreen == NULL)
		return 0;

		//load the background image
	gameScreen = LoadSurface("BasicBoard.png", NULL);
	if(gameScreen == NULL)
		return 0;

			//load the background image
	rulesScreen = LoadSurface("rulesScreen.png", NULL);
	if(rulesScreen == NULL)
		return 0;

		//load the background image
	creditsScreen = LoadSurface("creditsScreen.png", NULL);
	if(creditsScreen == NULL)
		return 0;

	select_Image = LoadTexture("Selected1.png", D3DCOLOR_XRGB(255,200,255));
	if(select_Image == NULL)
		return 0;

	playerSquare.x = 340;
	playerSquare.y = 250;
	playerSquare.width = 60;
	playerSquare.height = 50;
	playerSquare.curframe = 0;
	playerSquare.lastframe = 1;
	playerSquare.animcount = 0;
	playerSquare.animdelay = 2000;
	
	tsPieceBl_Image = LoadTexture("tsKing.png", D3DCOLOR_XRGB(255,200,255));
	if(tsPieceBl_Image == NULL)
		return 0;

	tsKingBl.x = 620;
	tsKingBl.y = 350;
	tsKingBl.width = 50;
	tsKingBl.height = 50;
	tsKingBl.curframe = 1;
	tsKingBl.lastframe = 3;
	tsKingBl.animcount = 0;
	tsKingBl.animdelay = 3000;

	tsPieceWh_Image = LoadTexture("tsKing1.png", D3DCOLOR_XRGB(255,200,255));
	if(tsPieceWh_Image == NULL)
		return 0;

	tsKingWh.x = 120;
	tsKingWh.y = 350;
	tsKingWh.width = 50;
	tsKingWh.height = 50;
	tsKingWh.curframe = 1;
	tsKingWh.lastframe = 3;
	tsKingWh.animcount = 0;
	tsKingWh.animdelay = 3000;

	bPiece_Image = LoadTexture("blackPieces.png", D3DCOLOR_XRGB(255,200,255));
	if(bPiece_Image == NULL)
		return 0;

	for(int n = 0; n < 8; n++)
	{
		blackPawn[n].x = (165 + (60 * n));
		blackPawn[n].y = 140;
		blackPawn[n].width = 50;
		blackPawn[n].height = 50;
		blackPawn[n].curframe = 1;
		blackPawn[n].lastframe = 3;
		blackPawn[n].baseframe = 1;
		blackPawn[n].animcount = 0;
		blackPawn[n].animdelay = 2000;
		blackPawn[n].active = false;
		blackPawn[n].posY = 1;
		blackPawn[n].posX = (0 + n);
		blackPawn[n].team = BLACK;
		blackPawn[n].type = PAWN;
		blackPawn[n].alive = true;
	}

	for(int n = 0; n < 2; n++)
	{
		blackRook[n].y = 90;
		blackRook[n].width = 50;
		blackRook[n].height = 50;
		blackRook[n].curframe = (1 + 48);
		blackRook[n].lastframe = (3 + 48);
		blackRook[n].baseframe = (1 + 48);
		blackRook[n].animcount = 0;
		blackRook[n].animdelay = 2000;
		blackRook[n].active = false;
		blackRook[n].posY = 0;
		blackRook[n].team = BLACK;
		blackRook[n].type = ROOK;
		blackRook[n].alive = true;
	}
	blackRook[0].x = 165;
	blackRook[1].x = 585;
	blackRook[0].posX = 0;
	blackRook[1].posX = 7;

	for(int n = 0; n < 2; n++)
	{
		blackKnight[n].y = 90;
		blackKnight[n].width = 50;
		blackKnight[n].height = 50;
		blackKnight[n].curframe = (1 + 4);
		blackKnight[n].lastframe = (3 + 4);
		blackKnight[n].baseframe = (1 + 4);
		blackKnight[n].animcount = 0;
		blackKnight[n].animdelay = 2000;
		blackKnight[n].active = false;
		blackKnight[n].posY = 0;
		blackKnight[n].team = BLACK;
		blackKnight[n].type = KNIGHT;
		blackKnight[n].alive = true;
	}
	blackKnight[0].x = 225;
	blackKnight[1].x = 525;
	blackKnight[0].posX = 1;
	blackKnight[1].posX = 6;

	for(int n = 0; n < 2; n++)
	{
		blackBishop[n].y = 90;
		blackBishop[n].width = 50;
		blackBishop[n].height = 50;
		blackBishop[n].curframe = (1 + 8);
		blackBishop[n].lastframe = (3 + 8);
		blackBishop[n].baseframe = (1 + 8);
		blackBishop[n].animcount = 0;
		blackBishop[n].animdelay = 2000;
		blackBishop[n].active = false;
		blackBishop[n].posY = 0;
		blackBishop[n].team = BLACK;
		blackBishop[n].type = BISHOP;
		blackBishop[n].alive = true;
	}
	blackBishop[0].x = 285;
	blackBishop[1].x = 465;
	blackBishop[0].posX = 2;
	blackBishop[1].posX = 5;


	blackKing.x = 345;
	blackKing.y = 90;
	blackKing.width = 50;
	blackKing.height = 50;
	blackKing.curframe = ((1 + 4) + 48);
	blackKing.lastframe = ((3 + 4) + 48);
	blackKing.baseframe = ((1 + 4) + 48);
	blackKing.animcount = 0;
	blackKing.animdelay = 2000;
	blackKing.active = false;
	blackKing.posY = 0;
	blackKing.posX = 3;
	blackKing.team = BLACK;
	blackKing.type = KING;
	blackKing.alive = true;


	blackQueen.x = 405;
	blackQueen.y = 90;
	blackQueen.width = 50;
	blackQueen.height =50;
	blackQueen.curframe = ((1 + 8) + 48);
	blackQueen.lastframe = ((3 + 8) + 48);
	blackQueen.baseframe = ((1 + 8) + 48);
	blackQueen.animcount = 0;
	blackQueen.animdelay = 2000;
	blackQueen.active = false;
	blackQueen.posY = 0;
	blackQueen.posX = 4;
	blackQueen.team = BLACK;
	blackQueen.type = QUEEN;
	blackQueen.alive = true;

	wPiece_Image = LoadTexture("whitePieces.png", D3DCOLOR_XRGB(255,200,255));
	if(wPiece_Image == NULL)
		return 0;

	for (int n = 0; n < 8; n++)
	{
		whitePawn[n].x = (165 + (60 * n));
		whitePawn[n].y = 390;
		whitePawn[n].width = 50;
		whitePawn[n].height = 50;
		whitePawn[n].curframe = (1 + 36);
		whitePawn[n].lastframe = (3 + 36);
		whitePawn[n].baseframe = (1 + 36);
		whitePawn[n].animcount = 0;
		whitePawn[n].animdelay = 2000;
		whitePawn[n].active = false;
		whitePawn[n].posY = 6;
		whitePawn[n].posX = (0 + n);
		whitePawn[n].team = WHITE;
		whitePawn[n].type = PAWN;
		whitePawn[n].alive = true;
	}

	for(int n = 0; n < 2; n++)
	{
		whiteRook[n].y = 440;
		whiteRook[n].width = 50;
		whiteRook[n].height = 50;
		whiteRook[n].curframe = (1 + 84);
		whiteRook[n].lastframe = (3 + 84);
		whiteRook[n].baseframe = (1 + 84);
		whiteRook[n].animcount = 0;
		whiteRook[n].animdelay = 2000;
		whiteRook[n].active = false;
		whiteRook[n].posY = 7;
		whiteRook[n].team = WHITE;
		whiteRook[n].type = ROOK;
		whiteRook[n].alive = true;
	}
	whiteRook[0].x = 165;
	whiteRook[1].x = 585;
	whiteRook[0].posX = 0;
	whiteRook[1].posX = 7;

	for(int n = 0; n < 2; n++)
	{
		whiteKnight[n].y = 440;
		whiteKnight[n].width = 50;
		whiteKnight[n].height = 50;
		whiteKnight[n].curframe = ((1 + 4) + 36);
		whiteKnight[n].lastframe = ((3 + 4) + 36);
		whiteKnight[n].baseframe = ((1 + 4) + 36);
		whiteKnight[n].animcount = 0;
		whiteKnight[n].animdelay = 2000;
		whiteKnight[n].active = false;
		whiteKnight[n].posY = 7;
		whiteKnight[n].team = WHITE;
		whiteKnight[n].type = KNIGHT;
		whiteKnight[n].alive = true;
	}
	whiteKnight[0].x = 225;
	whiteKnight[1].x = 525;
	whiteKnight[0].posX = 1;
	whiteKnight[1].posX = 6;

	for(int n = 0; n < 2; n++)
	{
		whiteBishop[n].y = 440;
		whiteBishop[n].width = 50;
		whiteBishop[n].height = 50;
		whiteBishop[n].curframe = ((1 + 8) + 36);
		whiteBishop[n].lastframe = ((3 + 8) + 36);
		whiteBishop[n].baseframe = ((1 + 8) + 36);
		whiteBishop[n].animcount = 0;
		whiteBishop[n].animdelay = 2000;
		whiteBishop[n].active = false;
		whiteBishop[n].posY = 7;
		whiteBishop[n].team = WHITE;
		whiteBishop[n].type = BISHOP;
		whiteBishop[n].alive = true;
	}
	whiteBishop[0].x = 285;
	whiteBishop[1].x = 465;
	whiteBishop[0].posX = 2;
	whiteBishop[1].posX = 5;


	whiteKing.x = 345;
	whiteKing.y = 440;
	whiteKing.width = 50;
	whiteKing.height = 50;
	whiteKing.curframe = ((1 + 4) + 84);
	whiteKing.lastframe = ((3 + 4) + 84);
	whiteKing.baseframe = ((1 + 4) + 84);
	whiteKing.animcount = 0;
	whiteKing.animdelay = 2000;
	whiteKing.active = false;
	whiteKing.posY = 7;
	whiteKing.posX = 3;
	whiteKing.team = WHITE;
	whiteKing.type = KING;
	whiteKing.alive = true;


	whiteQueen.x = 405;
	whiteQueen.y = 440;
	whiteQueen.width = 50;
	whiteQueen.height =50;
	whiteQueen.curframe = ((1 + 8) + 84);
	whiteQueen.lastframe = ((3 + 8) + 84);
	whiteQueen.baseframe = ((1 + 8) + 84);
	whiteQueen.animcount = 0;
	whiteQueen.animdelay = 2000;
	whiteQueen.active = false;
	whiteQueen.posY = 7;
	whiteQueen.posX = 4;
	whiteQueen.team = WHITE;
	whiteQueen.type = QUEEN;
	whiteQueen.alive = true;

	for(int n = 0;n < 8;n++)
	{
		piecePtr[1][blackPawn[n].posX] = &blackPawn[n];
		piecePtr[6][whitePawn[n].posX] = &whitePawn[n];
	}
	for(int n = 0;n < 2;n++)
	{
		piecePtr[blackRook[n].posY][blackRook[n].posX] = &blackRook[n];
		piecePtr[whiteRook[n].posY][whiteRook[n].posX] = &whiteRook[n];
		piecePtr[blackKnight[n].posY][blackKnight[n].posX] = &blackKnight[n];
		piecePtr[whiteKnight[n].posY][whiteKnight[n].posX] = &whiteKnight[n];
		piecePtr[blackBishop[n].posY][blackBishop[n].posX] = &blackBishop[n];
		piecePtr[whiteBishop[n].posY][whiteBishop[n].posX] = &whiteBishop[n];
	}
	piecePtr[blackKing.posY][blackKing.posX] = &blackKing;
	piecePtr[whiteKing.posY][whiteKing.posX] = &whiteKing;
	piecePtr[blackQueen.posY][blackQueen.posX] = &blackQueen;
	piecePtr[whiteQueen.posY][whiteQueen.posX] = &whiteQueen;

	//return okay
	return 1;
}

//the main game loop
void Game_Run(HWND hwnd)
{
	// BMC - Get input
	Poll_Keyboard();
	Poll_Mouse();
	//PlaySound(sound_music);

	//make sure the Direct3D device is valid
	if (d3ddev == NULL)
		return;

	D3DXVECTOR3 position(0,0,0);

	//after short delay, ready for next frame?
	//this keeps the game running at a steady frame rate
	if (GetTickCount() - start >= 60)
	{
		//reset timing
		start = GetTickCount();

		if(activePiece == true)
		{
			if(KEY_DOWN(VK_RIGHT))
			{
				playerSquare.x += 60;
				playerActive->x += 60;
				PlaySound(sound_squareMove);
			}
			if(KEY_DOWN(VK_LEFT))
			{
				playerSquare.x -= 60;
				playerActive->x -= 60;
				PlaySound(sound_squareMove);
			}
			if(KEY_DOWN(VK_DOWN))
			{
				playerSquare.y += 50;
				playerActive->y += 50;
				PlaySound(sound_squareMove);
			}
			if(KEY_DOWN(VK_UP))
			{
				playerSquare.y -= 50;
				playerActive->y -= 50;
				PlaySound(sound_squareMove);
			}
		}
		else
		{
			if(KEY_DOWN(VK_RIGHT))
			{
				playerSquare.x += 60;
				PlaySound(sound_squareMove);
			}
			if(KEY_DOWN(VK_LEFT))
			{
				playerSquare.x -= 60;
				PlaySound(sound_squareMove);
			}
			if(KEY_DOWN(VK_DOWN))
			{
				playerSquare.y += 50;
				PlaySound(sound_squareMove);
			}
			if(KEY_DOWN(VK_UP))
			{
				playerSquare.y -= 50;
				PlaySound(sound_squareMove);
			}
		}

		//stop at edge of screen
		if(activePiece == true)
		{
			if (playerSquare.x > 640 - playerSquare.width)
			{
				playerSquare.x = 640 - playerSquare.width;
				playerActive->x = 635 - playerActive->width;
			}
			else if (playerSquare.x < 160)
			{
					playerSquare.x = 160;
					playerActive->x = 165;
			}

			if (playerSquare.y > 500 - playerSquare.height)
			{
				playerSquare.y = 500 - playerSquare.height;
				playerActive->y = 490 - playerActive->height;
			}
			else if (playerSquare.y < 100)
			{
					playerSquare.y = 100;
					playerActive->y = 90;
			}
		}
		else
		{
			if (playerSquare.x > 640 - playerSquare.width)
				playerSquare.x = 640 - playerSquare.width;
			else if (playerSquare.x < 160)
					playerSquare.x = 160;

			if (playerSquare.y > 500 - playerSquare.height)
				playerSquare.y = 500 - playerSquare.height;
			else if (playerSquare.y < 100)
					playerSquare.y = 100;
		}
	
	}

	if(KEY_DOWN(VK_RETURN) && stage != 0 && activePiece == false)
	{
		PlaySound(sound_pieceSelect);

		tempX = ((playerSquare.x - 160) / 60);
		tempY = ((playerSquare.y - 100) / 50);

		if (board.cell[tempY][tempX].isOccupied == true &&
			board.cell[tempY][tempX].isTeam == activeTeam)
		{
			activePiece = true;

			playerActive = piecePtr[tempY][tempX];
			playerActive->active = true;
			activeType = playerActive->type;

			pieceY = playerActive->y;
			pieceX = playerActive->x;

			board.cell[tempY][tempX].isOccupied = false;
			board.cell[tempY][tempX].isTeam = NONE;

			toggleBoard();
		}
	}

	/*
	if (KEY_DOWN(VK_RETURN) && stage != 0 && activePiece == false)
	{
		bool done = false;

		tempX = ((playerSquare.x - 160) / 60);
		tempY = ((playerSquare.y - 100) / 50);

		if (board.cell[tempY][tempX].isOccupied == true &&
			board.cell[tempY][tempX].isTeam == activeTeam)
		{
			while(done != true)
			{
				for(int n = 0; n <=7; n++)
				{
					if(blackPawn[n].posY == tempY)
					{
						if(blackPawn[n].posX == tempX &&
							blackPawn[n].alive == true)
						{
							blackPawn[n].active = true;
							activePiece = true;

							pieceX = blackPawn[n].x;
							pieceY = blackPawn[n].y;

							playerActive = &blackPawn[n];
							done = true;
						}
					}
					else if(whitePawn[n].posY == tempY)
					{
						if(whitePawn[n].posX == tempX &&
							whitePawn[n].alive == true)
						{
							whitePawn[n].active = true;
							activePiece = true;

							pieceX = whitePawn[n].x;
							pieceY = whitePawn[n].y;

							playerActive = &whitePawn[n];
							done = true;
						}
					}
				}
				for(int n = 0; n <= 1; n++)
				{
					if(blackRook[n].posY == tempY)
					{
						if(blackRook[n].posX == tempX &&
							blackRook[n].alive == true)
						{
							blackRook[n].active = true;
							activePiece = true;

							pieceX = blackRook[n].x;
							pieceY = blackRook[n].y;

							playerActive = &blackRook[n];
							done = true;
						}
					}
					else if(whiteRook[n].posY == tempY)
					{
						if(whiteRook[n].posX == tempX &&
							whiteRook[n].alive == true)
						{
							whiteRook[n].active = true;
							activePiece = true;

							pieceX = whiteRook[n].x;
							pieceY = whiteRook[n].y;

							playerActive = &whiteRook[n];
							done = true;
						}
					}
					if(blackKnight[n].posY == tempY)
					{
						if(blackKnight[n].posX == tempX &&
							blackKnight[n].alive == true)
						{
							blackKnight[n].active = true;
							activePiece = true;

							pieceX = blackKnight[n].x;
							pieceY = blackKnight[n].y;

							playerActive = &blackKnight[n];
							done = true;
						}
					}
					else if(whiteKnight[n].posY == tempY)
					{
						if(whiteKnight[n].posX == tempX &&
							whiteKnight[n].alive == true)
						{
							whiteKnight[n].active = true;
							activePiece = true;

							pieceX = whiteKnight[n].x;
							pieceY = whiteKnight[n].y;

							playerActive = &whiteKnight[n];
							done = true;
						}
					}
					if(blackBishop[n].posY == tempY)
					{
						if(blackBishop[n].posX == tempX &&
							blackBishop[n].alive == true)
						{
							blackBishop[n].active = true;
							activePiece = true;

							pieceX = blackBishop[n].x;
							pieceY = blackBishop[n].y;

							playerActive = &blackBishop[n];
							done = true;
						}
					}
					else if(whiteBishop[n].posY == tempY)
					{
						if(whiteBishop[n].posX == tempX &&
							whiteBishop[n].alive == true)
						{
							whiteBishop[n].active = true;
							activePiece = true;

							pieceX = whiteBishop[n].x;
							pieceY = whiteBishop[n].y;

							playerActive = &whiteBishop[n];
							done = true;
						}
					}
				}
				if(blackKing.posY == tempY)
				{
					if(blackKing.posX == tempX &&
						blackKing.alive == true)
					{
						blackKing.active = true;
						activePiece = true;

						pieceX = blackKing.x;
						pieceY = blackKing.y;

						playerActive = &blackKing;
						done = true;
					}
				}
				else if(whiteKing.posY == tempY)
				{
					if(whiteKing.posX == tempX &&
						whiteKing.alive == true)
					{
						whiteKing.active = true;
						activePiece = true;

						pieceX = whiteKing.x;
						pieceY = whiteKing.y;

						playerActive = &whiteKing;
						done = true;
					}
				}
				if(blackQueen.posY == tempY)
				{
					if(blackQueen.posX == tempX &&
						blackQueen.alive == true)
					{
						blackQueen.active = true;
						activePiece = true;

						pieceX = blackQueen.x;
						pieceY = blackQueen.y;

						playerActive = &blackQueen;
						done = true;
					}
				}
				else if(whiteQueen.posY == tempY)
				{
					if(whiteQueen.posX == tempX &&
						whiteQueen.alive == true)
					{
						whiteQueen.active = true;
						activePiece = true;

						pieceX = whiteQueen.x;
						pieceY = whiteQueen.y;

						playerActive = &whiteQueen;
						done = true;
					}
				}
				activeType = playerActive->type;
				board.cell[tempY][tempX].isOccupied = false;
				board.cell[tempY][tempX].isTeam = NONE;
			}
			toggleBoard();
		}
	}
	*/

	if (KEY_DOWN(VK_BACK) && activePiece == true)
	{
		activePiece = false;
		toggleBoard();

		activeType = NONE;
		playerActive->active = false;

		playerActive->x = pieceX;
		playerActive->y = pieceY;
		playerActive->curframe = playerActive->baseframe;

		board.cell[tempY][tempX].isOccupied = true;
		board.cell[tempY][tempX].isTeam = playerActive->team;

	}

	if (KEY_DOWN(VK_SPACE) && activePiece == true)
	{
		newX = ((playerSquare.x - 160) / 60);
		newY = ((playerSquare.y - 100) / 50);

		if(board.cell[newY][newX].isActive == true &&
			board.cell[newY][newX].isTeam != playerActive->team)
		{
			if(activeTeam == WHITE)
			{
				if(board.cell[newY][newX].isTeam == BLACK)
					piecePtr[newY][newX]->alive = false;

				activeTeam = BLACK;
			}
			else if(activeTeam == BLACK)
			{
				if(board.cell[newY][newX].isTeam == WHITE)
					piecePtr[newY][newX]->alive = false;

				activeTeam = WHITE;
			}

			activePiece = false;
			toggleBoard();

			piecePtr[newY][newX] = piecePtr[tempY][tempX];
			playerActive = piecePtr[newY][newX];
			piecePtr[tempY][tempX] = NULL;

			activeType = NONE;
			playerActive->active = false;
			playerActive->curframe = playerActive->baseframe;

			playerActive->posY = newY;
			playerActive->posX = newX;

			board.cell[newY][newX].isOccupied = true;
			board.cell[newY][newX].isTeam = playerActive->team;
		}
	}

	playerSquare.animcount++;
	if(playerSquare.animcount > playerSquare.animdelay)
	{
		playerSquare.animcount = 0;

		playerSquare.curframe++;
		if(playerSquare.curframe > playerSquare.lastframe)
			playerSquare.curframe = 0;
	}

	if(activePiece == true)
	{
		playerActive->animcount++;
		if(playerActive->animcount > playerActive->animdelay)
		{
			playerActive->animcount = 0;

			playerActive->curframe++;
			if(playerActive->curframe > playerActive->lastframe)
				playerActive->curframe = (playerActive->baseframe - 1);

			if(playerActive->curframe > playerActive->lastframe)
				playerActive->curframe = (playerActive->baseframe - 1);
		}
	}

	if(stage == 0)
	{
		PlaySound(sound_music);

		 //has animation delay reached threshold?
		if (++tsKingBl.animcount > tsKingBl.animdelay)
		{
			//reset counter
			tsKingBl.animcount = 0;

			//animate the sprite
			if (++tsKingBl.curframe > tsKingBl.lastframe)
				tsKingBl.curframe = 0;
		}

		//has animation delay reached threshold?
		if (++tsKingWh.animcount > tsKingWh.animdelay)
		{
			//reset counter
			tsKingWh.animcount = 0;

			//animate the sprite
			if (++tsKingWh.curframe > tsKingWh.lastframe)
				tsKingWh.curframe = 0;
		}
	}

	//start rendering
	if (d3ddev->BeginScene())
	{
		if(stage == 0)
			d3ddev->StretchRect(titleScreen, NULL, backbuffer, NULL, D3DTEXF_NONE);
		else if(stage == 1)
			d3ddev->StretchRect(gameScreen, NULL, backbuffer, NULL, D3DTEXF_NONE);
		else if(stage == 2)
			d3ddev->StretchRect(rulesScreen, NULL, backbuffer, NULL, D3DTEXF_NONE);
		else if(stage == 3)
			d3ddev->StretchRect(creditsScreen, NULL, backbuffer, NULL, D3DTEXF_NONE);

		//start sprite handler
		sprite_handler->Begin(D3DXSPRITE_ALPHABLEND);

		if(stage == 0)
		{
			RECT srcRect;
			int columns = 4;

			position.x = (float)tsKingBl.x;
			position.y = (float)tsKingBl.y;

			srcRect.left = (tsKingBl.curframe % columns) * tsKingBl.width;
			srcRect.top = (tsKingBl.curframe / columns) * tsKingBl.height;
			srcRect.right = srcRect.left + tsKingBl.width;
			srcRect.bottom = srcRect.top + tsKingBl.height;

			sprite_handler->Draw(tsPieceBl_Image, &srcRect, NULL, &position,
				D3DCOLOR_XRGB(255,255,255));

			position.x = (float)tsKingWh.x;
			position.y = (float)tsKingWh.y;

			srcRect.left = (tsKingWh.curframe % columns) * tsKingWh.width;
			srcRect.top = (tsKingWh.curframe / columns) * tsKingWh.height;
			srcRect.right = srcRect.left + tsKingWh.width;
			srcRect.bottom = srcRect.top + tsKingWh.height;

			sprite_handler->Draw(tsPieceWh_Image, &srcRect, NULL, &position,
				D3DCOLOR_XRGB(255,255,255));

			if(KEY_DOWN('1'))
			stage = 1;

			if(KEY_DOWN('2'))
			stage = 2;

			if(KEY_DOWN('3'))
			stage = 3;

			//check for escape key (to exit program)
			if (KEY_DOWN('4'))
				PostMessage(hwnd, WM_DESTROY, 0, 0);
		}
		//configure the rect for the source tile
		else if(stage == 1)
		{
			RECT srcRect;
			int columns = 2;

			//stop music
			StopSound(sound_music);

			position.x = (float)playerSquare.x;
			position.y = (float)playerSquare.y;

			srcRect.left = (playerSquare.curframe % columns) * playerSquare.width;
			srcRect.top = (playerSquare.curframe / columns) * playerSquare.height;
			srcRect.right = srcRect.left + playerSquare.width;
			srcRect.bottom = srcRect.top + playerSquare.height;

			sprite_handler->Draw(select_Image, &srcRect, NULL, &position,
				D3DCOLOR_XRGB(255,255,255));

			columns = 12;

			for(int n = 0; n < 8; n++)
			{
				position.x = (float)blackPawn[n].x;
				position.y = (float)blackPawn[n].y;

				srcRect.left = (blackPawn[n].curframe % columns) * blackPawn[n].width;
				srcRect.top = (blackPawn[n].curframe / columns) * blackPawn[n].height;
				srcRect.right = srcRect.left + blackPawn[n].width;
				srcRect.bottom = srcRect.top + blackPawn[n].height;

				if(blackPawn[n].alive == true)
					sprite_handler->Draw(bPiece_Image, &srcRect, NULL, &position, 
						D3DCOLOR_XRGB(255,255,255));
			}

			for(int n = 0; n < 2; n++)
			{
				position.x = (float)blackRook[n].x;
				position.y = (float)blackRook[n].y;

				srcRect.left = (blackRook[n].curframe % columns) * blackRook[n].width;
				srcRect.top = (blackRook[n].curframe / columns) * blackRook[n].height;
				srcRect.right = srcRect.left + blackRook[n].width;
				srcRect.bottom = srcRect.top + blackRook[n].height;

				if(blackRook[n].alive == true)
					sprite_handler->Draw(bPiece_Image, &srcRect, NULL, &position, 
						D3DCOLOR_XRGB(255,255,255));
			}

			for(int n = 0; n < 2; n++)
			{
				position.x = (float)blackKnight[n].x;
				position.y = (float)blackKnight[n].y;

				srcRect.left = (blackKnight[n].curframe % columns) * blackKnight[n].width;
				srcRect.top = (blackKnight[n].curframe / columns) * blackKnight[n].height;
				srcRect.right = srcRect.left + blackKnight[n].width;
				srcRect.bottom = srcRect.top + blackKnight[n].height;

				if(blackKnight[n].alive == true)
					sprite_handler->Draw(bPiece_Image, &srcRect, NULL, &position, 
						D3DCOLOR_XRGB(255,255,255));
			}

			for(int n = 0; n < 2; n++)
			{
				position.x = (float)blackBishop[n].x;
				position.y = (float)blackBishop[n].y;

				srcRect.left = (blackBishop[n].curframe % columns) * blackBishop[n].width;
				srcRect.top = (blackBishop[n].curframe / columns) * blackBishop[n].height;
				srcRect.right = srcRect.left + blackBishop[n].width;
				srcRect.bottom = srcRect.top + blackBishop[n].height;

				if(blackBishop[n].alive == true)
					sprite_handler->Draw(bPiece_Image, &srcRect, NULL, &position, 
						D3DCOLOR_XRGB(255,255,255));
			}

			position.x = (float)blackKing.x;
			position.y = (float)blackKing.y;

			srcRect.left = (blackKing.curframe % columns) * blackKing.width;
			srcRect.top = (blackKing.curframe / columns) * blackKing.height;
			srcRect.right = srcRect.left + blackKing.width;
			srcRect.bottom = srcRect.top + blackKing.height;

			if(blackKing.alive == true)
				sprite_handler->Draw(bPiece_Image, &srcRect, NULL, &position,
					D3DCOLOR_XRGB(255,255,255));

			position.x = (float)blackQueen.x;
			position.y = (float)blackQueen.y;

			srcRect.left = (blackQueen.curframe % columns) * blackQueen.width;
			srcRect.top = (blackQueen.curframe / columns) * blackQueen.height;
			srcRect.right = srcRect.left + blackQueen.width;
			srcRect.bottom = srcRect.top + blackQueen.height;

			if(blackQueen.alive == true)
				sprite_handler->Draw(bPiece_Image, &srcRect, NULL, &position,
					D3DCOLOR_XRGB(255,255,255));

			for(int n = 0; n < 8; n++)
			{
				position.x = (float)whitePawn[n].x;
				position.y = (float)whitePawn[n].y;

				srcRect.left = (whitePawn[n].curframe % columns) * whitePawn[n].width;
				srcRect.top = (whitePawn[n].curframe / columns) * whitePawn[n].height;
				srcRect.right = srcRect.left + whitePawn[n].width;
				srcRect.bottom = srcRect.top + whitePawn[n].height;

				if(whitePawn[n].alive == true)
					sprite_handler->Draw(wPiece_Image, &srcRect, NULL, &position, 
						D3DCOLOR_XRGB(255,255,255));
			}

			for(int n = 0; n < 2; n++)
			{
				position.x = (float)whiteRook[n].x;
				position.y = (float)whiteRook[n].y;

				srcRect.left = (whiteRook[n].curframe % columns) * whiteRook[n].width;
				srcRect.top = (whiteRook[n].curframe / columns) * whiteRook[n].height;
				srcRect.right = srcRect.left + whiteRook[n].width;
				srcRect.bottom = srcRect.top + whiteRook[n].height;

				if(whiteRook[n].alive == true)
					sprite_handler->Draw(wPiece_Image, &srcRect, NULL, &position, 
						D3DCOLOR_XRGB(255,255,255));
			}

			for(int n = 0; n < 2; n++)
			{
				position.x = (float)whiteKnight[n].x;
				position.y = (float)whiteKnight[n].y;

				srcRect.left = (whiteKnight[n].curframe % columns) * whiteKnight[n].width;
				srcRect.top = (whiteKnight[n].curframe / columns) * whiteKnight[n].height;
				srcRect.right = srcRect.left + whiteKnight[n].width;
				srcRect.bottom = srcRect.top + whiteKnight[n].height;

				if(whiteKnight[n].alive == true)
					sprite_handler->Draw(wPiece_Image, &srcRect, NULL, &position, 
						D3DCOLOR_XRGB(255,255,255));
			}

			for(int n = 0; n < 2; n++)
			{
				position.x = (float)whiteBishop[n].x;
				position.y = (float)whiteBishop[n].y;

				srcRect.left = (whiteBishop[n].curframe % columns) * whiteBishop[n].width;
				srcRect.top = (whiteBishop[n].curframe / columns) * whiteBishop[n].height;
				srcRect.right = srcRect.left + whiteBishop[n].width;
				srcRect.bottom = srcRect.top + whiteBishop[n].height;

				if(whiteBishop[n].alive == true)
					sprite_handler->Draw(wPiece_Image, &srcRect, NULL, &position, 
						D3DCOLOR_XRGB(255,255,255));
			}

			position.x = (float)whiteKing.x;
			position.y = (float)whiteKing.y;

			srcRect.left = (whiteKing.curframe % columns) * whiteKing.width;
			srcRect.top = (whiteKing.curframe / columns) * whiteKing.height;
			srcRect.right = srcRect.left + whiteKing.width;
			srcRect.bottom = srcRect.top + whiteKing.height;

			if(whiteKing.alive == true)
				sprite_handler->Draw(wPiece_Image, &srcRect, NULL, &position,
					D3DCOLOR_XRGB(255,255,255));

			position.x = (float)whiteQueen.x;
			position.y = (float)whiteQueen.y;

			srcRect.left = (whiteQueen.curframe % columns) * whiteQueen.width;
			srcRect.top = (whiteQueen.curframe / columns) * whiteQueen.height;
			srcRect.right = srcRect.left + whiteQueen.width;
			srcRect.bottom = srcRect.top + whiteQueen.height;

			if(whiteQueen.alive == true)
				sprite_handler->Draw(wPiece_Image, &srcRect, NULL, &position,
					D3DCOLOR_XRGB(255,255,255));
		}
		//stop drawing
		sprite_handler->End();

		//stop rendering
		d3ddev->EndScene();
	}

	//display the back buffer on the screen
	d3ddev->Present(NULL, NULL, NULL, NULL);

	//check for escape key (to return to menu)
	if(stage != 0)
	{
		if(KEY_DOWN(VK_ESCAPE))
		{
			stage = 0;
		}
	}
}

//frees memory and cleans up before the game ends
void Game_End(HWND hwnd)
{
	if (titleScreen != NULL)
		titleScreen->Release();

	if (gameScreen != NULL)
		gameScreen->Release();

	if (bPiece_Image != NULL)
		bPiece_Image->Release();

	if (wPiece_Image != NULL)
		wPiece_Image->Release();

	if (select_Image != NULL)
		select_Image->Release();

	if (tsPieceBl_Image != NULL)
		tsPieceBl_Image->Release();

	if (tsPieceWh_Image != NULL)
		tsPieceWh_Image->Release();

	if (sprite_handler != NULL)
		sprite_handler->Release();
}

void Board_Init()
{
	// These variables keep track of the coordinates of our current cell
	int a, b;
	a = -1;
	b = -1;

	for (int y=500; y >= 150; y-=50)
	{
		a++;
		for (int x=160; x <= 580; x+=60)
		{
			b++;
			CELL newCell;
			SPRITE newTile;
			newTile.x = x;
			newTile.y = y;
			newTile.height = 50;
			newTile.width = 60;

			newCell.cellSprite = newTile;
			newCell.isActive = false;
			newCell.isOccupied = false;
			board.cell[a][b] = newCell;
		}
		b=-1;
	}
}

void toggleBoard()
{
		switch(activeType)
		{
		case PAWN:
			movePawn();
			break;
		case ROOK:
			moveRook();
			break;
		case KNIGHT:
			moveKnight();
			break;
		case BISHOP:
			moveBishop();
			break;
		case KING:
			moveKing();
			break;
		case QUEEN:
			moveQueen();
			break;
		default:
			break;
		}
}

void movePawn()
{
	if(playerActive->team == BLACK)
	{
		if(activePiece == true)
		{
			if(board.cell[(tempY + 1)][tempX].isOccupied == false)
			{
				board.cell[(tempY + 1)][tempX].isActive = true;
					board.cell[(tempY + 2)][tempX].isActive = true;
			}

			if(board.cell[(tempY + 1)][(tempX - 1)].isTeam == WHITE)
				board.cell[(tempY + 1)][(tempX - 1)].isActive = true;

			if(board.cell[(tempY + 1)][(tempX + 1)].isTeam == WHITE)
				board.cell[(tempY + 1)][(tempX + 1)].isActive = true;
		}
		else
		{
			board.cell[(tempY + 1)][tempX].isActive = false;
			if(tempY == 1)
				board.cell[(tempY + 2)][tempX].isActive = false;

			if(board.cell[(tempY + 1)][(tempX - 1)].isTeam == WHITE)
				board.cell[(tempY + 1)][(tempX - 1)].isActive = false;

			if(board.cell[(tempY + 1)][(tempX + 1)].isTeam == WHITE)
				board.cell[(tempY + 1)][(tempX + 1)].isActive = false;
		}
	}
	else if(playerActive->team == WHITE)
	{
		if(activePiece == true)
		{
			if(board.cell[(tempY - 1)][tempX].isOccupied == false)
			{
				board.cell[(tempY - 1)][tempX].isActive = true;
				if(tempY == 6)
					board.cell[(tempY - 2)][tempX].isActive = true;
			}

			if(board.cell[(tempY - 1)][(tempX - 1)].isTeam == BLACK)
				board.cell[(tempY - 1)][(tempX - 1)].isActive = true;

			if(board.cell[(tempY - 1)][(tempX + 1)].isTeam == BLACK)
				board.cell[(tempY - 1)][(tempX + 1)].isActive = true;
		}
		else
		{
			board.cell[(tempY - 1)][tempX].isActive = false;
			if(tempY == 6)
				board.cell[(tempY - 2)][tempX].isActive = false;

			if(board.cell[(tempY - 1)][(tempX - 1)].isTeam == BLACK)
				board.cell[(tempY - 1)][(tempX - 1)].isActive = false;

			if(board.cell[(tempY - 1)][(tempX + 1)].isTeam == BLACK)
				board.cell[(tempY - 1)][(tempX + 1)].isActive = false;
		}
	}
}

void moveRook()
{
	int blockY, blockX;

	blockY = 8;
	for(int nY = tempY; nY < blockY; nY++)
	{
		if(board.cell[nY][tempX].isOccupied == true)
			blockY = nY;
		else
		{
			if(activePiece == true)
				board.cell[nY][tempX].isActive = true;
			else
				board.cell[nY][tempX].isActive = false;
		}
	}
	blockY = -1;
	for(int nY = tempY; nY > blockY; nY--)
	{
		if(board.cell[nY][tempX].isOccupied == true)
			blockY = nY;
		else
		{
			if(activePiece == true)
				board.cell[nY][tempX].isActive = true;
			else
				board.cell[nY][tempX].isActive = false;
		}
	}

	blockX = 8;
	for(int nX = tempX; nX < blockX; nX++)
	{
		if(board.cell[tempY][nX].isOccupied == true)
			blockX = nX;
		else
		{
			if(activePiece == true)
				board.cell[tempY][nX].isActive = true;
			else
				board.cell[tempY][nX].isActive = false;
		}
	}
	blockX = -1;
	for(int nX = tempX; nX > blockX; nX--)
	{
		if(board.cell[tempY][nX].isOccupied == true)
			blockX = nX;
		else
		{
			if(activePiece == true)
				board.cell[tempY][nX].isActive = true;
			else
				board.cell[tempY][nX].isActive = false;
		}
	}
}

void moveKnight()
{
	if(activePiece == true)
	{
		if(tempY > 1 && tempY < 6)
		{
			if(tempX > 1 && tempX < 6)
			{
				board.cell[(tempY + 2)][(tempX + 1)].isActive = true;
				board.cell[(tempY + 2)][(tempX - 1)].isActive = true;
				board.cell[(tempY - 2)][(tempX + 1)].isActive = true;
				board.cell[(tempY - 2)][(tempX - 1)].isActive = true;

				board.cell[(tempY + 1)][(tempX + 2)].isActive = true;
				board.cell[(tempY - 1)][(tempX + 2)].isActive = true;
				board.cell[(tempY + 1)][(tempX - 2)].isActive = true;
				board.cell[(tempY - 1)][(tempX - 2)].isActive = true;
			}
			else if(tempX < 2)
			{
				board.cell[(tempY + 2)][(tempX + 1)].isActive = true;
				board.cell[(tempY - 2)][(tempX + 1)].isActive = true;

				board.cell[(tempY + 1)][(tempX + 2)].isActive = true;
				board.cell[(tempY - 1)][(tempX + 2)].isActive = true;
				if(tempX == 1)
				{
					board.cell[(tempY + 2)][(tempX - 1)].isActive = true;
					board.cell[(tempY - 2)][(tempX - 1)].isActive = true;
				}
			}
			else if(tempX > 5)
			{
				board.cell[(tempY + 2)][(tempX - 1)].isActive = true;
				board.cell[(tempY - 2)][(tempX - 1)].isActive = true;

				board.cell[(tempY + 1)][(tempX - 2)].isActive = true;
				board.cell[(tempY - 1)][(tempX - 2)].isActive = true;
				if(tempX == 6)
				{
					board.cell[(tempY + 2)][(tempX + 1)].isActive = true;
					board.cell[(tempY - 2)][(tempX + 1)].isActive = true;
				}
			}
		}
		else if(tempY < 2)
		{
			if(tempX > 1 && tempX < 6)
			{
				board.cell[(tempY + 2)][(tempX + 1)].isActive = true;
				board.cell[(tempY + 2)][(tempX - 1)].isActive = true;

				board.cell[(tempY + 1)][(tempX + 2)].isActive = true;
				board.cell[(tempY + 1)][(tempX - 2)].isActive = true;
			}
			else if(tempX < 2)
			{
				board.cell[(tempY + 2)][(tempX + 1)].isActive = true;

				board.cell[(tempY + 1)][(tempX + 2)].isActive = true;
				if(tempX == 1)
					board.cell[(tempY + 2)][(tempX - 1)].isActive = true;
			}
			else if(tempX > 5)
			{
				board.cell[(tempY + 2)][(tempX - 1)].isActive = true;

				board.cell[(tempY + 1)][(tempX - 2)].isActive = true;
				if(tempX == 6)
					board.cell[(tempY + 2)][(tempX + 1)].isActive = true;
			}

			if(tempY == 1)
			{
				if(tempX > 1 && tempX < 6)
				{
					board.cell[(tempY - 1)][(tempX + 2)].isActive = true;
					board.cell[(tempY - 1)][(tempX - 2)].isActive = true;
				}
				else if(tempX < 2)
				{
					board.cell[(tempY - 1)][(tempX + 2)].isActive = true;
					if(tempX == 1)
						board.cell[(tempY + 2)][(tempX - 1)].isActive = true;
				}
				else if(tempX > 5)
				{
					board.cell[(tempY - 1)][(tempX - 2)].isActive = true;
					if(tempX == 6)
						board.cell[(tempY + 2)][(tempX + 1)].isActive = true;
				}
			}
		}
		else if(tempY > 5)
		{
			if(tempX > 1 && tempX < 6)
			{
				board.cell[(tempY - 2)][(tempX + 1)].isActive = true;
				board.cell[(tempY - 2)][(tempX - 1)].isActive = true;

				board.cell[(tempY - 1)][(tempX + 2)].isActive = true;
				board.cell[(tempY - 1)][(tempX - 2)].isActive = true;
			}
			else if(tempX < 2)
			{
				board.cell[(tempY - 2)][(tempX + 1)].isActive = true;

				board.cell[(tempY - 1)][(tempX + 2)].isActive = true;
				if(tempX == 1)
					board.cell[(tempY - 2)][(tempX - 1)].isActive = true;
			}
			else if(tempX > 5)
			{
				board.cell[(tempY - 2)][(tempX - 1)].isActive = true;

				board.cell[(tempY - 1)][(tempX - 2)].isActive = true;
				if(tempX == 6)
					board.cell[(tempY - 2)][(tempX + 1)].isActive = true;
			}

			if(tempY == 6)
			{
				if(tempX > 1 && tempX < 6)
				{
					board.cell[(tempY + 1)][(tempX + 2)].isActive = true;
					board.cell[(tempY + 1)][(tempX - 2)].isActive = true;
				}
				else if(tempX < 2)
				{
					board.cell[(tempY + 1)][(tempX + 2)].isActive = true;
					if(tempX == 1)
						board.cell[(tempY - 2)][(tempX - 1)].isActive = true;
				}
				else if(tempX > 5)
				{
					board.cell[(tempY - 1)][(tempX - 2)].isActive = true;
					if(tempX == 6)
						board.cell[(tempY - 2)][(tempX + 1)].isActive = true;
				}
			}
		}
	}
	else
	{
		if(tempY > 1 && tempY < 6)
		{
			if(tempX > 1 && tempX < 6)
			{
				board.cell[(tempY + 2)][(tempX + 1)].isActive = false;
				board.cell[(tempY + 2)][(tempX - 1)].isActive = false;
				board.cell[(tempY - 2)][(tempX + 1)].isActive = false;
				board.cell[(tempY - 2)][(tempX - 1)].isActive = false;

				board.cell[(tempY + 1)][(tempX + 2)].isActive = false;
				board.cell[(tempY - 1)][(tempX + 2)].isActive = false;
				board.cell[(tempY + 1)][(tempX - 2)].isActive = false;
				board.cell[(tempY - 1)][(tempX - 2)].isActive = false;
			}
			else if(tempX < 2)
			{
				board.cell[(tempY + 2)][(tempX + 1)].isActive = false;
				board.cell[(tempY - 2)][(tempX + 1)].isActive = false;

				board.cell[(tempY + 1)][(tempX + 2)].isActive = false;
				board.cell[(tempY - 1)][(tempX + 2)].isActive = false;
				if(tempX == 1)
				{
					board.cell[(tempY + 2)][(tempX - 1)].isActive = false;
					board.cell[(tempY - 2)][(tempX - 1)].isActive = false;
				}
			}
			else if(tempX > 5)
			{
				board.cell[(tempY + 2)][(tempX - 1)].isActive = false;
				board.cell[(tempY - 2)][(tempX - 1)].isActive = false;

				board.cell[(tempY + 1)][(tempX - 2)].isActive = false;
				board.cell[(tempY - 1)][(tempX - 2)].isActive = false;
				if(tempX == 6)
				{
					board.cell[(tempY + 2)][(tempX + 1)].isActive = false;
					board.cell[(tempY - 2)][(tempX + 1)].isActive = false;
				}
			}
		}
		else if(tempY < 2)
		{
			if(tempX > 1 && tempX < 6)
			{
				board.cell[(tempY + 2)][(tempX + 1)].isActive = false;
				board.cell[(tempY + 2)][(tempX - 1)].isActive = false;

				board.cell[(tempY + 1)][(tempX + 2)].isActive = false;
				board.cell[(tempY + 1)][(tempX - 2)].isActive = false;
			}
			else if(tempX < 2)
			{
				board.cell[(tempY + 2)][(tempX + 1)].isActive = false;

				board.cell[(tempY + 1)][(tempX + 2)].isActive = false;
				if(tempX == 1)
					board.cell[(tempY + 2)][(tempX - 1)].isActive = false;
			}
			else if(tempX > 5)
			{
				board.cell[(tempY + 2)][(tempX - 1)].isActive = false;

				board.cell[(tempY + 1)][(tempX - 2)].isActive = false;
				if(tempX == 6)
					board.cell[(tempY + 2)][(tempX + 1)].isActive = false;
			}

			if(tempY == 1)
			{
				if(tempX > 1 && tempX < 6)
				{
					board.cell[(tempY - 1)][(tempX + 2)].isActive = false;
					board.cell[(tempY - 1)][(tempX - 2)].isActive = false;
				}
				else if(tempX < 2)
				{
					board.cell[(tempY - 1)][(tempX + 2)].isActive = false;
					if(tempX == 1)
						board.cell[(tempY + 2)][(tempX - 1)].isActive = false;
				}
				else if(tempX > 5)
				{
					board.cell[(tempY - 1)][(tempX - 2)].isActive = false;
					if(tempX == 6)
						board.cell[(tempY + 2)][(tempX + 1)].isActive = false;
				}
			}
		}
		else if(tempY > 5)
		{
			if(tempX > 1 && tempX < 6)
			{
				board.cell[(tempY - 2)][(tempX + 1)].isActive = false;
				board.cell[(tempY - 2)][(tempX - 1)].isActive = false;

				board.cell[(tempY - 1)][(tempX + 2)].isActive = false;
				board.cell[(tempY - 1)][(tempX - 2)].isActive = false;
			}
			else if(tempX < 2)
			{
				board.cell[(tempY - 2)][(tempX + 1)].isActive = false;

				board.cell[(tempY - 1)][(tempX + 2)].isActive = false;
				if(tempX == 1)
					board.cell[(tempY - 2)][(tempX - 1)].isActive = false;
			}
			else if(tempX > 5)
			{
				board.cell[(tempY - 2)][(tempX - 1)].isActive = false;

				board.cell[(tempY - 1)][(tempX - 2)].isActive = false;
				if(tempX == 6)
					board.cell[(tempY - 2)][(tempX + 1)].isActive = false;
			}

			if(tempY == 6)
			{
				if(tempX > 1 && tempX < 6)
				{
					board.cell[(tempY + 1)][(tempX + 2)].isActive = false;
					board.cell[(tempY + 1)][(tempX - 2)].isActive = false;
				}
				else if(tempX < 2)
				{
					board.cell[(tempY + 1)][(tempX + 2)].isActive = false;
					if(tempX == 1)
						board.cell[(tempY - 2)][(tempX - 1)].isActive = false;
				}
				else if(tempX > 5)
				{
					board.cell[(tempY - 1)][(tempX - 2)].isActive = false;
					if(tempX == 6)
						board.cell[(tempY - 2)][(tempX + 1)].isActive = false;
				}
			}
		}
	}
}

void moveBishop()
{
	int nY, nX;
	bool block = false;
	nY = tempY;
	nX = tempX;

	while(nY < 7 && nX < 7 && block == false)
	{
		nY++;
		nX++;

		if(board.cell[nY][nX].isOccupied == false)
		{
			if(activePiece == true)
				board.cell[nY][nX].isActive = true;
			else
				board.cell[nY][nX].isActive = false;
		}
		else
			block = true;
	}

	block = false;
	nY = tempY;
	nX = tempX;
	while(nY < 7 && nX > 0 && block == false)
	{
		nY++;
		nX--;

		if(board.cell[nY][nX].isOccupied == false)
		{
			if(activePiece == true)
				board.cell[nY][nX].isActive = true;
			else
				board.cell[nY][nX].isActive = false;
		}
		else
			block = true;
	}

	block = false;
	nY = tempY;
	nX = tempX;
	while(nY > 0 && nX < 7 && block == false)
	{
		nY--;
		nX++;

		if(board.cell[nY][nX].isOccupied == false)
		{
			if(activePiece == true)
				board.cell[nY][nX].isActive = true;
			else
				board.cell[nY][nX].isActive = false;
		}
		else
			block = true;
	}

	block = false;
	nY = tempY;
	nX = tempX;
	while(nY > 0 && nX > 0 && block == false)
	{
		nY--;
		nX--;

		if(board.cell[nY][nX].isOccupied == false)
		{
			if(activePiece == true)
				board.cell[nY][nX].isActive = true;
			else
				board.cell[nY][nX].isActive = false;
		}
		else
			block = true;
	}
}

void moveKing()
{
	if(activePiece == true)
	{
		if(tempY > 0)
		{
			board.cell[(tempY - 1)][tempX].isActive = true;
			if(tempX > 0)
			{
				board.cell[tempY][(tempX - 1)].isActive = true;
				board.cell[(tempY -1)][(tempX - 1)].isActive = true;
			}

			if(tempX < 7)
			{
				board.cell[tempY][(tempX + 1)].isActive = true;
				board.cell[(tempY - 1)][(tempX + 1)].isActive = true;
			}
		}

		if(tempY < 7)
		{
			board.cell[(tempY + 1)][tempX].isActive = true;
			if(tempX > 0)
			{
				board.cell[tempY][(tempX - 1)].isActive = true;
				board.cell[(tempY + 1)][(tempX - 1)].isActive = true;
			}

			if(tempX < 7)
			{
				board.cell[tempY][(tempX + 1)].isActive = true;
				board.cell[(tempY + 1)][(tempX + 1)].isActive = true;
			}
		}
	}
	else
	{
		if(tempY > 0)
		{
			board.cell[(tempY - 1)][tempX].isActive = false;
			if(tempX > 0)
			{
				board.cell[tempY][(tempX - 1)].isActive = false;
				board.cell[(tempY -1)][(tempX - 1)].isActive = false;
			}

			if(tempX < 7)
			{
				board.cell[tempY][(tempX + 1)].isActive = false;
				board.cell[(tempY - 1)][(tempX + 1)].isActive = false;
			}
		}

		if(tempY < 7)
		{
			board.cell[(tempY + 1)][tempX].isActive = false;
			if(tempX > 0)
			{
				board.cell[tempY][(tempX - 1)].isActive = false;
				board.cell[(tempY + 1)][(tempX - 1)].isActive = false;
			}
			if(tempX < 7)
			{
				board.cell[tempY][(tempX + 1)].isActive = false;
				board.cell[(tempY + 1)][(tempX + 1)].isActive = false;
			}
		}
	}
}

void moveQueen()
{
	moveRook();
	moveBishop();
}
