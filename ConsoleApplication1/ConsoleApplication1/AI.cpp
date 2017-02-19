#include "stdafx.h"
#include <random>
#include <ctime>

#include "MainLogic.h"
#include "AI.h"

// Main function controlling AI's turn.
void TurnOfAI(SeaCell(*playerField)[11][11], SeaCell (*specialPlayersFieldForAI)[11][11],
			  Player(*playersPointer), bool *specialTactic)
{
	srand(time(0));
	ShotResult result;
	int x = 0;
	int y = 0;

	// True, if shot was successful.
	bool extraAction = false;

	// Doing shots until miss
	do
	{
		if (!specialTactic)
		{
			ChooseRandomSquare(&x, &y, specialPlayersFieldForAI);
		}
		else
		{
			// Searching for coordinates of 'firsWoundShot' to continue attacking around it.
			for (int i = 0; i < 10; i++)
			{
				for (int j; j < 10; j++)
				{
					if ((*specialPlayersFieldForAI)[i][j] == firstWoundShot)
					{
						x = i;
						y = j;
					}
				}
			}

			ShootingTacticChange(&x, &y, specialPlayersFieldForAI, playersPointer);
		}

		//TODO: ShootingChecker doesn't work right. UPDATE: Pull Valera's commit.
		result = ShootingChecker(&x, &y, specialPlayersFieldForAI, playersPointer);
		extraAction = ContinueAction(&x, &y, result, specialPlayersFieldForAI, 
									 playersPointer, playerField, specialTactic);
	} 
	while (extraAction);
}

// Choosing square to shoot.
void ChooseRandomSquare(int (*x), int (*y), SeaCell(*specialPlayersFieldForAI)[11][11])
{
	SeaCell squareToShoot = empty;

	do
	{
		*x = rand() % 10;
		*y = rand() % 10;
		squareToShoot = (*specialPlayersFieldForAI)[*x][*y];
	} 
	while ((squareToShoot == marked) || (squareToShoot == kill));
}

// Making actions after getting info about result.
// Returns true, if extra action given (shot wounded or killed ship).
bool ContinueAction(int *x, int *y, ShotResult result, SeaCell(*specialPlayersFieldForAI)[11][11], 
					Player(*playersPointer), SeaCell(*playerField)[11][11], bool *specialTactic)
{
	switch (result)
	{
		case miss:
		{
			// Put marker to reflect miss on player's field.
			(*playerField)[*x][*y] = marked;
			(*specialPlayersFieldForAI)[*x][*y] = marked;

			return false;
		}

		case wounded:
		{
			// Put marker to reflect killed deck on player's and special fields.
			(*playerField)[*x][*y] = kill;
			(*specialPlayersFieldForAI)[*x][*y] = kill;

			(*playersPointer).count.totalNumOfPlSqares--;

			/* AI has just wounded enemy's ship. We are checking if it is not the first wound of this ship. 
			 * If it is the first wound - 'if' below will assign new 'firstWoundShot' to shot square.
			 * If it not, we just break of loops and do not go into 'if' below. 
			 * This square is the starting point for AI to find other decks of wounded ship.
			 */
			int j = 0;
			for (int i = 0; i < 10; i++)
			{
				for (; j < 10; j++)
				{
					// Is true only after second or third wound of the ship.
					if ((*specialPlayersFieldForAI)[i][j] == firstWoundShot)
					{
						// TODO!!!: ÇÀÏÎËÍÈÒÜ ÝÒÓ ÔÓÍÊÖÈÞ. ÍÀÄÎ ÓÇÍÀÒÜ, ÍÀ ÊÀÊÎÉ ÊÎÎÐÄÈÍÀÒÍÎÉ ËÈÍÈÈ ÐÀÑÏÎËÀÃÀÅÒÑß
						// ÊÎÐÀÁËÜ, ÏÐÎÄÓÌÀÒÜ ÄÅÉÑÒÂÈß ÄËß ÑËÅÄÓÞÙÈÕ ÈÒÅÐÀÖÈÉ ÏÎÈÑÊÀ ÏÀËÓÁ
						tacticChoiceToDestroy3Or4Decker();
						i = 10;
						break;
					}
				}
			}
			// It would be 'true' only when 'firstWoundShot' was not found.
			if (j == 10)
			{
				(*specialPlayersFieldForAI)[*x][*y] = firstWoundShot;
			}

						

			// Stops random square shooting.
			*specialTactic = true;

			return true;
		}

		case killed:
		{
			// Put marker to reflect killed deck on player's and special fields.
			(*playerField)[*x][*y] = kill;
			(*specialPlayersFieldForAI)[*x][*y] = kill;

			(*playersPointer).count.totalNumOfPlSqares--;

			// After wounded ship was killed, we do not need 'firstWoundShot' marker any more,
			// so we are searching for it. If it exists - we set 'marked' instead.
			for (int i = 0; i < 10; i++)
			{
				for (int j; j < 10; j++)
				{
					if ((*specialPlayersFieldForAI)[i][j] == firstWoundShot)
					{
						(*specialPlayersFieldForAI)[i][j] == marked;
						i = 10;
						break;
					}
				}
			}

			// TODO: function, that will set 'marked' to all squares of 
			// 'specialPlayersFieldForAI' near destroyed ship, so AI ignore them. 

			// Allows random square shooting.
			*specialTactic = false;

			return true;
		}
	}
}

// AI makes deliberate choice of next square to shoot.
void ShootingTacticChange(int *x, int *y, SeaCell(*specialPlayersFieldForAI)[11][11], 
						  Player(*playersPointer))
{
	ShotResult result = miss;

	/* Checking, if AI's next square to attack should be LEFT from previous.
	 *
	 * First condition: checking if current's shot 'x' is not on the border 
	 * of the field, so we won't shoot beyond the field.
	 *
	 * Second condition: checking if new square to shoot is not marked,
	 * i.e. wasn't shot by AI before.
	 *
	 * Othe 'else ifs' conditions are built with similar principle.
	 */
	if  (((*x) > 0) && 
		((*specialPlayersFieldForAI)[(*x) - 1][*y] != marked))
	{
		(*x) -= 1;
	}
	// Checking, if AI's next square to attack should be TOP from previous.
	else if (((*y) > 0) && 
			((*specialPlayersFieldForAI)[*x][(*y) - 1] != marked))
	{
		(*y) -= 1;
	}
	// Checking, if AI's next square to attack should be RIGHT from previous.
	else if (((*x) < 9) && 
			((*specialPlayersFieldForAI)[(*x) + 1][*y] != marked))
	{
		(*x)++;
	}
	// AI's next square to attack should be BOTTOM from previous.
	else 
	{
		(*y)++;
	}
}

void tacticChoiceToDestroy3Or4Decker()
{

}