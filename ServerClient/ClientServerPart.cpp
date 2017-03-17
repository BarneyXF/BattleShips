#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "../Headers/ServerClient/ClientServerPart.h"

// TODO: timer of turn, do not wait for enemy's end of turn(press any key)

// True if connected.
bool isConnected;

// Server's logic func.
void Server(SeaCell(*field)[11][11], SeaCell(*enemysfield)[11][11],
	Player(*playersPointer), Player(*enemysPointer), char *random, int *battleResult)
{
	// Initialization started.

	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(1, 1), &WSAData) != 0)
	{
		ClientInformation(wsaError, '\0', WSAGetLastError(), 0);
		return;
	}
	SOCKET listeningSocket;
	if (INVALID_SOCKET == (listeningSocket = socket(AF_INET, SOCK_STREAM, 0)))
	{
		ClientInformation(wsaError, '\0', WSAGetLastError(), 0);
		return;
	}
	struct sockaddr_in addrIn;
	addrIn.sin_family = AF_INET;
	addrIn.sin_port = htons(PORT);
	addrIn.sin_addr.s_addr = INADDR_ANY;

	// Initalization ended.
	ClientInformation(serverCreate, '\0', 0, 0);

	bind(listeningSocket, (LPSOCKADDR)&addrIn, sizeof(addrIn));
	listen(listeningSocket, 100);
	sockaddr_in addrFrom;
	int addrFromLength = sizeof(addrFrom);
	
	// Connect.
	SOCKET socket = accept(listeningSocket, (struct sockaddr*)&addrFrom, &addrFromLength);
	isConnected = true;
	// Creating thread for checking connnection with client.
	std::thread checkingThread(ServerConnectionChecker, &isConnected);
	checkingThread.detach();

	// First step - synconizing.
	char buffer[10];

	buffer[0] = placing;
	send(socket, buffer, sizeof(buffer), 0);

	if (!PlacingShips(field, enemysfield, playersPointer, enemysPointer, *random, false))
	{
		return;
	}

	// Wait for enemy's ready.
	recv(socket, buffer, sizeof(buffer), 0);
	buffer[0] = playing;
	send(socket, buffer, sizeof(buffer), 0);
	// Wait for enemy's ready.
	recv(socket, buffer, sizeof(buffer), 0);
	if (buffer[0] == ready)
	{
		(*playersPointer).count.totalNumOfPlSquares = totalNumOfSqares;
		(*enemysPointer).count.totalNumOfPlSquares = totalNumOfSqares;
		DamagedShipToBeDestroedByAI shipToAttack;

		if (Playing(field, enemysfield, playersPointer, enemysPointer, &shipToAttack, false, &socket, &addrFrom))
		{
			*battleResult = 1; // win
		}
		else
		{
			*battleResult = 0; // loose
		}
	}
	//checkingThread.join();
	closesocket(socket);
	WSACleanup();
	return;
}

// Client's logic func.
void Client(SeaCell(*field)[11][11], SeaCell(*enemysfield)[11][11],
	Player(*playersPointer), Player(*enemysPointer), char *random, int *battleResult)
{
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(1, 1), &WSAData) != 0)
	{
		ClientInformation(wsaError, '\0', WSAGetLastError(), 0);
		return;
	}

	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in anAddr;
	anAddr.sin_family = AF_INET;
	anAddr.sin_port = htons(PORT);

	char serverIP[INET_ADDRSTRLEN];

	ClientInformation(getIP, &serverIP, 0, 0);
	anAddr.sin_addr.S_un.S_addr = inet_addr(serverIP);

	connect(clientSocket, (struct sockaddr *)&anAddr, sizeof(struct sockaddr));
	isConnected = true;
	// Create thread for checking connection with server.
	std::thread checkingThread(ClientConnectionChecker, serverIP, &isConnected);
	checkingThread.detach();

	// First step - syncronizing.
	char buffer[10];
	recv(clientSocket, buffer, sizeof(buffer), 0);
	if (buffer[0] != placing)
	{
		ClientInformation(socketError, '\0', 0, 0);
		return;
	}
	else
	{
		int stime = (unsigned int)time(NULL) / 2;
		Repaint(field, enemysfield, anAddr);
		//Sleep(100);
		srand(stime);
		if (!PlacingShips(field, enemysfield, playersPointer, enemysPointer, *random, false))
		{
			return;
		}
	}
	// We ready.
	buffer[0] = ready;
	send(clientSocket, buffer, sizeof(buffer), 0);
	// Stage - playing.
	recv(clientSocket, buffer, sizeof(buffer), 0);
	if (buffer[0] != playing)
	{
		ClientInformation(socketError, '\0', 0, 0);
		return;
	}
	else
	{
		buffer[0] = ready;
		send(clientSocket, buffer, sizeof(buffer), 0);
		(*playersPointer).count.totalNumOfPlSquares = totalNumOfSqares;
		(*enemysPointer).count.totalNumOfPlSquares = totalNumOfSqares;
		DamagedShipToBeDestroedByAI shipToAttack;
		Repaint(field, enemysfield, anAddr);
		// First turn of server.
		RepaintCell(0, 15, "", infoMode);
		ClearInfoScreen();
		RepaintCell(0, 15, "", infoMode);
		ClientInformation(clientShoot, '\0', 0, 0);
		recv(clientSocket, buffer, sizeof(buffer), 0);
		do
		{
			int x = buffer[1];
			int y = buffer[2];

			ShotResult result = ShootingChecker(&x, &y, field, playersPointer);
			switch (result)
			{
				case repeatedShot:
				{
					ClientInformation(clientRepeated, '\0', x, y);
					break;
				}
				case miss:
				{

					(*field)[x][y] = marked;
					ClientInformation(clientMissed, '\0', x, y);
					RepaintCell(x, y, Miss_Cell, playMode);
					break;
				}

				case damaged:
				{
					(*field)[x][y] = kill;
					(*playersPointer).count.totalNumOfPlSquares--;
					RepaintCell(x, y, Killed_Cell, playMode);
					RepaintCell(0, 15, "", infoMode);
					ClientInformation(clientDamaged, '\0', x, y);
					break;
				}

				case killed:
				{
					(*field)[x][y] = kill;
					(*playersPointer).count.totalNumOfPlSquares--;
					RepaintCell(x, y, Killed_Cell, playMode);
					RepaintCell(0, 15, "", infoMode);
					ClientInformation(clientKilled, '\0', x, y);
					break;
				}
			}
			//Send info back.
			buffer[3] = result;
			send(clientSocket, buffer, sizeof(buffer), 0);
			// Waiting for our turn.
			recv(clientSocket, buffer, sizeof(buffer), 0);
		} while (buffer[0] != changeCurrentPlayer);

		if (Playing(field, enemysfield, playersPointer, enemysPointer, &shipToAttack, false, &clientSocket, &anAddr))
		{
			*battleResult = 1; // win
		}
		else
		{
			*battleResult = 0; // loose
		}
	}
	//checkingThread.join();
	closesocket(clientSocket);
	WSACleanup();
	return;
}

// Give's turn to enemy.
ResultOfTurn EnemysTurn(SeaCell(*field)[11][11], SeaCell(*enemysfield)[11][11],
	Player(*playersPointer), Player(*enemysPointer), SOCKET *s1, sockaddr_in *dest_addr, int x, int y)
{
	// Clearing info screen.

	RepaintCell(0, 15, "", infoMode);
	ClearInfoScreen();
	RepaintCell(0, 15, "", infoMode);
	ClientInformation(clientShoot, '\0', 0, 0);

	char buffer[10];

	if (!isConnected)
	{
		return disconnected;
	}

	buffer[0] = changeCurrentPlayer;
	send((*s1), buffer, sizeof(buffer), 0);

	// Get coordinates for checking
	int client_addr_size = sizeof(dest_addr);
	recv((*s1), buffer, sizeof(buffer), 0);
	do
	{
		int x = buffer[1];
		int y = buffer[2];

		ShotResult result = ShootingChecker(&x, &y, field, playersPointer);
		switch (result)
		{
			case repeatedShot:
			{
				ClientInformation(clientRepeated, '\0', x, y);
				//PlayInformation(repeat, '\0');
				break;
			}
			case miss:
			{

				(*field)[x][y] = marked;
				ClientInformation(clientMissed, '\0', x, y);
				//PlayInformation(missed, '\0');
				RepaintCell(x, y, Miss_Cell, playMode);
				break;
			}

			case damaged:
			{
				(*field)[x][y] = kill;
				(*playersPointer).count.totalNumOfPlSquares--;
				RepaintCell(x, y, Killed_Cell, playMode);
				RepaintCell(0, 15, "", infoMode);
				ClientInformation(clientDamaged, '\0', x, y);
				//PlayInformation(damage, '\0');
				break;
			}

			case killed:
			{
				(*field)[x][y] = kill;
				(*playersPointer).count.totalNumOfPlSquares--;
				RepaintCell(x, y, Killed_Cell, playMode);
				RepaintCell(0, 15, "", infoMode);
				ClientInformation(clientKilled, '\0', x, y);
				if ((*playersPointer).count.totalNumOfPlSquares == 0)
				{
					buffer[0] = end;
					//return false;
				}
				//PlayInformation(killing, '\0');
				break;
			}
		}
		//Send info back.
		buffer[3] = result;
		send((*s1), buffer, sizeof(buffer), 0);
		if (buffer[0] == end)
		{
			return loose;
		}
		// Waiting for our turn.
		int client_addr_size = sizeof(dest_addr);
		recv((*s1), buffer, sizeof(buffer), 0);
	} while (buffer[0] != changeCurrentPlayer);

	return nextTurn;
}

// Sending and receiving info of enemy's shot.
ResultOfTurn SendToCheck(int x, int y, ShotResult *result, SOCKET *s1, sockaddr_in *client_addr)
{
	char buffer[10];

	if (!isConnected)
	{
		return disconnected;
	}

	buffer[0] = sendInformation;
	buffer[1] = x;
	buffer[2] = y;
	send((*s1), buffer, sizeof(buffer), 0);

	int client_addr_size = sizeof(*client_addr);

	int size = recv((*s1), buffer, sizeof(buffer), 0);
	
	int res = buffer[3];
	*result = (ShotResult)res;
}

// Sends pakages for cheking connection every 5-10 secs and disconnect if there is not package for 30 secs.
void ServerConnectionChecker(bool *isConnected)
{
	char checkBuffer[5];


	SOCKET listeningSocket;
	if (INVALID_SOCKET == (listeningSocket = socket(AF_INET, SOCK_STREAM, 0)))
	{
		ClientInformation(wsaError, '\0', WSAGetLastError(), 0);
		return;
	}
	struct sockaddr_in addrIn;
	addrIn.sin_family = AF_INET;
	addrIn.sin_port = htons(CHECKPORT);
	addrIn.sin_addr.s_addr = INADDR_ANY;

	bind(listeningSocket, (LPSOCKADDR)&addrIn, sizeof(addrIn));
	listen(listeningSocket, 100);
	sockaddr_in addrFrom;
	int addrFromLength = sizeof(addrFrom);

	// Connect.
	SOCKET socket = accept(listeningSocket, (struct sockaddr*)&addrFrom, &addrFromLength);

	do
	{
		Sleep(5000);
		checkBuffer[0] = 1;
		checkBuffer[1] = 2;
		checkBuffer[2] = 0;
		send(socket, checkBuffer, sizeof(checkBuffer), 0);
		checkBuffer[0] = 0;
		checkBuffer[1] = 1;
		checkBuffer[2] = 2;
		recv(socket, checkBuffer, sizeof(checkBuffer), 0);
		if (!(checkBuffer[0] == 2) || !(checkBuffer[1] == 0) || !(checkBuffer[2] == 1))
		{
			printf("Connection error!\n");
			*isConnected = false;
		}
	} while (*isConnected);
}

// Check's connection with server every 5 seconds.
void ClientConnectionChecker(char serverIP[], bool *isConnected)
{
	char checkBuffer[5];

	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in anAddr;
	anAddr.sin_family = AF_INET;
	anAddr.sin_port = htons(CHECKPORT);
	anAddr.sin_addr.S_un.S_addr = inet_addr(serverIP);
	connect(clientSocket, (struct sockaddr *)&anAddr, sizeof(struct sockaddr));

	do
	{
		Sleep(5000);
		recv(clientSocket, checkBuffer, sizeof(checkBuffer), 0);
		if (!(checkBuffer[0] == 1) || !(checkBuffer[1] == 2) || !(checkBuffer[2] == 0))
		{
			printf("Connection error!\n");
			*isConnected = false;
		}
		checkBuffer[0] = 2;
		checkBuffer[1] = 0;
		checkBuffer[2] = 1;
		send(clientSocket, checkBuffer, sizeof(checkBuffer), 0);
	} while (*isConnected);
}