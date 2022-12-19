#include "Shell.h"

#define UP 0
#define RIGHT 1
#define	DOWN 2
#define LEFT 3

#define BASE_SHELL_SPEED 20

RECT baseShell;
SHELL *headShell;
SHELL* finShell;

HBRUSH   hbrShell;

void InitializationShell() {
	SetRect(&baseShell, 0, 0, SHELL_SIZE + 1, SHELL_SIZE + 1);
	headShell = NULL;
	while (NULL == headShell) {
		headShell = (SHELL*)(malloc(sizeof(SHELL)));
	}
	memset(&headShell, 0, sizeof(SHELL));

	finShell = headShell;

	hbrShell = CreateSolidBrush(RGB(100, 0, 0));
}

void FinalizeShell() {
	for (SHELL*pred = headShell; NULL != headShell;) {
		headShell = headShell->next;
		free(pred);
		pred = headShell;
	}
	DeleteObject(hbrShell);
}

void DrawShell(HDC hdc) {

	SaveDC(hdc);
	SelectObject(hdc, hbrShell);
	for (SHELL* now = headShell->next; NULL != now; now = now->next) {
		Rectangle(hdc, baseShell.left + now->x, baseShell.left + now->y, baseShell.right + now->x, baseShell.bottom + now->y);
	}
	RestoreDC(hdc, -1);
}

void MoveShell(SHELL *now) {
		switch (now->orient)
		{
		case UP:
			now->y -= BASE_SHELL_SPEED;
			break;
		case RIGHT:
			now->x += BASE_SHELL_SPEED;
			break;
		case DOWN:
			now->y += BASE_SHELL_SPEED;
			break;
		case LEFT:
			now->x -= BASE_SHELL_SPEED;
			break;
	}
}

void DeleteShell(SHELL* now, SHELL* pred) {
	pred->next = now->next;
	if (now == finShell) {
		finShell = pred;
		finShell->next = NULL;
	}
	free(now);
}