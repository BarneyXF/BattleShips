#pragma once

void TurnOfAI(SeaCell(*playerField)[11][11], SeaCell(*specialPlayersFieldForAI)[11][11],
			  Player(*playersPointer), bool *specialTactic);
void ChooseRandomSquare(int(*x), int(*y), SeaCell(*specialPlayersFieldForAI)[11][11]);
bool ContinueAction(int *x, int *y, ShotResult result, SeaCell(*specialPlayersFieldForAI)[11][11],
					Player(*playersPointer), SeaCell(*playerField)[11][11], bool *specialTactic);
void ShootingTacticChange(int *x, int *y, SeaCell(*specialPlayersFieldForAI)[11][11],
						  Player(*playersPointer));
void tacticChoiceToDestroy3Or4Decker();