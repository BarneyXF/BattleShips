#include "../../MainLogic/SourceFiles/stdafx.h"
#include <random>
#include <ctime>
#include <iostream>
#include <string>
#include <conio.h>

#include "../../Headers/Console/ConsoleLogic.h"
#include "../../Headers/AI/AI.h"
#include "../../Headers/AI/Tactics.h"
#include "../../Headers/AI/ActionsAfterShot.h"

// Main function controlling AI's turn.
void TurnOfAI(SeaCell(*playersField)[11][11], Player(*playersPointer), DamagedShipToBeDestroedByAI *shipToAttack,
			  SeaCell(*enemyField)[11][11])
{
	srand(time(0));
	ShotResult result = miss;

	// Initializing coordinates with first destroyed deck's coordinates.
	// This way AI will try to shoot around them.
	int x = shipToAttack->shotShipDecksCoordinates.x[0];
	int y = shipToAttack->shotShipDecksCoordinates.y[0];

	// True, if shot was successful.
	bool extraAction = false;

	// Doing shots until miss
	do
	{
		ChoosingTactic(&x, &y, shipToAttack, playersField);
		result = ShootingChecker(&x, &y, playersField, playersPointer);
		extraAction = ContinueAction(&x, &y, result, playersField, playersPointer, shipToAttack);
		PrintShotInfoForPlayer(x, y, result, playersField, enemyField);
	} while (extraAction);
}

// Repaints field and prints AI's shot information.
// PrintShotInfoForPlayer moved to ConsoleLogic