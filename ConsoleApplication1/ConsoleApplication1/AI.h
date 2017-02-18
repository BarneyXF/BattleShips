#pragma once

void turnOfAI(SeaCell(*playerField)[11][11], Player(*playersPointer));
void chooseSquare(int(*x), int(*y), SeaCell(*playerField)[11][11]);
bool continueAction(int x, int y, ShotResult result, SeaCell(*playerField)[11][11], Player(*playersPointer));