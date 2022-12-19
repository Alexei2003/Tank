#pragma once

#include <Windows.h>

#define WALL_TOP 4
#define WALL_RIGHT 56
#define	WALL_BOTTOM 56
#define WALL_LEFT 4


typedef struct _WALL {
	short int  x;
	short int y;
}WALL;

extern RECT base_wall[2];
extern unsigned char numbWall;
extern WALL wall[68];

void InitializationWall();
void FinalizeWall();
void DrawWall(HDC hdc);