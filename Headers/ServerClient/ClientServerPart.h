#pragma once
#define PORT 49001
#define CHECKPORT 49002

#include "../Logic/MainLogic.h"
#include "../Console/ConsoleLogic.h"
#include "winsock2.h"
#include "conio.h"
#include "winsock2.h"
#include "windows.h"
#include <iostream>
#include <string.h> 
#include <thread>
#include "time.h"

#pragma comment(lib, "WSock32.lib")
#pragma comment (lib, "mswsock.lib")

enum StageOfTheGame
{
	menu, placing, playing, end, connectRequest, ready, changeCurrentPlayer, sendInformation
};

enum ResultOfTurn
{
	win, loose, nextTurn, disconnected
};

// Indetifyes who is the player(server or client).

enum Identifer
{
	serverID, clientID
};

void Server(SeaCell(*field)[11][11], SeaCell(*enemysfield)[11][11],
Player(*playersPointer), Player(*enemysPointer), char *random, int *battleResult);
void Client(SeaCell(*field)[11][11], SeaCell(*enemysfield)[11][11],
Player(*playersPointer), Player(*enemysPointer), char *random, int *battleResult);
ResultOfTurn EnemysTurn(SeaCell(*field)[11][11], SeaCell(*enemysfield)[11][11],
	Player(*playersPointer), Player(*enemysPointer), SOCKET *myClientSocket, sockaddr_in *dest_addr, int x, int y);
ResultOfTurn SendToCheck(int x, int y, ShotResult *result, SOCKET *socket, sockaddr_in *dest_addr);
void ClientConnectionChecker(char serverIP[], bool *isConnected);
void ServerConnectionChecker(bool *isConnected);