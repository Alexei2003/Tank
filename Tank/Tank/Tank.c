#include "Tank.h"

#define ENEMY 1
#define BASE_SPEED 5
#define BASE_DAMAGE 1
#define BASE_HP 0
#define BASE_RELOAD 2

BASE_TANK baseTank;
TANK tank[1];
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
	//гусеница 1
	SetRect(&baseTank.orient[UP][0], 5 , 5 , 15, 53);
	//гусеница 2
	SetRect(&baseTank.orient[UP][1], 44, 5, 54, 53);
	//корпус
	SetRect(&baseTank.orient[UP][2], 14, 10, 45, 50);
	//орудие
	SetRect(&baseTank.orient[UP][3], 27, 4 , 32, 21);
	//башня
	SetRect(&baseTank.orient[UP][4], 20, 20, 39, 40);
	//траки
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
	tank[PLAYER].speed = BASE_SPEED;
	tank[PLAYER].damage = BASE_DAMAGE;
	tank[PLAYER].hp = BASE_HP;
	tank[PLAYER].reload = BASE_RELOAD;
	tank[PLAYER].timeToReload = 0;
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
	int i,x,y,k,n,j;

	unsigned char orient;
	for (j = 0; j < numbTank; j++) {
		orient = tank[j].orient;

		if (tank[j].colour) {
			SelectObject(hdc, hbrPalayerLight);
		}
		else {
			SelectObject(hdc, hbrPlayer);
		}

		// Основные прямоугольники танка
		for (i = 0; i < 2; i++) {
			Rectangle(hdc, tank[j].x + baseTank.orient[orient][i].left , tank[j].y + baseTank.orient[orient][i].top,
				           tank[j].x + baseTank.orient[orient][i].right, tank[j].y + baseTank.orient[orient][i].bottom);
		}

		SelectObject(hdc, hbrPlayer);

		for (i = 2; i < 5; i++) {
			Rectangle(hdc, tank[j].x + baseTank.orient[orient][i].left , tank[j].y + baseTank.orient[orient][i].top,
						   tank[j].x + baseTank.orient[orient][i].right, tank[j].y + baseTank.orient[orient][i].bottom);
		}

		// переделать
		x = 0;
		i = 5;
		y = 0;

		if (tank[j].colour) {
			SelectObject(hdc, hbrPlayer);
		}
		else {
			SelectObject(hdc, hbrPalayerLight);
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
				Rectangle(hdc, tank[j].x + baseTank.orient[orient][i].left+x , tank[j].y + baseTank.orient[orient][i].top+y,
					           tank[j].x + baseTank.orient[orient][i].right+x, tank[j].y + baseTank.orient[orient][i].bottom+y);
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
	int i, x, y, tankX, tankY, n, k;
	SaveDC(hdc);

	for (int j = 0; j < numbTank; j++) {
		SelectObject(hdc, hpTank);
		if (tank[j].colour) {
			SelectObject(hdc, hbrEnemyLight);
		}
		else {
			SelectObject(hdc, hbrEnemy);
		}

		// Основные прямоугольники танка
		for (i = 0; i < 2; i++) {
			Rectangle(hdc, baseTank.orient[UP][i].left, baseTank.orient[UP][i].top,
				baseTank.orient[UP][i].right, baseTank.orient[UP][i].bottom);
		}

		SelectObject(hdc, hbrEnemy);

		for (i = 2; i < 5; i++) {
			Rectangle(hdc, baseTank.orient[UP][i].left, baseTank.orient[UP][i].top,
				baseTank.orient[UP][i].right, baseTank.orient[UP][i].bottom);
		}

		// переделать
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
				Rectangle(hdc, baseTank.orient[UP][i].left + x, baseTank.orient[UP][i].top + y,
					baseTank.orient[UP][i].right + x, baseTank.orient[UP][i].bottom + y);
				y += 9;
			}
			x += 39;
		}
		if (0 == tank[j].hp) {
			SelectObject(hdc, hpCross);
			MoveToEx(hdc, baseTank.orient[UP][0].left, baseTank.orient[UP][0].top, NULL);
			LineTo(hdc, baseTank.orient[UP][1].right, baseTank.orient[UP][1].bottom);
			MoveToEx(hdc, baseTank.orient[UP][0].left, baseTank.orient[UP][0].bottom, NULL);
			LineTo(hdc, baseTank.orient[UP][1].right, baseTank.orient[UP][1].top);
		}
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
