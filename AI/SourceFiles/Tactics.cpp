#include <random>
#include <ctime>

#include "../../Headers/AI/AI.h"
#include "../../Headers/AI/Tactics.h"

/* Chooses one of 3 tactics, depending on situation.
 *
 * 1st if true: no ship is currently damaged.
 * 2nd if true: only one deck is destroyed.
 * 3rd if true: more than one deck is destroyed.
 */
void ChoosingTactic(int *x, int *y, DamagedShipToBeDestroedByAI *shipToAttack, SeaCell(*playersField)[11][11])
{
	// Shortcuts
	bool *specialTactic = &(shipToAttack->specialTactic);
	int *destroyedDecks = &(shipToAttack->numberOfDestroyedDecks);

	if (!(*specialTactic)) ChooseRandomSquare(x, y, playersField);
	else if (*destroyedDecks < 2) TryDestroyingSecondDeck(x, y, playersField);
	else
	{
		TacticToDestroyMoreThanTwoDecker(shipToAttack, playersField);

		// Passing recieved coordinates to local x and y.
		*x = shipToAttack->coordinatesToShoot.x;
		*y = shipToAttack->coordinatesToShoot.y;
	}
}


// Choosing square to shoot.
void ChooseRandomSquare(int(*x), int(*y), SeaCell(*playersField)[11][11])
{
	SeaCell squareToShoot = empty;

	do
	{
		*x = rand() % 10;
		*y = rand() % 10;
		squareToShoot = (*playersField)[*x][*y];
	} while ((squareToShoot == marked) || (squareToShoot == kill));
}


// AI makes deliberate choice of next square to shoot.
void TryDestroyingSecondDeck(int *x, int *y, SeaCell(*playersField)[11][11])
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
			chosenSuccessfully = TryChoosingLeft(x, *y, playersField);
			break;
		}

		case right:
		{
			chosenSuccessfully = TryChoosingRight(x, *y, playersField);
			break;
		}

		case top:
		{
			chosenSuccessfully = TryChoosingTop(*x, y, playersField);
			break;
		}

		case bottom:
		{
			chosenSuccessfully = TryChoosingBottom(*x, y, playersField);
			break;
		}
		}
	} while (!chosenSuccessfully);
}

// Checking if AI's next square to shoot can be LEFT from previous.
bool TryChoosingLeft(int *x, int y, SeaCell(*playersField)[11][11])
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
bool TryChoosingRight(int *x, int y, SeaCell(*playersField)[11][11])
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
bool TryChoosingTop(int x, int *y, SeaCell(*playersField)[11][11])
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
bool TryChoosingBottom(int x, int *y, SeaCell(*playersField)[11][11])
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
void TacticToDestroyMoreThanTwoDecker(DamagedShipToBeDestroedByAI *shipToAttack, SeaCell(*playersField)[11][11])
{
	// Shortcuts for two first damaged decks's X coordinates.
	int *x0 = &(shipToAttack->shotShipDecksCoordinates.x[0]);
	int x1 = shipToAttack->shotShipDecksCoordinates.x[1];

	// Checks if ship is set horizontally or vertically
	if (*x0 != x1) HorizontalShot(shipToAttack, playersField);
	else VerticalShot(shipToAttack, playersField);
}

// Chooses to shoot left or right.
void HorizontalShot(DamagedShipToBeDestroedByAI *shipToAttack, SeaCell(*playersField)[11][11])
{
	// Left = 0 and right = 1 in 'SideToShoot' enum.
	int leftOrRightChooser = rand() % 2;

	switch (leftOrRightChooser)
	{
	case left:
	{
		CheckLeftShotPossibility(shipToAttack, playersField);
		break;
	}
	case right:
	{
		CheckRightShotPossibility(shipToAttack, playersField);
		break;
	}
	}
}

// Chooses to shoot top or bottom.
void VerticalShot(DamagedShipToBeDestroedByAI *shipToAttack, SeaCell(*playersField)[11][11])
{
	// Top = 2 and right = 3 in 'SideToShoot' enum.
	int topOrBottomChoser = rand() % 2 + 2;

	switch (topOrBottomChoser)
	{
	case top:
	{
		CheckTopShotPossibility(shipToAttack, playersField);
		break;
	}
	case bottom:
	{
		CheckBottomShotPossibility(shipToAttack, playersField);
		break;
	}
	}
}

// Checks left side shot possibility. 
// If unable to shoot left, sets to shoot right.
void CheckLeftShotPossibility(DamagedShipToBeDestroedByAI *shipToAttack, SeaCell(*playersField)[11][11])
{
	// Shortcuts for coordinates to shoot.
	int *tryX = &(shipToAttack->coordinatesToShoot.x);
	int *tryY = &(shipToAttack->coordinatesToShoot.y);

	// Setting starting values on first damaged deck's coordinates,
	// so we can search for other possible decks locations from it.
	*tryX = shipToAttack->shotShipDecksCoordinates.x[0];
	*tryY = shipToAttack->shotShipDecksCoordinates.y[0];

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
			CheckRightShotPossibility(shipToAttack, playersField);
			return;
		}

		cellToShoot = (*playersField)[*tryX][*tryY];
	} while (cellToShoot == kill);

	// Left side is marked, so we can shoot only to the right.
	if (cellToShoot == marked) CheckRightShotPossibility(shipToAttack, playersField);
}

// Checks right side shot possibility. 
// If unable to shoot right, sets to shoot left.
void CheckRightShotPossibility(DamagedShipToBeDestroedByAI *shipToAttack, SeaCell(*playersField)[11][11])
{
	// Shortcuts for coordinates to shoot.
	int *tryX = &(shipToAttack->coordinatesToShoot.x);
	int *tryY = &(shipToAttack->coordinatesToShoot.y);

	// Setting starting values on first damaged deck's coordinates,
	// so we can search for other possible decks locations from it.
	*tryX = shipToAttack->shotShipDecksCoordinates.x[0];
	*tryY = shipToAttack->shotShipDecksCoordinates.y[0];

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
			CheckLeftShotPossibility(shipToAttack, playersField);
			return;
		}

		cellToShoot = (*playersField)[*tryX][*tryY];
	} while (cellToShoot == kill);

	// Right side is marked, so we can shoot only to the left.
	if (cellToShoot == marked) CheckLeftShotPossibility(shipToAttack, playersField);
}

// Checks top side shot possibility. 
// If unable to shoot top, sets to shoot bottom.
void CheckTopShotPossibility(DamagedShipToBeDestroedByAI *shipToAttack, SeaCell(*playersField)[11][11])
{
	// Shortcuts for coordinates to shoot.
	int *tryX = &(shipToAttack->coordinatesToShoot.x);
	int *tryY = &(shipToAttack->coordinatesToShoot.y);

	// Setting starting values on first damaged deck's coordinates,
	// so we can search for other possible decks locations from it.
	*tryX = shipToAttack->shotShipDecksCoordinates.x[0];
	*tryY = shipToAttack->shotShipDecksCoordinates.y[0];

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
			CheckBottomShotPossibility(shipToAttack, playersField);
			return;
		}

		cellToShoot = (*playersField)[*tryX][*tryY];
	} while (cellToShoot == kill);

	// Top side is marked, so we can shoot only to the bottom.
	if (cellToShoot == marked) CheckBottomShotPossibility(shipToAttack, playersField);
}

// Checks bottom side shot possibility. 
// If unable to shoot bottom, sets to shoot top.
void CheckBottomShotPossibility(DamagedShipToBeDestroedByAI *shipToAttack, SeaCell(*playersField)[11][11])
{
	// Shortcuts for coordinates to shoot.
	int *tryX = &(shipToAttack->coordinatesToShoot.x);
	int *tryY = &(shipToAttack->coordinatesToShoot.y);

	// Setting starting values on first damaged deck's coordinates,
	// so we can search for other possible decks locations from it.
	*tryX = shipToAttack->shotShipDecksCoordinates.x[0];
	*tryY = shipToAttack->shotShipDecksCoordinates.y[0];

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
			CheckTopShotPossibility(shipToAttack, playersField);
			return;
		}

		cellToShoot = (*playersField)[*tryX][*tryY];
	} while (cellToShoot == kill);

	// Bottom side is marked, so we can shoot only to the top.
	if (cellToShoot == marked) CheckRightShotPossibility(shipToAttack, playersField);
}