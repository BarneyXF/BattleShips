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
void ChooseRandomSquare(int(*x), int(*y), SeaCell(*playersField)[11][11]);

bool ContinueAction(int *x, int *y, ShotResult result, SeaCell(*playersField)[11][11],
					Player(*playersPointer), DamagedShipToBeDestroedByAI(*shipToDestroy));
void AfterPlayersShipWasDamaged(int x, int y, DamagedShipToBeDestroedByAI *shipToDestroy,
								SeaCell(*playersField)[11][11], Player *playersPointer);
void AfterPlayersShipWasKilled(int x, int y, DamagedShipToBeDestroedByAI *shipToDestroy,
							   SeaCell(*playersField)[11][11], Player *playersPointer);

void tryDestroyingSecondDeck(int *x, int *y, SeaCell(*playersField)[11][11]);
bool tryChoosingLeft(int *x, int y, SeaCell(*playersField)[11][11]);
bool tryChoosingRight(int *x, int y, SeaCell(*playersField)[11][11]);
bool tryChoosingTop(int x, int *y, SeaCell(*playersField)[11][11]);
bool tryChoosingBottom(int x, int *y, SeaCell(*playersField)[11][11]);

void tacticToDestroyMoreThanTwoDecker(DamagedShipToBeDestroedByAI *shipToDestroy, SeaCell(*playersField)[11][11]);
void HorizontalShot(DamagedShipToBeDestroedByAI *shipToDestroy, SeaCell(*playersField)[11][11]);
void VerticalShot(DamagedShipToBeDestroedByAI *shipToDestroy, SeaCell(*playersField)[11][11]);
void checkLeftShotPossibility(DamagedShipToBeDestroedByAI *shipToDestroy, SeaCell(*playersField)[11][11]);
void checkRightShotPossibility(DamagedShipToBeDestroedByAI *shipToDestroy, SeaCell(*playersField)[11][11]);
void checkTopShotPossibility(DamagedShipToBeDestroedByAI *shipToDestroy, SeaCell(*playersField)[11][11]);
void checkBottomShotPossibility(DamagedShipToBeDestroedByAI *shipToDestroy, SeaCell(*playersField)[11][11]);
