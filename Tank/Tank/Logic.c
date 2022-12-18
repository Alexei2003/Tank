#include "Logic.h" 
#include "Wall.h"
#include "Tank.h"
#include "Shell.h"

void Shot(unsigned char numb) {
	if (0==tank[numb].timeToReload) {
		tank[numb].timeToReload = tank[numb].reload;
		SHELL* now = (SHELL*)(malloc(sizeof(SHELL)));
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

void CheckingCollisionsShell() {
	char flag;
	int i;
	SHELL* next, * pred = headShell;
	for (SHELL* now = headShell->next; NULL != now; pred = now) {
		flag = 1;
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
			}
			else {
				flag = 0;
			}

			if (flag) {
				MoveShell(now);
			}
			else {
				next = now->next;
				DeleteShell(now, pred);
				now = next;
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
			}
			else {
				flag = 0;
			}

			if (flag) {
				MoveShell(now);
			}
			else {
				next = now->next;
				DeleteShell(now, pred);
				now = next;
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
			}
			else {
				flag = 0;
			}

			if (flag) {
				MoveShell(now);
			}
			else {
				next = now->next;
				DeleteShell(now, pred);
				now = next;
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
			}
			else {
				flag = 0;
			}

			if (flag) {
				MoveShell(now);
			}
			else {
				next = now->next;
				DeleteShell(now, pred);
				now = next;
			}
			break;
		}
		if (NULL != now) {
			now = now->next;
		}
	}
}
