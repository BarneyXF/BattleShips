#include "../../Headers/AI/AI.h"
#include "../../Headers/AI/ActionsAfterShot.h"

// Making actions after getting info about result.
// Returns true, if extra action given (shot damaged or killed ship).
bool ContinueAction(int *x, int *y, ShotResult result, SeaCell(*playersField)[11][11],
	Player(*playersPointer), DamagedShipToBeDestroedByAI(*shipToAttack))
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
		AfterPlayersShipWasDamaged(*x, *y, shipToAttack, playersField, playersPointer);
		return true;
	}

	case killed:
	{
		AfterPlayersShipWasKilled(*x, *y, shipToAttack, playersField, playersPointer);
		return true;
	}
	}
}

// Fills some fields to prepare for next shot.
void AfterPlayersShipWasDamaged(int x, int y, DamagedShipToBeDestroedByAI *shipToAttack,
	SeaCell(*playersField)[11][11], Player *playersPointer)
{
	// Put marker to reflect killed deck on player's field.
	(*playersField)[x][y] = kill;

	// Removing destoyed deck.
	playersPointer->count.totalNumOfPlSquares--;

	// Shortcuts. 
	DamagedCellsCoordinates *killedCell = &(shipToAttack->shotShipDecksCoordinates);
	int *nextDeck = &(shipToAttack->numberOfDestroyedDecks);

	// Saving coordinates of destroyed deck.
	(*killedCell).x[(*nextDeck)] = x;
	(*killedCell).y[(*nextDeck)] = y;

	// Incrementing index to fill next deck coordinates of damaged ship.
	(*nextDeck)++;

	// Stops random square shooting.
	(*shipToAttack).specialTactic = true;
}

// Fills some data on field and cleans info about damaged ship.
void AfterPlayersShipWasKilled(int x, int y, DamagedShipToBeDestroedByAI *shipToAttack,
	SeaCell(*playersField)[11][11], Player *playersPointer)
{
	// Put marker to reflect killed deck on player's field.
	(*playersField)[x][y] = kill;

	// Removing destoyed deck.
	playersPointer->count.totalNumOfPlSquares--;

	// Clean intermidiate data of destroyed ship.
	shipToAttack->numberOfDestroyedDecks = 0;

	// TODO: function, that will set 'marked' to all squares of 
	// 'playersField' near destroyed ship, so AI ignores them. 

	// Allows random square shooting.
	shipToAttack->specialTactic = false;

}