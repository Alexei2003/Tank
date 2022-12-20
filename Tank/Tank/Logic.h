#pragma once

extern char game;

void MoveAndCheckingCollisionsTank(unsigned char orient, unsigned char numb);
void Shot(unsigned char numb);
void CheckingCollisionsShell();
void InitializationAI();
void AI();
void CheckGame();