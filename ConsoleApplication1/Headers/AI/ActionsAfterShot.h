#pragma once
#include "../ConsoleApplication1/MainLogic.h"

bool ContinueAction(int *x, int *y, ShotResult result, SeaCell(*playersField)[11][11],
					Player(*playersPointer), DamagedShipToBeDestroedByAI(*shipToAttack));

void AfterPlayersShipWasDamaged(int x, int y, DamagedShipToBeDestroedByAI *shipToAttack,
								SeaCell(*playersField)[11][11], Player *playersPointer);
void AfterPlayersShipWasKilled(int x, int y, DamagedShipToBeDestroedByAI *shipToAttack,
							   SeaCell(*playersField)[11][11], Player *playersPointer);


