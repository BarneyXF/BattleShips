#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "../../Headers/Console/ConsoleLogic.h"
#include "../../MainLogic/SourceFiles/stdafx.h"

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// TODO: information, place resources, cursor.

/*
*
*
// Logic to work with console.
*
*
*/

// Main menu of the game.
void Menu()
{
	// Creating battle fields.
	SeaCell playersBattleSea[11][11];
	SeaCell enemysBattleSea[11][11];

	SetConsoleTextAttribute(hConsole, InfoColor);
	// Entering menu.
	do
	{
		// Initializing fields.
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				playersBattleSea[i][j] = emptyCell;
				enemysBattleSea[i][j] = emptyCell;
			}
		}
		std::cout << "\t BattleShips.\n" << std::endl;
		std::cout << " - Play vs AI." << std::endl;
		std::cout << " - Create local server." << std::endl;
		std::cout << " - Connect to local server." << std::endl;
		std::cout << " - Exit." << std::endl;
		COORD coordinates;
		coordinates.X = 0;
		coordinates.Y = 2;
		char choise;
		choise = GoThroughMenu(2, 5, 0, 0, coordinates);
		if (choise == '4')
		{
			system("cls");
			std::cout << "Good Bye! Have a nice day!" << std::endl;
			// Waiting for players reaction
			system("pause");
			return;
		}
		// Creating players.
		Player player;
		int battleResult;
		switch (choise)
		{
			case '1':
			{
				Player ai;
				// Struct used by AI to save data about attacked ship.
				DamagedShipToBeDestroedByAI shipToAttack;
				char random;
				srand(time(0));
				UseRandom(&random);
				if (random == '\0')
				{
					system("cls");
					continue;
				}
				if (!PlacingShips(&playersBattleSea, &enemysBattleSea, &player, &ai, random, true))
				{
					system("cls");
					continue;
				}

				// Initializing number of squares(decks) of each player.
				player.count.totalNumOfPlSquares = totalNumOfSqares;
				ai.count.totalNumOfPlSquares = totalNumOfSqares;
				sockaddr_in k = { 1, 1 };
				SOCKET n;
				if (Playing(&playersBattleSea, &enemysBattleSea, &player, &ai, &shipToAttack, true, &n, &k))
				{
					system("cls");
					SetConsoleTextAttribute(hConsole, ShipsColor);
					std::cout << "You win!!!" << std::endl;
					InitializingSound(winSound);
				}
				else
				{
					system("cls");
					SetConsoleTextAttribute(hConsole, DamagedColor);
					std::cout << "You loose(fi vam)!" << std::endl;
					InitializingSound(looseSound);
				}
				SetConsoleTextAttribute(hConsole, InfoColor);
				break;
			}
			case '2':
			{
				char random = 'n';
				Player enemy;
				system("cls");
				Server(&playersBattleSea, &enemysBattleSea, &player, &enemy, &random, &battleResult);
				break;
			}
			case '3':
			{
				char random = 'n';
				Player enemy;
				Client(&playersBattleSea, &enemysBattleSea, &player, &enemy, &random, &battleResult);
				break;
			}
		}
		if (choise > '1')
		{
			system("cls");
			switch (battleResult)
			{
			case 1:
			{
				SetConsoleTextAttribute(hConsole, ShipsColor);
				std::cout << "You win!!!" << std::endl;
				InitializingSound(winSound);
				break;
			}
			case 0:
			{
				SetConsoleTextAttribute(hConsole, DamagedColor);
				std::cout << "You loose(fi vam)!" << std::endl;
				InitializingSound(looseSound);
				break;
			}
			default:
			{
				SetConsoleTextAttribute(hConsole, InfoColor);
				std::cout << "Connection was lost!" << std::endl;
				InitializingSound(lostConnectionSound);
				break;
			}
			}
		}
		SetConsoleTextAttribute(hConsole, InfoColor);
		//system("cls");
		std::cout << "\nReturn to menu in:" << std::endl;
		Timer(5, 19, 2);
		system("cls");
	} while (true);
}

// We need this func to call from ServerClient project.
void UseRandom(char *random)
{
	system("cls");
	std::cout << "\tBattleShips.\n" << std::endl;
	std::cout << "Do you want to place ships by yourself?" << std::endl;
	std::cout << " - By myself." << std::endl;
	std::cout << " - By randomizer." << std::endl;
	COORD coordinates;
	coordinates.X = 0;
	coordinates.Y = 3;
	char choise = GoThroughMenu(3, 4, 0, 0, coordinates);
	switch (choise)
	{
		case '2':
		{
			*random = 'y';
			break;
		}
		case '3':
		{
			*random = 'n';
			break;
		}
		case 27:
		{
			*random = '\0';
			return;
		}
	}
}

// Printing fields on screen.
void Print(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11])
{
	// Set's color of text in console.
	SetConsoleTextAttribute(hConsole, InfoColor);
	CONSOLE_CURSOR_INFO cursor;
	GetConsoleCursorInfo(hConsole, &cursor);
	cursor.bVisible = false;
	SetConsoleCursorInfo(hConsole, &cursor);
	std::cout << "  0 1 2 3 4 5 6 7 8 9\t  0 1 2 3 4 5 6 7 8 9" << std::endl;
	for (int j = 0; j < 10; j++)
	{
		SetConsoleTextAttribute(hConsole, InfoColor);
		std::cout << j;
		for (int i = 0; i < 10; i++)
		{
			switch ((*field)[i][j])
			{	
				case emptyCell:
				{
					SetConsoleTextAttribute(hConsole, CellsColor);
					std::cout << Free_Cell;				
					break;
				}
				case ship:
				{
					SetConsoleTextAttribute(hConsole, ShipsColor);
					std::cout << Ship_Cell;
					break;
				}
				case marked:
				{
					SetConsoleTextAttribute(hConsole, MissedColor);
					std::cout << Miss_Cell;
					break;
				}
				case kill:
				{
					SetConsoleTextAttribute(hConsole, DamagedColor);
					std::cout << Killed_Cell;
					break;
				}
			}
			std::cout << "\b";
		}
		SetConsoleTextAttribute(hConsole, InfoColor);
		std::cout << "\t" << j;
		for (int i = 0; i < 10; i++)
		{
			SetConsoleTextAttribute(hConsole, InfoColor);
			switch ((*enemyField)[i][j])
			{
				case emptyCell:
				{
					SetConsoleTextAttribute(hConsole, CellsColor);
					std::cout << Free_Cell;				
					break;
				}
				case ship:
				{
					SetConsoleTextAttribute(hConsole, CellsColor);
					std::cout << Ship_Cell;
					break;
				}
				case marked:
				{
					SetConsoleTextAttribute(hConsole, MissedColor);
					std::cout << Miss_Cell;
					break;
				}
				case kill:
				{
					SetConsoleTextAttribute(hConsole, DamagedColor);
					std::cout << Killed_Cell;
					break;
				}
			}
			std::cout << "\b";
		}
		std::cout << std::endl;
	}
	cursor.bVisible = true;
	SetConsoleCursorInfo(hConsole, &cursor);
}

// Printing some additional information.
void Repaint(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11], sockaddr_in address)
{
	SetConsoleTextAttribute(hConsole, InfoColor);
	system("cls");
	std::cout << "\t BattleShips: Player vs Player(ip:" << inet_ntoa(address.sin_addr) << ")\n" << std::endl;
	std::cout << "\t   Your field \t\tEnemy's field\n" << std::endl;
	Print(field, enemyField);
	SetConsoleTextAttribute(hConsole, InfoColor);
}

// Second function for server.
void Repaint(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11], GameMode mode)
{
	SetConsoleTextAttribute(hConsole, InfoColor);
	system("cls");
	if (single)
	{
		std::cout << "\t BattleShips: Player vs AI(ip: localhost)\n" << std::endl;
	}
	else
	{
		std::cout << "\t BattleShips: Player vs AI(ip: localhost(Server))\n" << std::endl;
	}
	std::cout << "\t Your field \t\tEnemy's field\n" << std::endl;
	Print(field, enemyField);
	SetConsoleTextAttribute(hConsole, InfoColor);
}

// Function that reprint's one symbol that get's as parameter. 
void RepaintCell(int _x, int _y, char *charToBePainted, RepaintMode mode)
{
	COORD coordinates;
	switch (mode)
	{
		case fieldMode:
		{
			SetConsoleTextAttribute(hConsole, ShipsColor);
			coordinates.X = _x * 2 + 2;
			coordinates.Y = _y + 5;
			break;
		}
		case advFieldMode:
		{
			SetConsoleTextAttribute(hConsole, CellsColor);
			coordinates.X = _x;
			coordinates.Y = _y;
			break;
		}
		case infoMode:
		{
			SetConsoleTextAttribute(hConsole, InfoColor);
			coordinates.X = _x + 1;
			coordinates.Y = _y;
			break;
		}
		case playMode:
		{
			SetConsoleTextAttribute(hConsole, FreshShotColor);
			coordinates.X = _x * 2 + 2;
			coordinates.Y = _y + 5;
			break;
		}
		case repeatMode:
		{
			SetConsoleTextAttribute(hConsole, DamagedColor);
			coordinates.X = _x * 2 + 2;
			coordinates.Y = _y + 5;
			break;
		}
	}
	
	SetConsoleCursorPosition(hConsole, coordinates);
	std::cout << "\b" << charToBePainted;
	SetConsoleTextAttribute(hConsole, InfoColor);
}

// Function to clear additional info.
void ClearInfoScreen()
{
	for (int i = 0; i < 75; i++)
		for (int j = 0; j < 15; j++)
			RepaintCell(i, j + 15, " ", infoMode);
}

// Returns false if user pressed esc, gets pressed button. 
// Sound func!!!
// Do not forget to place in graphics.
bool GetNum(char *symbol, char leftBorder, char rightBorder)
{
	do
	{		
		*symbol = _getch();
		InitializingSound(menuSound);
		if (*symbol == 27)
		{
			return false;
		}
	} while (!Check(*symbol, leftBorder, rightBorder));
	
	return true;
}
 

// Information for player(like asking him for coordinates etc).
void PlayInformation(InformatioForPlayerToBeShowed infoCode, char charToBeShowed)
{
	switch (infoCode)
	{
		case currentPlaceAction:
		{
			std::cout << "Choose cell to place "<< charToBeShowed <<"-decked ship by \"W,A,S,D\"" << std::endl;
			std::cout << "or arrows and ship's align by \"R\"" << std::endl;
			std::cout << "or press \"ESC\" to exit." << std::endl;
			break;
		}
		case currentShotAction:
		{
			std::cout << "Choose cell to shot opponent's ship by \"W,A,S,D\"" << std::endl;
			std::cout << "or press \"ESC\" to exit." << std::endl;
			break;
		}
		case shotResult:
		{
			std::cout << "Shot result is: ";
			break;
		}
		case missed:
		{
			std::cout << "missed. Lol.\n" << std::endl;
			break;
		}
		case repeat:
		{
			std::cout << "repeat. Are you serious? You shot at this point already!\n" << std::endl;
			break;
		}
		case damage:
		{
			std::cout << "damaged.\n" << std::endl;
			std::cout << "Your turn again." << std::endl;
			std::cout << "Next turn in: ";
			Timer(5, 14, 18);
			break;
		}
		case killing:
		{
			std::cout << "killed! Good job.\n" << std::endl;
			std::cout << "Your turn again." << std::endl;
			std::cout << "Next turn in: ";
			Timer(5, 14, 18);
			break;
		}
		case AIturn:
		{
			std::cout << "\nNow Opponent's turn." << std::endl;
			std::cout << "Next turn in: ";
			Timer(5, 14, 18);
			break;
		}
		case disconnect:
		{
			std::cout << "Connection was interrupted!" << std::endl;
			break;
		}
	}
}

// Information for player on stage of placing ships.
void PlacingInformation(InformatioForPlayerToBeShowed infoCode, char charToBeShowed, int numOfDecks, int numOfShips)
{
	switch (infoCode)
	{
		case xForPlace:
		{
			std::cout << "Enter x coordinate for " << numOfDecks << " - deck's ship" << std::endl;
			break;
		}
		case shotResult:
		{
			std::cout << "X coordinate is: " << charToBeShowed << std::endl;
			std::cout << "\nEnter y coordinate" << std::endl;
			break;
		}
		case yForPlace:
		{
			std::cout << "Y coordinate is: " << charToBeShowed << "\n\n " << numOfDecks << " - deck's ships num is: " << numOfShips;
			break;
		}
		case placeMode:
		{
			std::cout << "\nChoose horizontal(h) or vertical(v) mode(make sure that you use english!!!)" << std::endl;
			break;
		}
		case checking:
		{
			std::cout << "Checking position, please wait." << std::endl;
			break;
		}
		case wrong:
		{
			SetConsoleTextAttribute(hConsole, DamagedColor);
			std::cout << "\nWrong position, please choose another position." << std::endl;
			SetConsoleTextAttribute(hConsole, InfoColor);
			std::cout << "Return in: ";
			// Waiting for players reaction
			Timer(5, 11, 17);
			break;
		}
		case wait:
		{
			std::cout << "\nPlaced! Please wait for Opponent's turn." << std::endl;
			break;
		}
	}
}

// Information for player in multiplayer.
void ClientInformation(InformatioForPlayerToBeShowed infoCode, char (*charToGetted)[INET_ADDRSTRLEN], int x, int y)
{
	switch (infoCode)
	{
		case clientRepeated:
		{
			std::cout << "Your enemy pretty stupid!(" << x << ", " << y << ").\nNow opponent's turn.\n";
			break;
		}
		case clientMissed:
		{
			std::cout << "Your enemy missed!(" << x << ", " << y << ").\nNow opponent's turn.\n";
			break;
		}
		case clientDamaged:
		{
			std::cout << "Your ship placed at " << x << ", " << y << " is damaged!" << std::endl;
			break;
		}
		case clientKilled:
		{
			std::cout << "Your ship placed at " << x << ", " << y << " is killed!" << std::endl;
			break;
		}
		case clientWait:
		{
			std::cout << "Waiting!\nNow opponent's turn.\nNext turn in:";
			break;
		}
		case clientShoot:
		{
			std::cout << "\nWaiting!\nOpponent's turn." << std::endl;
			break;
		}
		case serverCreate:
		{
			std::cout << "Server created! Waiting for players..." << std::endl;
			break;
		}
		case wsaError:
		{
			std::cout << "WSAStarup error: " << x << std::endl;
			break;
		}
		case socketError:
		{
			std::cout << "Socket error. Cann't syncronise stages!" << std::endl;
			break;
		}
		case getIP:
		{
			system("cls");
			std::cout << "Enter ip-address (you can exit with typing \"return\"):" << std::endl;
			//std::cin >> charToGetted;
			scanf("%s", charToGetted);
			break;
		}
	}
}

// Repaints field and prints AI's shot information.
void PrintShotInfoForPlayer(int x, int y, ShotResult result, SeaCell(*playersField)[11][11],
	SeaCell(*enemyField)[11][11])
{
	Repaint(playersField, enemyField, single);

	std::string strResult = "";

	// Convert result to string for output.
	switch (result)
	{
		case miss:
		{
			strResult = "miss";
			break;
		}
		case killed:
		{
			strResult = "killed";
			break;
		}

		case damaged:
		{
			strResult = "damaged";
			break;
		}
	}

	std::cout << std::endl << "AI shoots cell {" << x << ";" << y << "}" << std::endl;
	std::cout << "Result: " << strResult << std::endl << std::endl;
	std::cout << "Next turn in:" << std::endl;
	Timer(5, 14, 19);
}

//
char GoThroughMenu(int upBound, int downBound, int leftBound, int rightBound, COORD startCoord)
{
	COORD coordinates;
	coordinates.X = startCoord.X;
	coordinates.Y = startCoord.Y;
	SetConsoleCursorPosition(hConsole, coordinates);
	do
	{
		char choise = _getch();
		switch (choise)
		{
			case 57:
			case 119:
			case -26:
			case VK_UP:
			{
				if (coordinates.Y > upBound)
					coordinates.Y -= 1;
				break;
			}
			case 53:
			case 115:
			case -21:
			case VK_DOWN:
			{
				if (coordinates.Y < downBound)
					coordinates.Y += 1;
				break;
			}
			case VK_RETURN:
			{
				return (coordinates.Y + '0' - 1);
			}
			case VK_ESCAPE:
			{
				return '4';
			}
			default:
			{
				continue;
			}
		}
		SetConsoleCursorPosition(hConsole, coordinates);

	} while (true);
}

// 
ShipCell ShipsPlaceSelector(int numOfDecks, SeaCell(*playersField)[11][11],
	SeaCell(*enemyField)[11][11], bool *horAlign)
{
	*horAlign = true;
	int upBound = 5, downBound = 14, leftBound = 2, rightBound = 20;
	COORD coordinates;
	coordinates.X = 2;
	coordinates.Y = 5;
	SetConsoleCursorPosition(hConsole, coordinates);
	ShowGhostShip(coordinates.X, coordinates.Y, numOfDecks, *horAlign);
	do
	{
		char choise = _getch();
		switch (choise)
		{
			case 'W':
			case 'w':
			case -26:
			case -106:
			case VK_UP:
			{
				if (coordinates.Y > upBound)
				{
					coordinates.Y -= 1;
				}
				break;
			}
			case 'S':
			case 's':
			case -21:
			case -101:
			case VK_DOWN:
			{
				if (((coordinates.Y < downBound) && *horAlign) || ((coordinates.Y + numOfDecks - 1 < downBound) && !*horAlign))
				{
					coordinates.Y += 1;
				}
				break;
			}
			case 'A':
			case 'a':
			case -28:
			case -108:
			case VK_LEFT:
			{
				if (coordinates.X > leftBound)
				{
					coordinates.X -= 2;
				}
				break;
			}
			case 'D':
			case 'd':
			case -94:
			case -126:
			case VK_RIGHT:
			{
				if (((coordinates.X < rightBound) && !*horAlign) || ((coordinates.X + (numOfDecks * 2) <= rightBound) && *horAlign))
				{
					coordinates.X += 2;
				}
				break;
			}
			case 'R':
			case 'r':
			{
				if (*horAlign)
				{
					*horAlign = false;
					if (coordinates.Y + numOfDecks > downBound)
					{
						coordinates.Y -= coordinates.Y + numOfDecks - downBound - 1;
						SetConsoleCursorPosition(hConsole, coordinates);
					}
				}
				else
				{
					*horAlign = true;
					if (coordinates.X + (numOfDecks * 2) > rightBound)
					{
						coordinates.X -= coordinates.X + (numOfDecks * 2) - rightBound - 2;
						SetConsoleCursorPosition(hConsole, coordinates);
					}
				}
				break;
			}
			case VK_RETURN:
			{
				ShipCell ship;
				ship.x[0] = (coordinates.X - 1) / 2;
				ship.y[0] = coordinates.Y - 5;
				RepaintCell(0, 15, "", infoMode);
				return ship;
			}
			case VK_ESCAPE:
			{
				ShipCell ship;
				ship.x[0] = -1;
				ship.y[0] = -1;
				coordinates.X = 0;
				coordinates.Y = 16;
				SetConsoleCursorPosition(hConsole, coordinates);
				return ship;
			}
			default:
			{
				continue;
			}
		}
		RepaintCell(0, 4, "", infoMode);
		Print(playersField, enemyField);
		SetConsoleCursorPosition(hConsole, coordinates);
		ShowGhostShip(coordinates.X, coordinates.Y, numOfDecks, *horAlign);
	} while (true);
}

//
void ShowGhostShip(int x, int y, int numOfDecks, bool horAlign)
{
	for (int i = 0; i < numOfDecks; i++)
	{
		if (horAlign)
		{
			RepaintCell(x + i * 2, y, Ship_Cell, advFieldMode);
		}
		else
		{
			RepaintCell(x, y + i, Ship_Cell, advFieldMode);
		}
	}
	COORD coordinates;
	coordinates.X = x;
	coordinates.Y = y;
	SetConsoleCursorPosition(hConsole, coordinates);
}

//
ShipCell ShipToShootSelector(SeaCell(*playersField)[11][11],
	SeaCell(*enemyField)[11][11])
{
	int upBound = 5, downBound = 14, leftBound = 26, rightBound = 46;
	COORD coordinates;
	coordinates.X = 26;
	coordinates.Y = 5;
	SetConsoleCursorPosition(hConsole, coordinates);
	do
	{
		char choise = _getch();
		switch (choise)
		{
			case 'W':
			case 'w':
			case -26:
			case -106:
			case VK_UP:
			{
				if (coordinates.Y > upBound)
				{
					coordinates.Y -= 1;
				}
				break;
			}
			case 'S':
			case 's':
			case -21:
			case -101:
			case VK_DOWN:
			{
				if (coordinates.Y < downBound)
				{
					coordinates.Y += 1;
				}
				break;
			}
			case 'A':
			case 'a':
			case -28:
			case -108:
			case VK_LEFT:
			{
				if (coordinates.X > leftBound)
				{
					coordinates.X -= 2;
				}
				break;
			}
			case 'D':
			case 'd':
			case -94:
			case -126:
			case VK_RIGHT:
			{
				if (coordinates.X < rightBound)
				{
					coordinates.X += 2;
				}
				break;
			}
			case VK_RETURN:
			{
				ShipCell ship;
				ship.x[0] = (coordinates.X - leftBound) / 2;
				ship.y[0] = coordinates.Y - upBound;
				RepaintCell(0, 15, "", infoMode);
				return ship;
			}
			case VK_ESCAPE:
			{
				ShipCell ship;
				ship.x[0] = -1;
				ship.y[0] = -1;
				coordinates.X = 0;
				coordinates.Y = 15;
				SetConsoleCursorPosition(hConsole, coordinates);
				return ship;
			}
			default:
			{
				continue;
			}
		}
		RepaintCell(0, 4, "", infoMode);
		Print(playersField, enemyField);
		SetConsoleCursorPosition(hConsole, coordinates);
	} while (true);
}