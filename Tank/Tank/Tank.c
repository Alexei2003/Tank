#include "Tank.h"

#define ENEMY 1
#define BASE_SPEED 5
#define BASE_DAMAGE 1
#define BASE_HP 1
#define BASE_RELOAD 2

BASE_TANK baseTank;
TANK tank[5];
unsigned char numbTank;

HBRUSH   hbrPlayer, hbrPalayerLight, hbrEnemy, hbrEnemyLight;
HPEN	 hpCross, hpTank;

void InitializationTank(unsigned char numb) {
	hbrPlayer = CreateSolidBrush(RGB(0,100,0));
	hbrPalayerLight = CreateSolidBrush(RGB(0,50,0));
	hbrEnemy = CreateSolidBrush(RGB(100,0,0));
	hbrEnemyLight = CreateSolidBrush(RGB(50,0,0));
	hpCross = CreatePen(PS_SOLID, 10, RGB(255, 100, 100));
	hpTank = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

	numbTank = numb;

	int i;
	//???????? 1
	SetRect(&baseTank.orient[UP][0], 5 , 5 , 15, 53);
	//???????? 2
	SetRect(&baseTank.orient[UP][1], 44, 5, 54, 53);
	//??????
	SetRect(&baseTank.orient[UP][2], 14, 10, 45, 50);
	//??????
	SetRect(&baseTank.orient[UP][3], 27, 4 , 32, 21);
	//?????
	SetRect(&baseTank.orient[UP][4], 20, 20, 39, 40);
	//?????
	SetRect(&baseTank.orient[UP][5], 5 , 5 , 15, 10);

	for (i = 0; i < 6; i++) {
		SetRect(&baseTank.orient[RIGHT][i], SIZE_TANK - baseTank.orient[UP][i].bottom, baseTank.orient[UP][i].left ,
											SIZE_TANK - baseTank.orient[UP][i].top   , baseTank.orient[UP][i].right);
	}

	for (i = 0; i < 6; i++) {
		SetRect(&baseTank.orient[DOWN][i], SIZE_TANK - baseTank.orient[UP][i].right , SIZE_TANK - baseTank.orient[UP][i].bottom,
			                               SIZE_TANK - baseTank.orient[UP][i].left  , SIZE_TANK - baseTank.orient[UP][i].top   );
	}

	for (i = 0; i < 6; i++) {
		SetRect(&baseTank.orient[LEFT][i], baseTank.orient[UP][i].top   , SIZE_TANK - baseTank.orient[UP][i].right,
			                               baseTank.orient[UP][i].bottom, SIZE_TANK - baseTank.orient[UP][i].left);
	}


	memset(tank,0, sizeof(TANK)* numbTank);
	tank[PLAYER].x = ZONE_LEFT+410;
	tank[PLAYER].y = 410;

	tank[1].x = 370;
	tank[1].y = 10;
	tank[2].x = 1180;
	tank[2].y = 10;
	tank[3].x = 370;
	tank[3].y = 840;
	tank[4].x = 1180;
	tank[4].y = 840;

	tank[1].cell = 0;
	tank[2].cell = 1;
	tank[3].cell = 17;
	tank[4].cell = 18;

	for (i = 0; i < numbTank; i++) {
		tank[i].speed = BASE_SPEED;
		tank[i].damage = BASE_DAMAGE;
		tank[i].hp = BASE_HP;
		tank[i].reload = BASE_RELOAD;
	}
}

void FinalizeTank() {
	DeleteObject(hbrPlayer);
	DeleteObject(hbrPalayerLight);
	DeleteObject(hbrEnemy);
	DeleteObject(hbrEnemyLight);
}


void DrawTank(HDC hdc) {
	SaveDC(hdc);
	SelectObject(hdc, hpTank);
	int i, x, y, k, n, j;

	unsigned char orient;
	for (j = 0; j < numbTank; j++) {
		orient = tank[j].orient;

		if (tank[j].colour) {
			if (j) {
				SelectObject(hdc, hbrEnemyLight);
			}
			else {
				SelectObject(hdc, hbrPalayerLight);
			}
		}
		else {
			if (j) {
				SelectObject(hdc, hbrEnemy);
			}
			else {
				SelectObject(hdc, hbrPlayer);
			}

		}

		// ???????? ?????????????? ?????
		for (i = 0; i < 2; i++) {
			Rectangle(hdc, tank[j].x + baseTank.orient[orient][i].left, tank[j].y + baseTank.orient[orient][i].top,
				tank[j].x + baseTank.orient[orient][i].right, tank[j].y + baseTank.orient[orient][i].bottom);
		}

		if (j) {
			SelectObject(hdc, hbrEnemy);
		}
		else {
			SelectObject(hdc, hbrPlayer);
		}

		for (i = 2; i < 5; i++) {
			Rectangle(hdc, tank[j].x + baseTank.orient[orient][i].left, tank[j].y + baseTank.orient[orient][i].top,
				tank[j].x + baseTank.orient[orient][i].right, tank[j].y + baseTank.orient[orient][i].bottom);
		}

		// ??????????
		x = 0;
		i = 5;
		y = 0;

		if (tank[j].colour) {
			if (j) {
				SelectObject(hdc, hbrEnemy);
			}
			else {
				SelectObject(hdc, hbrPlayer);
			}
		}
		else {
			if (j) {
				SelectObject(hdc, hbrEnemyLight);
			}
			else {
				SelectObject(hdc, hbrPalayerLight);
			}
		}

		for (n = 0; n < 2; n++) {
			switch (orient)
			{
			case UP:
			case DOWN:
				y = 0;
				break;
			case RIGHT:
			case LEFT:
				x = 0;
				break;
			}

			for (k = 0; k < 6; k++) {
				Rectangle(hdc, tank[j].x + baseTank.orient[orient][i].left + x, tank[j].y + baseTank.orient[orient][i].top + y,
					tank[j].x + baseTank.orient[orient][i].right + x, tank[j].y + baseTank.orient[orient][i].bottom + y);
				switch (orient)
				{
				case UP:
					y += 9;
					break;
				case RIGHT:
					x -= 9;
					break;
				case DOWN:
					y -= 9;
					break;
				case LEFT:
					x += 9;
					break;
				default:
					break;
				}
			}
			switch (orient)
			{
			case UP:
				x += 39;
				break;
			case RIGHT:
				y += 39;
				break;
			case DOWN:
				x -= 39;
				break;
			case LEFT:
				y -= 39;
				break;
			default:
				break;
			}

		}
	}

	RestoreDC(hdc, -1);
}

void DrawAllTank(HDC hdc) {
	int i, x, y, tankX = 0, tankY = 0, n, k;
	SaveDC(hdc);

	for (int j = 1; j < numbTank; j++) {
		SelectObject(hdc, hpTank);
		if (tank[j].colour) {
			SelectObject(hdc, hbrEnemyLight);
		}
		else {
			SelectObject(hdc, hbrEnemy);
		}

		// ???????? ?????????????? ?????
		for (i = 0; i < 2; i++) {
			Rectangle(hdc, baseTank.orient[UP][i].left + tankX, baseTank.orient[UP][i].top + tankY,
				baseTank.orient[UP][i].right + tankX, baseTank.orient[UP][i].bottom + tankY);
		}

		SelectObject(hdc, hbrEnemy);

		for (i = 2; i < 5; i++) {
			Rectangle(hdc, baseTank.orient[UP][i].left + tankX, baseTank.orient[UP][i].top + tankY,
				baseTank.orient[UP][i].right + tankX, baseTank.orient[UP][i].bottom + tankY);
		}

		// ??????????
		x = 0;
		i = 5;
		y = 0;

		if (tank[j].colour) {
			SelectObject(hdc, hbrEnemy);
		}
		else {
			SelectObject(hdc, hbrEnemyLight);
		}

		for (n = 0; n < 2; n++) {
			y = 0;

			for (k = 0; k < 6; k++) {
				Rectangle(hdc, baseTank.orient[UP][i].left + x + tankX, baseTank.orient[UP][i].top + y + tankY,
					baseTank.orient[UP][i].right + x + tankX, baseTank.orient[UP][i].bottom + y + tankY);
				y += 9;
			}
			x += 39;
		}
		if (0 == tank[j].hp) {
			SelectObject(hdc, hpCross);
			MoveToEx(hdc, baseTank.orient[UP][0].left + tankX, baseTank.orient[UP][0].top + tankY, NULL);
			LineTo(hdc, baseTank.orient[UP][1].right + tankX, baseTank.orient[UP][1].bottom + tankY);
			MoveToEx(hdc, baseTank.orient[UP][0].left + tankX, baseTank.orient[UP][0].bottom + tankY, NULL);
			LineTo(hdc, baseTank.orient[UP][1].right + tankX, baseTank.orient[UP][1].top + tankY);
		}
		tankY += 60;
	}

	RestoreDC(hdc, -1);
}


void MoveTank(unsigned char orient, unsigned char numb) {

	tank[numb].colour = !tank[numb].colour;

	if (orient == tank[numb].orient) {
		switch (orient)
		{
		case UP:
			tank[numb].y -= tank[numb].speed;
			break;
		case RIGHT:
			tank[numb].x += tank[numb].speed;
			break;
		case DOWN:
			tank[numb].y += tank[numb].speed;
			break;
		case LEFT:
			tank[numb].x -= tank[numb].speed;
			break;
		default:
			break;
		}
	}
	else {
		tank[numb].orient = orient;
	}

}

void TankReload() {
	for (int i = 0; i < numbTank; i++) {
		if (0 != tank[i].timeToReload) {
			tank[i].timeToReload--;
		}
	}
}
