#include "stdafx.h"
#include <random>
#include <ctime>

#include "MainLogic.h"
#include "AI.h"

void turnOfAI(SeaCell (*playerField)[11][11], Player(*playersPointer))
{
	srand(time(0));
	ShotResult result;
	int x = 0;
	int y = 0;
	bool extraAction = false;

	// Doing shots until miss
	do
	{
		chooseSquare(&x, &y, playerField);
		//TODO: ShootingChecker doesn't work right.
		result = ShootingChecker(&x, &y, playerField, playersPointer);
		extraAction = continueAction(x, y, result, playerField, playersPointer);
	} 
	while (extraAction);
}

// Choosing square to shoot.
void chooseSquare(int (*x), int (*y), SeaCell(*playerField)[11][11])
{
	SeaCell square;
	int l = *x;
	do
	{
		*x = rand() % 10;
		*y = rand() % 10;
		l = *x;
		square = (*playerField)[*x][*y];
	} 
	while ((square == marked) || (square == kill));
}

// Making actions after getting info about result.
bool continueAction(int x, int y, ShotResult result, SeaCell(*playerField)[11][11], Player(*playersPointer))
{
	switch (result)
	{
		case miss:
		{
			(*playerField)[x][y] = marked;
			return false;
		}

		case wounded:
		{
			(*playerField)[x][y] = kill;
			(*playersPointer).count.totalNumOfPlSqares--;
			return true;
		}

		case killed:
		{
			(*playerField)[x][y] = kill;
			(*playersPointer).count.totalNumOfPlSqares--;
			return true;
		}
	}
}

