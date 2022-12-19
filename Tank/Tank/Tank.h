#pragma once

#include <Windows.h>

#define UP 0
#define RIGHT 1
#define	DOWN 2
#define LEFT 3

#define TANK_TOP 5
#define TANK_BOTTOM 53
#define TANK_RIGHT 54
#define TANK_LEFT 5

#define SIZE_TANK 60
#define ZONE_TOP 0
#define ZONE_BOTTOM 840
#define ZONE_RIGHT 1190
#define	ZONE_LEFT 350

#define PLAYER 0

typedef struct _BASE_TANK {
	RECT orient[4][6];
}BASE_TANK;

typedef struct _TANK {
	short int  x;
	short int y;

	unsigned char orient;
	unsigned char colour;
	unsigned char timeToReload;
	unsigned char cell;
	unsigned char move;


	unsigned char hp;
	unsigned char speed;
	unsigned char damage;
	unsigned char reload;
}TANK;

extern BASE_TANK baseTank;
extern TANK tank[5];
extern unsigned char numbTank;

void InitializationTank(unsigned char numb);
void FinalizeTank();
void DrawTank(HDC hdc);
void DrawAllTank(HDC hdc);
void MoveTank(unsigned char orient, unsigned char numb);
void TankReload();
