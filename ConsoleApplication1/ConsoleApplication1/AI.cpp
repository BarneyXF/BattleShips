#include "stdafx.h"
#include <random>
#include <ctime>

#include "AI.h"

// Main function controlling AI's turn.
void TurnOfAI(SeaCell(*playersField)[11][11], Player(*playersPointer), DamagedShipToBeDestroedByAI *shipToAttack)
{
	srand(time(0));
	ShotResult result;
	int x = 0;
	int y = 0;

	// True, if shot was successful.
	bool extraAction = false;

	// Shortcut
	bool specialTactic = shipToAttack->specialTactic;

	// Doing shots until miss
	do
	{
		if (!specialTactic) ChooseRandomSquare(&x, &y, playersField);
		else tryDestroyingSecondDeck(&x, &y, playersField);

		//TODO: ShootingChecker doesn't work right. UPDATE: Pull Valera's commit.
		result = ShootingChecker(&x, &y, playersField, playersPointer);
		extraAction = ContinueAction(&x, &y, result, playersField, playersPointer, shipToAttack);
	} 
	while (extraAction);
}

// Choosing square to shoot.
void ChooseRandomSquare(int (*x), int (*y), SeaCell(*playersField)[11][11])
{
	SeaCell squareToShoot = empty;

	do
	{
		*x = rand() % 10;
		*y = rand() % 10;
		squareToShoot = (*playersField)[*x][*y];
	} 
	while ((squareToShoot == marked) || (squareToShoot == kill));
}



// Making actions after getting info about result.
// Returns true, if extra action given (shot damaged or killed ship).
bool ContinueAction(int *x, int *y, ShotResult result, SeaCell(*playersField)[11][11], 
					Player(*playersPointer), DamagedShipToBeDestroedByAI (*shipToDestroy))
{
	switch (result)
	{
		case miss:
		{
			// Put marker to reflect miss on player's field.
			(*playersField)[*x][*y] = marked;

			return false;
		}
		
		case damaged:
		{
			AfterPlayersShipWasDamaged(*x, *y, shipToDestroy, playersField, playersPointer);
			return true;
		}

		case killed:
		{
			AfterPlayersShipWasKilled(*x, *y, shipToDestroy, playersField, playersPointer);
			return true;
		}
	}
}

// Fills some fields to prepare for next shot.
void AfterPlayersShipWasDamaged(int x, int y, DamagedShipToBeDestroedByAI *shipToDestroy, 
								SeaCell(*playersField)[11][11], Player *playersPointer)
{
	// Put marker to reflect killed deck on player's field.
	(*playersField)[x][y] = kill;

	// Removing destoyed deck.
	playersPointer->count.totalNumOfPlSqares--;

	// Shortcuts. 
	DamagedCellsCoordinates *killedCell = &(shipToDestroy->shotShipDecksCoordinates);
	int *nextDeck = &(shipToDestroy->numberOfDestroyedDecks);

	// Saving coordinates of destroyed deck.
	(*killedCell).x[(*nextDeck)] = x;
	(*killedCell).y[(*nextDeck)] = y;

	// Incrementing index to fill next deck coordinates of damaged ship.
	(*nextDeck)++;

	// Confirms that ship is 3- or 4-decker. Then new tactic is chosen.
	if ((*nextDeck) == 2) tacticToDestroyMoreThanTwoDecker(shipToDestroy, playersField);

	// Stops random square shooting.
	(*shipToDestroy).specialTactic = true;
}

// Fills some data on field and cleans info about damaged ship.
void AfterPlayersShipWasKilled(int x, int y, DamagedShipToBeDestroedByAI *shipToDestroy,
							   SeaCell(*playersField)[11][11], Player *playersPointer)
{
	// Put marker to reflect killed deck on player's field.
	(*playersField)[x][y] = kill;

	// Removing destoyed deck.
	playersPointer->count.totalNumOfPlSqares--;

	// Clean intermidiate data of destroyed ship.
	shipToDestroy->numberOfDestroyedDecks = 0;

	// TODO: function, that will set 'marked' to all squares of 
	// 'playersField' near destroyed ship, so AI ignores them. 

	// Allows random square shooting.
	shipToDestroy->specialTactic = false;

}



// AI makes deliberate choice of next square to shoot.
void tryDestroyingSecondDeck(int *x, int *y, SeaCell(*playersField)[11][11])
{
	// Confirms possibility to shoot on chosen side.
	bool chosenSuccessfully = false;

	// Randoming side to shoot.
	int sideToShootChooser = 0;
	
	// Reapeat, until chosen cell to shoot is valid for a shot.
	do
	{
		sideToShootChooser = rand() % 4;

		switch (sideToShootChooser)
		{
			case left:
			{
				chosenSuccessfully = tryChoosingLeft(x, *y, playersField);
				break;
			}

			case right:
			{
				chosenSuccessfully = tryChoosingRight(x, *y, playersField);
				break;
			}

			case top:
			{
				chosenSuccessfully = tryChoosingTop(*x, y, playersField);
				break;
			}

			case bottom:
			{
				chosenSuccessfully = tryChoosingBottom(*x, y, playersField);
				break;
			}
		}
	} 
	while (!chosenSuccessfully);
} 

// Checking if AI's next square to shoot can be LEFT from previous.
bool tryChoosingLeft(int *x, int y, SeaCell(*playersField)[11][11])
{
	// Checks if coordinate is out of bound, so we do not shoot beyond field.
	if ((*x) <= lowerFieldBound) return false;

	// Shortcut
	SeaCell cellToShoot = (*playersField)[(*x) - 1][y];

	// Checks if we already shot this cell.
	if (cellToShoot == marked) return false;

	// Saves chosen cell's 'x' coordinate.
	(*x)--;

	return true;
}

// Checking if AI's next square to shoot can be RIGHT from previous.
bool tryChoosingRight(int *x, int y, SeaCell(*playersField)[11][11])
{
	// Checks if coordinate is out of bound, so we do not shoot beyond field.
	if ((*x) >= upperFieldBound) return false;

	// Shortcut
	SeaCell cellToShoot = (*playersField)[(*x) + 1][y];

	// Checks if we already shot this cell.
	if (cellToShoot == marked) return false;

	// Saves chosen cell's 'x' coordinate.
	(*x)++;

	return true;
}

// Checking if AI's next square to shoot can be TOP from previous.
bool tryChoosingTop(int x, int *y, SeaCell(*playersField)[11][11])
{
	// Checks if coordinate is out of bound, so we do not shoot beyond field.
	if ((*y) <= lowerFieldBound) return false;

	// Shortcut
	SeaCell cellToShoot = (*playersField)[x][(*y) - 1];

	// Checks if we already shot this cell.
	if (cellToShoot == marked) return false;

	// Saves chosen cell's 'y' coordinate.
	(*y)--;

	return true;
}

// Checking if AI's next square to shoot can be BOTTOM from previous.
bool tryChoosingBottom(int x, int *y, SeaCell(*playersField)[11][11])
{
	// Checks if coordinate is out of bound, so we do not shoot beyond field.
	if ((*y) >= upperFieldBound) return false;

	// Shortcut
	SeaCell cellToShoot = (*playersField)[x][(*y) + 1];

	// Checks if we already shot this cell.
	if (cellToShoot == marked) return false;

	// Saves chosen cell's 'y' coordinate.
	(*y)++;

	return true;
}



// Chooses new cells to shoot, considering that ship has more than two decks.
void tacticToDestroyMoreThanTwoDecker(DamagedShipToBeDestroedByAI *shipToDestroy, SeaCell(*playersField)[11][11])
{
	// Shortcuts for two first damaged decks's X coordinates.
	int *x0 = &(shipToDestroy->shotShipDecksCoordinates.x[0]);
	int x1 = shipToDestroy->shotShipDecksCoordinates.x[1];

	// Checks if ship is set horizontally or vertically
	if (*x0 != x1) HorizontalShot(shipToDestroy, playersField);
	else VerticalShot(shipToDestroy, playersField);
}

// Chooses to shoot left or right.
void HorizontalShot(DamagedShipToBeDestroedByAI *shipToDestroy, SeaCell(*playersField)[11][11])
{
	// Left = 0 and right = 1 in 'SideToShoot' enum.
	int leftOrRightChooser = rand() % 2;

	switch (leftOrRightChooser)
	{
		case left:
		{
			checkLeftShotPossibility(shipToDestroy, playersField);
			break;
		}
		case right:
		{
			checkRightShotPossibility(shipToDestroy, playersField);
			break;
		}
	}
}

// Chooses to shoot top or bottom.
void VerticalShot(DamagedShipToBeDestroedByAI *shipToDestroy, SeaCell(*playersField)[11][11])
{
	// Top = 2 and right = 3 in 'SideToShoot' enum.
	int topOrBottomChoser = rand() % 2 + 2;

	switch (topOrBottomChoser)
	{
		case top:
		{
			checkTopShotPossibility(shipToDestroy, playersField);
			break;
		}
		case bottom:
		{
			checkBottomShotPossibility(shipToDestroy, playersField);
			break;
		}
	}
}

// Checks left side shot possibility. 
// If unable to shoot left, sets to shoot right.
void checkLeftShotPossibility(DamagedShipToBeDestroedByAI *shipToDestroy, SeaCell(*playersField)[11][11])
{
	// Shortcuts for coordinates to shoot.
	int *tryX = &(shipToDestroy->coordinatesToShoot.x);
	int *tryY = &(shipToDestroy->coordinatesToShoot.y);

	// Setting starting values on first damaged deck's coordinates,
	// so we can search for other possible decks locations from it.
	*tryX = shipToDestroy->shotShipDecksCoordinates.x[0];
	*tryY = shipToDestroy->shotShipDecksCoordinates.y[0];

	// Keeps temporarily unchecked for possibility of shot cell.
	SeaCell cellToShoot = empty;

	// Continuous checking cells, until sea cell was not shot by AI before.
	do
	{
		// Checks for bound, so we do not shoot out of field.
		if (*tryX > lowerFieldBound) (*tryX)--;
		// Left side is near bound, so we can shoot only to the right.
		else 
		{ 
			checkRightShotPossibility(shipToDestroy, playersField); 
			return; 
		}

		cellToShoot = (*playersField)[*tryX][*tryY];
	} while (cellToShoot == kill);

	// Left side is marked, so we can shoot only to the right.
	if (cellToShoot == marked) checkRightShotPossibility(shipToDestroy, playersField);
}

// Checks right side shot possibility. 
// If unable to shoot right, sets to shoot left.
void checkRightShotPossibility(DamagedShipToBeDestroedByAI *shipToDestroy, SeaCell(*playersField)[11][11])
{
	// Shortcuts for coordinates to shoot.
	int *tryX = &(shipToDestroy->coordinatesToShoot.x);
	int *tryY = &(shipToDestroy->coordinatesToShoot.y);

	// Setting starting values on first damaged deck's coordinates,
	// so we can search for other possible decks locations from it.
	*tryX = shipToDestroy->shotShipDecksCoordinates.x[0];
	*tryY = shipToDestroy->shotShipDecksCoordinates.y[0];

	// Keeps temporarily unchecked for possibility of shot cell.
	SeaCell cellToShoot = empty;

	// Continuous checking cells, until sea cell was not shot by AI before.
	do
	{
		// Checks for bound, so we do not shoot out of field.
		if (*tryX < upperFieldBound) (*tryX)++;
		// Left side is near bound, so we can shoot only to the right.
		else
		{
			checkLeftShotPossibility(shipToDestroy, playersField);
			return;
		}

		cellToShoot = (*playersField)[*tryX][*tryY];
	} while (cellToShoot == kill);

	// Right side is marked, so we can shoot only to the left.
	if (cellToShoot == marked) checkLeftShotPossibility(shipToDestroy, playersField);
}

// Checks top side shot possibility. 
// If unable to shoot top, sets to shoot bottom.
void checkTopShotPossibility(DamagedShipToBeDestroedByAI *shipToDestroy, SeaCell(*playersField)[11][11])
{
	// Shortcuts for coordinates to shoot.
	int *tryX = &(shipToDestroy->coordinatesToShoot.x);
	int *tryY = &(shipToDestroy->coordinatesToShoot.y);

	// Setting starting values on first damaged deck's coordinates,
	// so we can search for other possible decks locations from it.
	*tryX = shipToDestroy->shotShipDecksCoordinates.x[0];
	*tryY = shipToDestroy->shotShipDecksCoordinates.y[0];

	// Keeps temporarily unchecked for possibility of shot cell.
	SeaCell cellToShoot = empty;

	// Continuous checking cells, until sea cell was not shot by AI before.
	do
	{
		// Checks for bound, so we do not shoot out of field.
		if (*tryY > lowerFieldBound) (*tryY)--;
		// Top side is near bound, so we can shoot only to the bottom.
		else
		{
			checkBottomShotPossibility(shipToDestroy, playersField);
			return;
		}

		cellToShoot = (*playersField)[*tryX][*tryY];
	} while (cellToShoot == kill);

	// Top side is marked, so we can shoot only to the bottom.
	if (cellToShoot == marked) checkBottomShotPossibility(shipToDestroy, playersField);
}

// Checks bottom side shot possibility. 
// If unable to shoot bottom, sets to shoot top.
void checkBottomShotPossibility(DamagedShipToBeDestroedByAI *shipToDestroy, SeaCell(*playersField)[11][11])
{
	// Shortcuts for coordinates to shoot.
	int *tryX = &(shipToDestroy->coordinatesToShoot.x);
	int *tryY = &(shipToDestroy->coordinatesToShoot.y);

	// Setting starting values on first damaged deck's coordinates,
	// so we can search for other possible decks locations from it.
	*tryX = shipToDestroy->shotShipDecksCoordinates.x[0];
	*tryY = shipToDestroy->shotShipDecksCoordinates.y[0];

	// Keeps temporarily unchecked for possibility of shot cell.
	SeaCell cellToShoot = empty;

	// Continuous checking cells, until sea cell was not shot by AI before.
	do
	{
		// Checks for bound, so we do not shoot out of field.
		if (*tryY < upperFieldBound) (*tryY)++;
		// Bottom side is near bound, so we can shoot only to the top.
		else
		{
			checkTopShotPossibility(shipToDestroy, playersField);
			return;
		}

		cellToShoot = (*playersField)[*tryX][*tryY];
	} while (cellToShoot == kill);

	// Bottom side is marked, so we can shoot only to the top.
	if (cellToShoot == marked) checkRightShotPossibility(shipToDestroy, playersField);
}
