#pragma once

#include "../Logic/MainLogic.h"
#include "../ServerClient/ClientServerPart.h"
#include "../Console/ConsoleLogic.h"
#include "winsock2.h"
#include "d3d9.h"
#include "dinput.h"


// Unneeded for graphics funcs.
/*void Menu();
void ClearInfoScreen();
void Repaint(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11], GameMode mode);
void Repaint(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11], sockaddr_in mode);
void Print(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11]);
void RepaintCell(int _x, int _y, char *charToBePainted, RepaintMode mode);*/

// Need to read number or char.
bool GetNum(char *symbol, char leftBorder, char rightBorder);

// Smth like messageBoxes??
void PlayInformation(InformatioForPlayerToBeShowed infoCode, char charToBeShowed);
void PlacingInformation(InformatioForPlayerToBeShowed infoCode, char charToBeShowed, int numOfDecks, int numOfShips);
void ClientInformation(InformatioForPlayerToBeShowed infoCode, char(*charToGetted)[INET_ADDRSTRLEN], int x, int y);