#pragma once
#include "MainLogic.h"



enum sideToShoot
{
	left = 0,
	right = 1,
	top = 2,
	bottom = 3
};



void TurnOfAI(SeaCell(*playersField)[11][11], Player(*playersPointer), DamagedShipToBeDestroedByAI *shipToAttack);

void ChoosingTactic(int *x, int *y, DamagedShipToBeDestroedByAI *shipToAttack, SeaCell(*playersField)[11][11]);

void ChooseRandomSquare(int(*x), int(*y), SeaCell(*playersField)[11][11]);

void TryDestroyingSecondDeck(int *x, int *y, SeaCell(*playersField)[11][11]);
bool TryChoosingLeft(int *x, int y, SeaCell(*playersField)[11][11]);
bool TryChoosingRight(int *x, int y, SeaCell(*playersField)[11][11]);
bool TryChoosingTop(int x, int *y, SeaCell(*playersField)[11][11]);
bool TryChoosingBottom(int x, int *y, SeaCell(*playersField)[11][11]);

void TacticToDestroyMoreThanTwoDecker(DamagedShipToBeDestroedByAI *shipToAttack, SeaCell(*playersField)[11][11]);
void HorizontalShot(DamagedShipToBeDestroedByAI *shipToAttack, SeaCell(*playersField)[11][11]);
void VerticalShot(DamagedShipToBeDestroedByAI *shipToAttack, SeaCell(*playersField)[11][11]);
void CheckLeftShotPossibility(DamagedShipToBeDestroedByAI *shipToAttack, SeaCell(*playersField)[11][11]);
void CheckRightShotPossibility(DamagedShipToBeDestroedByAI *shipToAttack, SeaCell(*playersField)[11][11]);
void CheckTopShotPossibility(DamagedShipToBeDestroedByAI *shipToAttack, SeaCell(*playersField)[11][11]);
void CheckBottomShotPossibility(DamagedShipToBeDestroedByAI *shipToAttack, SeaCell(*playersField)[11][11]);



bool ContinueAction(int *x, int *y, ShotResult result, SeaCell(*playersField)[11][11],
					Player(*playersPointer), DamagedShipToBeDestroedByAI(*shipToAttack));
void AfterPlayersShipWasDamaged(int x, int y, DamagedShipToBeDestroedByAI *shipToAttack,
								SeaCell(*playersField)[11][11], Player *playersPointer);
void AfterPlayersShipWasKilled(int x, int y, DamagedShipToBeDestroedByAI *shipToAttack,
							   SeaCell(*playersField)[11][11], Player *playersPointer);




