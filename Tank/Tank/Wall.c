#include"Wall.h"

#define ZONE_TOP 0
#define ZONE_BOTTOM 840
#define ZONE_RIGHT 1190
#define	ZONE_LEFT 350

RECT base_wall[2];
unsigned char numbWall=0;

WALL wall[68];

HPEN     hpNoBorder, hpBorder;
HBRUSH   hbrWall;

unsigned char HorizontalLineWall(unsigned char StartInd,unsigned char numb, short int  x, short int y) {
	x += ZONE_LEFT;
	y += ZONE_TOP;
	wall[StartInd].x = x;
	wall[StartInd].y = y;
	for (int i = StartInd+1; i < StartInd+numb; i++) {
		wall[i].x = wall[i - 1].x + 52;
		wall[i].y = y;
	}
	return StartInd += numb;
}

unsigned char VerticalLineWall(unsigned char StartInd, unsigned char numb, short int  x, short int y) {
	x += ZONE_LEFT;
	y += ZONE_TOP;
	wall[StartInd].x = x;
	wall[StartInd].y = y;
	for (int i = StartInd + 1; i < StartInd + numb; i++) {
		wall[i].x = x;
		wall[i].y = wall[i - 1].y + 52;
	}
	return StartInd += numb;
}

void InitializationWall(){
	hbrWall = CreateSolidBrush(RGB(0,0,100));
	hpNoBorder = CreatePen(PS_SOLID,0,RGB(0,0,100));
	hpBorder = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));

	SetRect(&base_wall[0],4 ,4 ,30,17);
	SetRect(&base_wall[1],4 ,17,17,30);

	numbWall = HorizontalLineWall(numbWall,14, 81, 60);

	numbWall = VerticalLineWall(numbWall, 4, 120, 180);
	numbWall = VerticalLineWall(numbWall, 4, 120, 480);

	numbWall = HorizontalLineWall(numbWall, 3, 240, 180);
	numbWall = VerticalLineWall(numbWall, 3, 344, 232);

	numbWall = HorizontalLineWall(numbWall, 3, 240, 636);
	numbWall = VerticalLineWall(numbWall, 3, 344, 480);

	numbWall = HorizontalLineWall(numbWall, 3, 480, 180);
	numbWall = VerticalLineWall(numbWall, 3, 480, 232);

	numbWall = HorizontalLineWall(numbWall, 3, 480, 636);
	numbWall = VerticalLineWall(numbWall, 3, 480, 480);

	numbWall = VerticalLineWall(numbWall, 4, 720, 180);
	numbWall = VerticalLineWall(numbWall, 4, 720, 480);

	numbWall = HorizontalLineWall(numbWall, 14, 81, 780);
}

void FinalizeWall() {
	DeleteObject(hbrWall);
	DeleteObject(hpNoBorder);
	DeleteObject(hpBorder);
}

void DrawWall(HDC hdc) {
	SaveDC(hdc);

	SelectObject(hdc, hbrWall);

	Rectangle(hdc, ZONE_LEFT, ZONE_TOP, ZONE_RIGHT + 60, ZONE_TOP + 5);
	Rectangle(hdc, ZONE_RIGHT + 55, ZONE_TOP, ZONE_RIGHT + 60, ZONE_BOTTOM + 60);
	Rectangle(hdc, ZONE_LEFT, ZONE_BOTTOM + 55, ZONE_RIGHT + 60, ZONE_BOTTOM + 60);
	Rectangle(hdc, ZONE_LEFT, ZONE_TOP, ZONE_LEFT + 5, ZONE_BOTTOM + 60);

	int x, y, i, j, n;

	for (i = 0; i < numbWall; i++) {

		y = 0;
		SelectObject(hdc, hpBorder);
		for (j = 0; j < 2; j++) {
			x = 0;
			for (n = 0; n < 2; n++) {
				Rectangle(hdc, wall[i].x + base_wall[0].left + x, wall[i].y + base_wall[0].top + y, wall[i].x + base_wall[0].right + x, wall[i].y + base_wall[0].bottom + y);
				x = 26;
			}
			x = 13;
			y += 13;
			Rectangle(hdc, wall[i].x + base_wall[0].left + x, wall[i].y + base_wall[0].top + y, wall[i].x + base_wall[0].right + x, wall[i].y + base_wall[0].bottom + y);
			y += 13;
		}

		y = 0;
		SelectObject(hdc, hpNoBorder);
		for (j = 0; j < 2; j++) {
			x = 0;
			for (n = 0; n < 2; n++) {
				Rectangle(hdc, wall[i].x + base_wall[1].left + x, wall[i].y + base_wall[1].top + y, wall[i].x + base_wall[1].right + x, wall[i].y + base_wall[1].bottom + y);
				x = 39;
			}
			y += 26;
		}
	}

	RestoreDC(hdc, -1);
}