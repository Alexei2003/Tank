#pragma once

#include <Windows.h>

#define SHELL_SIZE 4

typedef struct _SHELL {
	short int x;
	short int y;
	unsigned char damage;
	unsigned char orient;
	struct _SHELL* next;
}SHELL;

extern SHELL* finShell;
extern SHELL* headShell;

void InitializationShell();
void FinalizeShell();
void DrawShell(HDC hdc);
void MoveShell(SHELL* now);
void DeleteShell(SHELL* now, SHELL* pred);