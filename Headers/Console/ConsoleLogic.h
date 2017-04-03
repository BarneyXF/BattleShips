#pragma once

#include "../Logic/MainLogic.h"
#include "../ServerClient/ClientServerPart.h"
#include "../Sound/SoundHeaders.h"
#include <math.h>
#include <random>
#include <iostream> 
#include <string>
#include <ctime>
#include <conio.h>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include "winsock2.h"
#include "Ws2tcpip.h"

// Console colors.
#define MissedColor 27		// Light blue
#define CellsColor 23		// Blue.
#define ShipsColor 26		// Green.
#define DamagedColor 28		// Red.
#define InfoColor 15		// White.
#define FreshShotColor 30	// Yellow

// Different checking bounds.
#define Left_Border '0'
#define Right_BorderOfAlign '1'
#define Right_Border '9'

// Field cells.
#define Free_Cell " ~ "
// Ships deck.
#define Ship_Cell " þ "	
#define Miss_Cell " O "
#define Killed_Cell " X "

enum RepaintMode
{
	fieldMode = 0, advFieldMode = 1, infoMode = 2, playMode = 3, repeatMode = 4
};

enum GameMode
{
	single, server
};

// We need it to separate console and placing and playing functions(contains information to be showed).
enum InformatioForPlayerToBeShowed
{
	currentPlaceAction, currentShotAction, xIs, shotResult, missed, repeat, damage, killing, changePlayer, AIturn,
	xForPlace, yForPlace, placeMode, checking, wrong, wait, clientKilled, clientDamaged, clientMissed,
	clientRepeated, clientShoot, clientWait, serverCreate, wsaError, socketError, getIP, randomPlacing, disconnect
};

void ClearInfoScreen();

void Menu();
void UseRandom(char *random);
void PlayInformation(InformatioForPlayerToBeShowed infoCode, char charToBeShowed);
void Print(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11]);
void RepaintCell(int _x, int _y, char *charToBePainted, RepaintMode mode);
void Repaint(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11], GameMode mode);
void Repaint(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11], sockaddr_in mode);
bool GetNum(char *symbol, char leftBorder, char rightBorder);
void PlacingInformation(InformatioForPlayerToBeShowed infoCode, char charToBeShowed, int numOfDecks, int numOfShips);
void ClientInformation(InformatioForPlayerToBeShowed infoCode, char (*charToGetted)[INET_ADDRSTRLEN], int x, int y);
void PrintShotInfoForPlayer(int x, int y, ShotResult result, SeaCell(*playersField)[11][11],
	SeaCell(*enemyField)[11][11]);
ShipCell ShipsPlaceSelector(int numOfDecks, SeaCell(*playersField)[11][11],
	SeaCell(*enemyField)[11][11], bool *horAlign);
void ShowGhostShip(int x, int y, int numOfDecks, bool horAlign);
char GoThroughMenu(int upBound, int downBound, int leftBound, int rightBound, COORD startCoord);
ShipCell ShipToShootSelector(SeaCell(*playersField)[11][11],
	SeaCell(*enemyField)[11][11]);