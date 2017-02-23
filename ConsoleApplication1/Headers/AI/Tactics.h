#pragma once
#include "../ConsoleApplication1/MainLogic.h"

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