#include "Logic.h" 
#include "Wall.h"
#include "Tank.h"
#include "Shell.h"
#include <time.h>


#define NIL 255
#define colision 25

typedef struct _GRID_MOVE {
	unsigned char cell[4];
	short int x;
	short int y;
}GRID_MOVE;

GRID_MOVE gridMove[19];
char game = 1;


void Shot(unsigned char numb) {
	if (0==tank[numb].timeToReload && 0!=tank[numb].hp) {
		tank[numb].timeToReload = tank[numb].reload;
		SHELL* now = NULL;
		while (NULL == now) {
			now = (SHELL*)(malloc(sizeof(SHELL)));
		}
		now->next = NULL;
		now->damage = tank[numb].damage;
		switch (tank[numb].orient)
		{
		case UP:
			now->orient = tank[numb].orient;
			now->x = tank[numb].x + baseTank.orient[UP][3].left;
			now->y = tank[numb].y + baseTank.orient[UP][3].top - SHELL_SIZE;
			break;
		case RIGHT:
			now->orient = tank[numb].orient;
			now->x = tank[numb].x + baseTank.orient[RIGHT][3].right;
			now->y = tank[numb].y + baseTank.orient[RIGHT][3].top;
			break;
		case DOWN:
			now->orient = tank[numb].orient;
			now->x = tank[numb].x + baseTank.orient[DOWN][3].left;
			now->y = tank[numb].y + baseTank.orient[DOWN][3].bottom;
			break;
		case LEFT:
			now->orient = tank[numb].orient;
			now->x = tank[numb].x + baseTank.orient[LEFT][3].left - SHELL_SIZE;
			now->y = now->y = tank[numb].y + baseTank.orient[LEFT][3].top;
			break;
		}
		finShell->next = now;
		finShell = now;
	}
}

void MoveAndCheckingCollisionsTank(unsigned char orient, unsigned char numb) {
	if (0 != tank[numb].hp) {
		char flag = 1;
		int i;
		switch (orient)
		{
		case UP:
			if ((tank[numb].y - tank[numb].speed) > ZONE_TOP - 1) {
				for (i = 0; i < numbWall; i++) {
					if ((wall[i].y + WALL_TOP < tank[numb].y + TANK_TOP + tank[numb].speed) && (tank[numb].y + TANK_TOP < wall[i].y + WALL_BOTTOM + tank[numb].speed)) {
						if (((wall[i].x + WALL_LEFT < tank[numb].x + TANK_LEFT) && (tank[numb].x + TANK_LEFT < wall[i].x + WALL_RIGHT)) || ((wall[i].x + WALL_LEFT < tank[numb].x + TANK_RIGHT) && (tank[numb].x + TANK_RIGHT < wall[i].x + WALL_RIGHT))) {
							flag = 0;
						}
					}
				}
				if (flag) {
					MoveTank(UP, numb);
				}
			}
			break;
		case RIGHT:
			if ((tank[numb].x + tank[numb].speed) < ZONE_RIGHT + 1) {
				for (i = 0; i < numbWall; i++) {
					if ((wall[i].x + WALL_LEFT < tank[numb].x + TANK_RIGHT + tank[numb].speed + 2) && (tank[numb].x + TANK_RIGHT < wall[i].x + WALL_RIGHT + tank[numb].speed + 2)) {
						if (((wall[i].y + WALL_TOP < tank[numb].y + TANK_TOP) && (tank[numb].y + TANK_TOP < wall[i].y + WALL_BOTTOM)) || ((wall[i].y + WALL_TOP < tank[numb].y + TANK_BOTTOM) && (tank[numb].y + TANK_BOTTOM < wall[i].y + WALL_BOTTOM))) {
							flag = 0;
						}
					}
				}
				if (flag) {
					MoveTank(RIGHT, numb);
				}
			}
			break;
		case DOWN:
			if ((tank[numb].y + tank[numb].speed) < ZONE_BOTTOM + 1) {
				for (i = 0; i < numbWall; i++) {
					if ((wall[i].y + WALL_TOP < tank[numb].y + TANK_BOTTOM + tank[numb].speed + 2) && (tank[numb].y + TANK_BOTTOM < wall[i].y + WALL_BOTTOM + tank[numb].speed + 2)) {
						if (((wall[i].x + WALL_LEFT < tank[numb].x + TANK_LEFT) && (tank[numb].x + TANK_LEFT < wall[i].x + WALL_RIGHT)) || ((wall[i].x + WALL_LEFT < tank[numb].x + TANK_RIGHT) && (tank[numb].x + TANK_RIGHT < wall[i].x + WALL_RIGHT))) {
							flag = 0;
						}
					}
				}
				if (flag) {
					MoveTank(DOWN, numb);
				}
			}
			break;
		case LEFT:
			if ((tank[numb].x - tank[numb].speed) > ZONE_LEFT - 1) {
				for (i = 0; i < numbWall; i++) {
					if ((wall[i].x + WALL_LEFT < tank[numb].x + TANK_LEFT + tank[numb].speed) && (tank[numb].x + TANK_LEFT < wall[i].x + WALL_RIGHT + tank[numb].speed)) {
						if (((wall[i].y + WALL_TOP < tank[numb].y + TANK_TOP) && (tank[numb].y + TANK_TOP < wall[i].y + WALL_BOTTOM)) || ((wall[i].y + WALL_TOP < tank[numb].y + TANK_BOTTOM) && (tank[numb].y + TANK_BOTTOM < wall[i].y + WALL_BOTTOM))) {
							flag = 0;
						}
					}
				}
				if (flag) {
					MoveTank(LEFT, numb);
				}
			}
			break;
		}
	}
}

void CheckingCollisionsShell() {
	unsigned char flag, kill;
	int i;
	SHELL* next, * pred = headShell;
	for (SHELL* now = headShell->next; NULL != now; ) {
		flag = 1;
		kill = 255;
		switch (now->orient)
		{
		case UP:
			if (now->y > ZONE_TOP - 20) {
				for (i = 0; i < numbWall; i++) {
					if ((wall[i].y + WALL_TOP < now->y) && (now->y < wall[i].y + WALL_BOTTOM)) {
						if (((wall[i].x + WALL_LEFT < now->x) && (now->x < wall[i].x + WALL_RIGHT)) || ((wall[i].x + WALL_LEFT < now->x + SHELL_SIZE) && (now->x + SHELL_SIZE < wall[i].x + WALL_RIGHT))) {
							flag = 0;
						}
					}
				}
				for (i = 0; i < numbTank; i++) {
					if ((tank[i].y + baseTank.orient[UP][0].top < now->y) && (now->y < tank[i].y + baseTank.orient[UP][1].bottom)) {
						if (((tank[i].x + baseTank.orient[UP][0].left < now->x) && (now->x < tank[i].x + baseTank.orient[UP][1].right)) || ((tank[i].x + baseTank.orient[UP][0].left < now->x + SHELL_SIZE) && (now->x + SHELL_SIZE < tank[i].x + baseTank.orient[UP][1].right))) {
							flag = 0;
							kill = i;
						}
					}
				}
			}
			else {
				flag = 0;
			}
			break;

		case RIGHT:
			if (now->x < ZONE_RIGHT + 80 - SHELL_SIZE) {
				for (i = 0; i < numbWall; i++) {
					if ((wall[i].x + WALL_LEFT < now->x+SHELL_SIZE) && (now->x+SHELL_SIZE < wall[i].x + WALL_RIGHT)) {
						if (((wall[i].y + WALL_TOP < now->y) && (now->y < wall[i].y + WALL_BOTTOM)) || ((wall[i].y + WALL_TOP < now->y + SHELL_SIZE) && (now->y + SHELL_SIZE < wall[i].y + WALL_BOTTOM))) {
							flag = 0;
						}
					}
				}
				for (i = 0; i < numbTank; i++) {
					if ((tank[i].x + baseTank.orient[UP][0].left < now->x + SHELL_SIZE) && (now->x + SHELL_SIZE < tank[i].x + baseTank.orient[UP][1].right)) {
						if (((tank[i].y + baseTank.orient[UP][0].top < now->y) && (now->y < tank[i].y + baseTank.orient[UP][1].bottom)) || ((tank[i].y + baseTank.orient[UP][0].top < now->y + SHELL_SIZE) && (now->y + SHELL_SIZE < tank[i].y + baseTank.orient[UP][1].bottom))) {
							flag = 0;
							kill = i;
						}
					}
				}
			}
			else {
				flag = 0;
			}
			break;

		case DOWN:
			if (now->y < ZONE_BOTTOM + 80 - SHELL_SIZE) {
				for (i = 0; i < numbWall; i++) {
					if ((wall[i].y + WALL_TOP < now->y + SHELL_SIZE)&& (now->y + SHELL_SIZE < wall[i].y + WALL_BOTTOM)) {
						if (((wall[i].x + WALL_LEFT < now->x) && (now->x < wall[i].x + WALL_RIGHT)) || ((wall[i].x + WALL_LEFT < now->x+SHELL_SIZE) && (now->x + SHELL_SIZE < wall[i].x + WALL_RIGHT))) {
							flag = 0;
						}
					}
				}
				for (i = 0; i < numbTank; i++) {
					if ((tank[i].y + baseTank.orient[UP][0].top < now->y + SHELL_SIZE) && (now->y + SHELL_SIZE < tank[i].y + baseTank.orient[UP][1].bottom)) {
						if (((tank[i].x + baseTank.orient[UP][0].left < now->x) && (now->x < tank[i].x + baseTank.orient[UP][1].right)) || ((tank[i].x + baseTank.orient[UP][0].left < now->x + SHELL_SIZE) && (now->x + SHELL_SIZE < tank[i].x + baseTank.orient[UP][1].right))) {
							flag = 0;
							kill = i;
						}
					}
				}
			}
			else {
				flag = 0;
			}
			break;

		case LEFT:
			if (now->x > ZONE_LEFT - 20) {
				for (i = 0; i < numbWall; i++) {
					if ((wall[i].x + WALL_LEFT < now->x) && (now->x < wall[i].x + WALL_RIGHT)) {
						if (((wall[i].y + WALL_TOP < now->y) && (now->y < wall[i].y + WALL_BOTTOM)) || ((wall[i].y + WALL_TOP < now->y+SHELL_SIZE) && (now->y + SHELL_SIZE < wall[i].y + WALL_BOTTOM))) {
							flag = 0;
						}
					}
				}
				for (i = 0; i < numbTank; i++) {
					if ((tank[i].x + baseTank.orient[UP][0].left < now->x) && (now->x < tank[i].x + baseTank.orient[UP][1].right)) {
						if (((tank[i].y + baseTank.orient[UP][0].top < now->y) && (now->y < tank[i].y + baseTank.orient[UP][1].bottom)) || ((tank[i].y + baseTank.orient[UP][0].top < now->y + SHELL_SIZE) && (now->y + SHELL_SIZE < tank[i].y + baseTank.orient[UP][1].bottom))) {
							flag = 0;
							kill = i;
						}
					}
				}
			}
			else {
				flag = 0;
			}
			break;
		}

		if (flag) {
			MoveShell(now);
		}
		else {
			next = now->next;
			DeleteShell(now, pred);
			now = next;
			if (255!=kill) {
				tank[kill].hp = 0;
				tank[kill].x = 0;
				tank[kill].y = 0;
			}
		}

		pred = now;
		if (NULL != now) {
			now = now->next;
		}
	}
}

void AddElemGridMove(unsigned char numb, unsigned char numb0, unsigned char numb1, unsigned char numb2, unsigned char numb3, short int x, short int y) {
	gridMove[numb].cell[0] = numb0;
	gridMove[numb].cell[1] = numb1;
	gridMove[numb].cell[2] = numb2;
	gridMove[numb].cell[3] = numb3;
	gridMove[numb].x = x;
	gridMove[numb].y = y;
}

void InitializationAI() {
	AddElemGridMove(0 , NIL, 1  , 2  , NIL, 370 , 10 );
	AddElemGridMove(1 , NIL, NIL, 6  , 0  , 1180, 10);
	AddElemGridMove(2 , 0  , 3  , 7  , NIL, 370 , 120);
	AddElemGridMove(3 , NIL, 4  , 8  , 2  , 540 , 120);
	AddElemGridMove(4 , NIL, 5  , 9  , 3  , 770 , 120);
	AddElemGridMove(5 , NIL, 6  , 10 , 4  , 1010, 120);
	AddElemGridMove(6 , 1  , NIL, 11 , 5  , 1180, 120);
	AddElemGridMove(7 , 2  , 8  , 12 , NIL, 370 , 400);
	AddElemGridMove(8 , 3  , 9  , 13 , 7  , 540 , 400);
	AddElemGridMove(9 , 4  , 10 , 14 , 8  , 770 , 400);
	AddElemGridMove(10, 5  , 11 , 15 , 9  , 1010, 400);
	AddElemGridMove(11, 6  , NIL, 16 , 10 , 1180, 400);
	AddElemGridMove(12, 7  , 13 , 17 , NIL, 370 , 700);
	AddElemGridMove(13, 8  , 14 , NIL, 12 , 540 , 700);
	AddElemGridMove(14, 9  , 15 , NIL, 13 , 770 , 700);
	AddElemGridMove(15, 10 , 16 , NIL, 14 , 1010, 700);
	AddElemGridMove(16, 11 , NIL, 18 , 15 , 1180, 700);
	AddElemGridMove(17, 12 , 18 , NIL, NIL, 370 , 840);
	AddElemGridMove(18, 16 , NIL, NIL, 17 , 1180, 840);
}
unsigned char m[100], j = 0;

void WayAI() {
	unsigned char cell;
	for (int i = 1; i < numbTank; i++) {
		if (0 < tank[i].hp && 0 == tank[i].move) {
			cell = NIL;
			while (NIL == cell) {
				cell = gridMove[tank[i].cell].cell[rand() % 4];
			}
			tank[i].move = 1;
			tank[i].cell = cell;
		}
	}
}

void MoveAI() {
	for (int i = 1; i < numbTank; i++) {
		if (tank[i].y != gridMove[tank[i].cell].y && tank[i].x != gridMove[tank[i].cell].x) {
			if (tank[i].y > gridMove[tank[i].cell].y) {
				MoveAndCheckingCollisionsTank(UP, i);
			}
			else {
				if (tank[i].x < gridMove[tank[i].cell].x) {
					MoveAndCheckingCollisionsTank(RIGHT, i);
				}
				else {
					if (tank[i].y < gridMove[tank[i].cell].y) {
						MoveAndCheckingCollisionsTank(DOWN, i);
					}
					else {
						if (tank[i].x > gridMove[tank[i].cell].x) {
							MoveAndCheckingCollisionsTank(LEFT, i);
						}
					}
				}
			}
		}
		else {
			tank[i].move = 0;
		}
		if (tank[i].x - colision < tank[PLAYER].x && tank[PLAYER].x < tank[i].x + colision) {
			if (tank[i].y > tank[PLAYER].y) {
				tank[i].orient = UP;
			}
			else {
				tank[i].orient = DOWN;
			}
			Shot(i);
		}
		else {
			if (tank[i].y - colision < tank[PLAYER].y && tank[PLAYER].y < tank[i].y + colision) {
				if (tank[i].x > tank[PLAYER].x) {
					tank[i].orient = LEFT;
				}
				else {
					tank[i].orient = RIGHT;
				}
				Shot(i);
			}
		}
	}
}

void AI() {
	srand(time(NULL));
	WayAI();
	MoveAI();

}

void CheckGame() {
	game = 0;
	for (int i = 1; i < numbTank; i++) {
		if (0 != tank[i].hp) {
			game = 1;
		}
	}
}