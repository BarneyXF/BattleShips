#include "../../Headers/Console/ConsoleLogic.h"
#include "../../Logic/stdafx.h"

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

/*
*
*
// Logic to work with console.
*
*
*/
// Main menu of the game.	Console func.
void Menu()
{
	//COORD bufferSize = { 40, 50 };
	//SetConsoleScreenBufferSize(hConsole, bufferSize);
	
	// Creating battle fields.
	SeaCell playersBattleSea[11][11];
	SeaCell enemysBattleSea[11][11];

	Stage gameStage;

	

	SetConsoleTextAttribute(hConsole, InfoColor);
	// Entering menu.
	do
	{
		// Initializing fields.
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				playersBattleSea[i][j] = empty;
				enemysBattleSea[i][j] = empty;
			}
		}
		gameStage = menu;
		printf("\t BattleShips.\n\n");
		//printf("Press one of this buttons to choose option:\n\n");
		printf(" 1 ) Play vs AI.\n");
		printf(" 2 ) Play vs another player.\n");
		printf("esc) Exit.\n");
		char choise;
		if (!GetNum(&choise, '1', '2'))
		{
			system("cls");
			printf("Good Bye! Have a nice day!\nPress any keyboard button to continue...\n");
			// Waiting for players reaction
			_getch();
			return;
		}
		// Creating players.
		Player player;

		if (choise == '1')
		{
			Player ai;
			// Struct used by AI to save data about attacked ship.
			DamagedShipToBeDestroedByAI shipToAttack;

			system("cls");
			printf("\tBattleShips.\n\nDo you want to place ships by yourself(if no, we will use randomizer)?\n(y/n)\n");
			char random;
			do
			{
				if (!GetNum(&random, 'n', 'y'))
				{
					system("cls");
					printf("Good Bye! Have a nice day!\nPress any keyboard button to continue...\n");
					// Waiting for players reaction
					_getch();
					return;
				}
			} while ((random != 'n') && (random != 'y'));

			//gameStage = 
			if (!PlacingShips(&playersBattleSea, &enemysBattleSea, &player, &ai, random))
			{
				system("cls");
				continue;
			}

			// Initializing number of squares(decks) of each player.
			player.count.totalNumOfPlSquares = totalNumOfSqares;
			ai.count.totalNumOfPlSquares = totalNumOfSqares;

			if (Playing(&playersBattleSea, &enemysBattleSea, &player, &ai, &shipToAttack))
			{
				system("cls");
				SetConsoleTextAttribute(hConsole, ShipsColor);
				printf("You win!\n");

			}
			else
			{
				system("cls");
				SetConsoleTextAttribute(hConsole, DamagedColor);
				printf("You loose(fi vam)!\n");
			}
			SetConsoleTextAttribute(hConsole, InfoColor);
			printf("\nPress any keyboard button to continue...\n");
			// Waiting for players reaction
			_getch();
			system("cls");
		}
		else
		{
			Player enemy; //we need to get this info from player
						  //play vs player(ask ip etc)
		}
	} while (true);
}

// Printing fields on screen.	Console func.
void Print(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11])
{
	// Set's color of text in console.
	SetConsoleTextAttribute(hConsole, InfoColor);
	printf("  0 1 2 3 4 5 6 7 8 9\t  0 1 2 3 4 5 6 7 8 9\n");
	for (int j = 0; j < 10; j++)
	{
		SetConsoleTextAttribute(hConsole, InfoColor);
		printf("%i ", j);
		for (int i = 0; i < 10; i++)
		{
			switch ((*field)[i][j])
			{	
			case empty:
			{
				SetConsoleTextAttribute(hConsole, CellsColor);
				printf(Free_Cell);
				break;
			}
			case ship:
			{
				SetConsoleTextAttribute(hConsole, ShipsColor);
				printf(Ship_Cell);
				break;
			}
			case marked:
			{
				SetConsoleTextAttribute(hConsole, MissedColor);
				printf(Miss_Cell);
				break;
			}
			case kill:
			{
				SetConsoleTextAttribute(hConsole, DamagedColor);
				printf(Killed_Cell);
				break;
			}
			}
		}
		SetConsoleTextAttribute(hConsole, InfoColor);
		printf("\t%i ", j);
		for (int i = 0; i < 10; i++)
		{
			SetConsoleTextAttribute(hConsole, InfoColor);
			switch ((*enemyField)[i][j])
			{

			case empty:
			{
				SetConsoleTextAttribute(hConsole, CellsColor);
				printf(Free_Cell);
				break;
			}
			case ship:
			{
				// TODO: CHANGE TO FREE_CELL
				SetConsoleTextAttribute(hConsole, CellsColor);
				printf(Free_Cell);
				break;
			}
			case marked:
			{
				SetConsoleTextAttribute(hConsole, MissedColor);
				printf(Miss_Cell);
				break;
			}
			case kill:
			{
				SetConsoleTextAttribute(hConsole, DamagedColor);
				printf(Killed_Cell);
				break;
			}
			}
		}
		printf("\n");
	}
}

// Printing some additional information.	Console func.
void Repaint(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11])
{
	SetConsoleTextAttribute(hConsole, InfoColor);
	system("cls");
	printf("\t BattleShips: Player vs AI(ip: localhost)\n\n");
	printf("\t Your field \t\tEnemy's field\n\n");
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
		case infoMode:
		{
			SetConsoleTextAttribute(hConsole, InfoColor);
			coordinates.X = _x;
			coordinates.Y = _y;
			break;
		}
		case playMode:
		{
			SetConsoleTextAttribute(hConsole, DamagedColor);
			coordinates.X = _x * 2 + 2;
			coordinates.Y = _y + 5;
			break;
		}
	}
	
	SetConsoleCursorPosition(hConsole, coordinates);
	printf("%s", charToBePainted);
	SetConsoleTextAttribute(hConsole, InfoColor);
}

// Function to clear additional info.
void ClearInfoScreen()
{
	for (int i = 0; i < 60; i++)
		for (int j = 0; j < 11; j++)
			RepaintCell(i, j + 15, " ", infoMode);
}

// Returns false if user pressed esc, gets pressed button.	Console func.
bool GetNum(char *symbol, char leftBorder, char rightBorder)
{
	do
	{
		*symbol = _getch();
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
	case currentAction:
	{
		printf("\nCurrent actions:\n\nPress \"esc\" to exit or \n");
		break;
	}
	case xCoordinate:
	{
		printf("Enter x coordinate for shoot\n");
		break;
	}
	case xIs:
	{
		printf("X coordinate is: %c\n\nEnter y coordinate\n", charToBeShowed);
		break;
	}
	case yIs:
	{
		printf("Y coordinate is: %c\nShot result is: ", charToBeShowed);
		break;
	}
	case missed:
	{
		printf("missed. Lol.\n");
		break;
	}
	case repeat:
	{
		printf("are you serious? You shot at this point already!\n");
		break;
	}
	case damage:
	{
		printf("damaged.\n");
		printf("Your turn again. Press any keyboard button to continue...\n");
		_getch();
		break;
	}
	case killing:
	{
		printf("killed! Good job.\n");
		printf("Your turn again. Press any keyboard button to continue...\n");
		_getch();
		break;
	}
	case AIturn:
	{
		printf("Now AI's turn. Press any keyboard button to continue...\n");
		_getch();
		break;
	}
	case AIInfo:
	{
		break;
	}
	}
}

// Information for player on stage of placing ships.
void PlacingInformation(InformatioForPlayerToBeShowed infoCode, char charToBeShowed, int numOfDecks, int numOfShips)
{
	switch (infoCode)
	{
	case currentAction:
	{
		printf("Current actions:\nPress \"esc\" to exit or \n");
		break;
	}
	case xForPlace:
	{
		printf("Enter x coordinate for %i - deck's ship\n", numOfDecks);
		break;
	}
	case yIs:
	{
		printf("X coordinate is: %c\n\nEnter y coordinate\n", charToBeShowed);
		break;
	}
	case yForPlace:
	{
		printf("Y coordinate is: %c\n\n %i - deck's ships num is: %i", charToBeShowed, numOfDecks, numOfShips);
		break;
	}
	case placeMode:
	{
		printf("\nChoose horizontal(0) or vertical(1) mode\n");
		break;
	}
	case checking:
	{
		printf("Checking position, please wait\n");
		break;
	}
	case wrong:
	{
		SetConsoleTextAttribute(hConsole, DamagedColor);
		printf("\nWrong position, please choose another position.\n");
		SetConsoleTextAttribute(hConsole, InfoColor);
		printf("Press any keyboard button to continue...\n");
		// Waiting for players reaction
		_getch();
		break;
	}
	case wait:
	{
		printf("\nPlaced! Please wait for ai's turn.\n");
		break;
	}
	}
}
