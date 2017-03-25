#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "../../Headers/ServerClient/ClientServerPart.h"

// TODO: timer of turn(haven't idea how realize that without threads, so for now, we haven't it)

// Server's logic func.
void Server(SeaCell(*field)[11][11], SeaCell(*enemysfield)[11][11],
	Player(*playersPointer), Player(*enemysPointer), char *random, int *battleResult)
{
	// Initialization started.

	int connection;
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

	// First step - synconizing.
	char buffer[10];

	buffer[0] = placing;

	connection = send(socket, buffer, sizeof(buffer), 0);
	if (connection == -1)
	{
		PlayInformation(disconnect, '\0');
		return;
	}
	UseRandom(random);

	if (!PlacingShips(field, enemysfield, playersPointer, enemysPointer, *random, false))
	{
		PlayInformation(disconnect, '\0');
		return;
	}

	// Wait for enemy's ready.
	connection = recv(socket, buffer, sizeof(buffer), 0);
	if (connection == -1)
	{
		PlayInformation(disconnect, '\0');
		return;
	}
	buffer[0] = playing;
	connection = send(socket, buffer, sizeof(buffer), 0);
	if (connection == -1)
	{
		return;
	}
	// Wait for enemy's ready.
	connection = recv(socket, buffer, sizeof(buffer), 0);
	if (connection == -1)
	{
		PlayInformation(disconnect, '\0');
		return;
	}
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
	closesocket(socket);
	WSACleanup();
	return;
}

// Client's logic func.
void Client(SeaCell(*field)[11][11], SeaCell(*enemysfield)[11][11],
	Player(*playersPointer), Player(*enemysPointer), char *random, int *battleResult)
{
	int connection;
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
	if (strcmp(serverIP, "return") == 0)
	{
		return;
	}
	else if ((strcmp(serverIP, "localhost") == 0) || (strcmp(serverIP, "LOCALHOST") == 0))
	{
		anAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	}
	else
	{
		anAddr.sin_addr.S_un.S_addr = inet_addr(serverIP);
	}

	connect(clientSocket, (struct sockaddr *)&anAddr, sizeof(struct sockaddr));

	// First step - syncronizing.
	char buffer[10];
	connection = recv(clientSocket, buffer, sizeof(buffer), 0);
	if (connection == -1)
	{
		PlayInformation(disconnect, '\0');
		return;
	}
	if (buffer[0] != placing)
	{
		ClientInformation(socketError, '\0', 0, 0);
		return;
	}
	else
	{
		int stime = (unsigned int)time(NULL) / 2;
		Repaint(field, enemysfield, anAddr);
		UseRandom(random);
		srand(stime);
		if (!PlacingShips(field, enemysfield, playersPointer, enemysPointer, *random, false))
		{
			return;
		}
	}
	// We ready.
	buffer[0] = ready;
	connection = send(clientSocket, buffer, sizeof(buffer), 0);
	if (connection == -1)
	{
		PlayInformation(disconnect, '\0');
		return;
	}
	// Stage - playing.
	connection = recv(clientSocket, buffer, sizeof(buffer), 0);
	if (connection == -1)
	{
		PlayInformation(disconnect, '\0');
		return;
	}
	if (buffer[0] != playing)
	{
		ClientInformation(socketError, '\0', 0, 0);
		return;
	}
	else
	{
		buffer[0] = ready;
		connection = send(clientSocket, buffer, sizeof(buffer), 0);
		if (connection == -1)
		{
			PlayInformation(disconnect, '\0');
			return;
		}
		(*playersPointer).count.totalNumOfPlSquares = totalNumOfSqares;
		(*enemysPointer).count.totalNumOfPlSquares = totalNumOfSqares;
		DamagedShipToBeDestroedByAI shipToAttack;
		Repaint(field, enemysfield, anAddr);
		// First turn of server.
		RepaintCell(0, 15, "", infoMode);
		ClearInfoScreen();
		RepaintCell(0, 15, "", infoMode);
		ClientInformation(clientShoot, '\0', 0, 0);
		connection = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (connection == -1)
		{
			PlayInformation(disconnect, '\0');
			return;
		}
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
					InitializingSound(missSound);
					break;
				}
				case miss:
				{

					(*field)[x][y] = marked;
					ClientInformation(clientMissed, '\0', x, y);
					RepaintCell(x, y, Miss_Cell, playMode);
					InitializingSound(missSound);
					break;
				}

				case damaged:
				{
					(*field)[x][y] = kill;
					(*playersPointer).count.totalNumOfPlSquares--;
					RepaintCell(x, y, Killed_Cell, playMode);
					RepaintCell(0, 15, "", infoMode);
					ClientInformation(clientDamaged, '\0', x, y);
					InitializingSound(explosionSound);
					break;
				}

				case killed:
				{
					(*field)[x][y] = kill;
					(*playersPointer).count.totalNumOfPlSquares--;
					RepaintCell(x, y, Killed_Cell, playMode);
					RepaintCell(0, 15, "", infoMode);
					ClientInformation(clientKilled, '\0', x, y);
					InitializingSound(explosionSound);
					break;
				}
			}
			//Send info back.
			buffer[3] = result;
			connection = send(clientSocket, buffer, sizeof(buffer), 0);
			if (connection == -1)
			{
				PlayInformation(disconnect, '\0');
				return;
			}
			// Waiting for our turn.
			connection = recv(clientSocket, buffer, sizeof(buffer), 0);
			if (connection == -1)
			{
				PlayInformation(disconnect, '\0');
				return;
			}
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

	buffer[0] = changeCurrentPlayer;
	int connection = send((*s1), buffer, sizeof(buffer), 0);
	if (connection == -1)
	{
		PlayInformation(disconnect, '\0');
		return disconnected;
	}
	// Get coordinates for checking
	connection = recv((*s1), buffer, sizeof(buffer), 0);
	if (connection == -1)
	{
		PlayInformation(disconnect, '\0');
		return disconnected;
	}
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
				InitializingSound(missSound);
				break;
			}
			case miss:
			{

				(*field)[x][y] = marked;
				ClientInformation(clientMissed, '\0', x, y);
				RepaintCell(x, y, Miss_Cell, playMode);
				InitializingSound(missSound);
				break;
			}

			case damaged:
			{
				(*field)[x][y] = kill;
				(*playersPointer).count.totalNumOfPlSquares--;
				RepaintCell(x, y, Killed_Cell, playMode);
				RepaintCell(0, 15, "", infoMode);
				ClientInformation(clientDamaged, '\0', x, y);
				InitializingSound(explosionSound);
				break;
			}

			case killed:
			{
				(*field)[x][y] = kill;
				(*playersPointer).count.totalNumOfPlSquares--;
				RepaintCell(x, y, Killed_Cell, playMode);
				RepaintCell(0, 15, "", infoMode);
				ClientInformation(clientKilled, '\0', x, y);
				InitializingSound(explosionSound);
				if ((*playersPointer).count.totalNumOfPlSquares == 0)
				{
					buffer[0] = end;
				}
				break;
			}
		}
		//Send info back.
		buffer[3] = result;
		connection = send((*s1), buffer, sizeof(buffer), 0);
		if (connection == -1)
		{
			PlayInformation(disconnect, '\0');
			return disconnected;
		}
		if (buffer[0] == end)
		{
			return loose;
		}
		// Waiting for our turn.
		connection = recv((*s1), buffer, sizeof(buffer), 0);
		if (connection == -1)
		{
			PlayInformation(disconnect, '\0');
			return disconnected;
		}
	} while (buffer[0] != changeCurrentPlayer);

	return nextTurn;
}

// Sending and receiving info of enemy's shot.
void SendToCheck(int x, int y, ShotResult *result, SOCKET *s1, sockaddr_in *client_addr)
{
	char buffer[10];

	buffer[0] = sendInformation;
	buffer[1] = x;
	buffer[2] = y;
	int connection = send((*s1), buffer, sizeof(buffer), 0);
	if (connection == -1)
	{
		PlayInformation(disconnect, '\0');
		return;
	}
	int client_addr_size = sizeof(*client_addr);

	connection = recv((*s1), buffer, sizeof(buffer), 0);
	if (connection == -1)
	{
		PlayInformation(disconnect, '\0');
		return;
	}
	int res = buffer[3];
	*result = (ShotResult)res;
}