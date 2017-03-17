#pragma once

#include "../Logic/MainLogic.h"
#include "../ServerClient/ClientServerPart.h"
#include <math.h>
#include <random>
#include <iostream> 
#include <ctime>
#include <conio.h>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include "winsock2.h"
#include "Ws2tcpip.h"

// Console colors.
#define MissedColor 8	// Grey.
#define CellsColor 9	// Blue.
#define ShipsColor 10	// Green.
#define DamagedColor 12 // Red.
#define InfoColor 15	// White.

// Different checking bounds.
#define Left_Border '0'
#define Right_BorderOfAlign '1'
#define Right_Border '9'

// Field cells.
#define Free_Cell "~ "
// Ships deck.
#define Ship_Cell "þ "	
#define Miss_Cell "O "
#define Killed_Cell "X "

enum RepaintMode
{
	fieldMode, infoMode, playMode
};

enum GameMode
{
	single, server
};

// We need it to separate console and placing and playing functions(contains information to be showed).
enum InformatioForPlayerToBeShowed
{
	currentAction, xCoordinate, xIs, yIs, missed, repeat, damage, killing, changePlayer, AIturn, AIInfo,
	xForPlace, yForPlace, placeMode, checking, wrong, wait, clientKilled, clientDamaged, clientMissed,
	clientRepeated, clientShoot, clientWait, serverCreate, wsaError, socketError, getIP, randomPlacing, disconnect
};

void ClearInfoScreen();

void Menu();
void PlayInformation(InformatioForPlayerToBeShowed infoCode, char charToBeShowed);
void Print(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11]);
void RepaintCell(int _x, int _y, char *charToBePainted, RepaintMode mode);
void Repaint(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11], GameMode mode);
void Repaint(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11], sockaddr_in mode);
bool GetNum(char *symbol, char leftBorder, char rightBorder);
void PlacingInformation(InformatioForPlayerToBeShowed infoCode, char charToBeShowed, int numOfDecks, int numOfShips);
void ClientInformation(InformatioForPlayerToBeShowed infoCode, char (*charToGetted)[INET_ADDRSTRLEN], int x, int y);