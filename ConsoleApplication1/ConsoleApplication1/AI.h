#pragma once

void TurnOfAI(SeaCell(*playerField)[11][11], Player(*playersPointer));
void ChooseSquare(int(*x), int(*y), SeaCell(*playerField)[11][11]);
bool ContinueAction(int x, int y, ShotResult result, SeaCell(*playerField)[11][11], Player(*playersPointer));