#include "stdafx.h"
#include <math.h>
#include <random>
#include <iostream> 
#include <ctime>
#include <conio.h>
#include <cstdio>
#include <cstdlib>

// Additional header files.
#include "../Headers/ConsoleApplication1/MainLogic.h"
#include "../Headers/AI/AI.h"

// Main function.
int main()
{	
	// Creating battle fields and players.
	SeaCell playersBattleSea[11][11];
	SeaCell enemysBattleSea[11][11];
	Stage gameStage = menu;
	Player player;
	Player ai;

	// Struct used by AI to save data about attacked ship.
	DamagedShipToBeDestroedByAI shipToAttack;

	// Initializing fields
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			playersBattleSea[i][j] = empty;
			enemysBattleSea[i][j] = empty;
		}
	}

	PlacingShips(&playersBattleSea, &enemysBattleSea, &player, &ai);

	player.count.totalNumOfPlSquares = totalNumOfSqares;
	ai.count.totalNumOfPlSquares = totalNumOfSqares;

	if (Playing(&playersBattleSea, &enemysBattleSea, &player, &ai, &shipToAttack))
	{
		system("cls");
		printf("You win!\n");
	}
	else
	{
		system("cls");
		printf("You loose(fi vam)!\n");
	}
	
	printf("Press any keyboard button to continue...\n");		
	// Waiting for players reaction
	_getch();
    return 0;
}

// Placing ships on the fields.
void PlacingShips(SeaCell(*field)[11][11], SeaCell(*enemysfield)[11][11], Player(*playersPointer), Player(*aisPointer))
{
	int numOfShipsOfType = 4;
	int shipCounter = 0;
	for (int numOfDecks = 1; numOfDecks <= 4; numOfDecks++)
	{
		numOfShipsOfType--;
		for (int j = numOfShipsOfType; j >= 0; j--)
		{
			//char charX, charY;
			Repaint(field, enemysfield);
			printf("\nCurrent actions:\n\nPress \"esc\" to exit or \n");

			/*printf("Enter x coordinate for %i - deck's ship\n", numOfDecks);
			if (!GetNum(&charX, Left_Border, Right_Border))
			{
				return false;
			}

			printf("X coordinate is: %c\n\nEnter y coordinate\n", charX);
			if (!GetNum(&charY, Left_Border, Right_Border))
			{
				return false;
			}

			printf("Y coordinate is: %c\n\n %i - deck's ships num is: %i", charY, numOfDecks, j);
			int x = charX - '0';
			int y = charY - '0';

			printf("\nChoose horizontal(0) or vertical(1) mode\n");
			char charAlign;
			if (!GetNum(&charX, Left_Border, Right_BorderOfAlign))
			{
				return false;
			}

			printf("Checking position, please wait\n");
			int placingMode = charAlign - '0';*/

			srand(time(0));
			int x = rand() % 10 + 0;
			int y = rand() % 10 + 0;
			int placingMode = rand() % 2 + 0;
			if (!Placing(x, y, placingMode, numOfDecks, shipCounter, field, playersPointer))
			{
				j++;
				continue;
			}
			shipCounter++;
		}
	}
	Repaint(field, enemysfield);
	printf("\nCurrent actions:\n\nPress \"esc\" to exit or \n");
	printf("Placed! Please wait for ai's turn.\n");

	// AI placing ships
	numOfShipsOfType = 4;
	shipCounter = 0;
	for (int numOfDecks = 1; numOfDecks <= 4; numOfDecks++)
	{
		numOfShipsOfType--;
		for (int j = numOfShipsOfType; j >= 0; j--)
		{	
			srand(time(0));
			int x = rand() % 10 + 0;
			int y = rand() % 10 + 0;
			int placingMode = rand() % 2 + 0;

			if (!Placing(x, y, placingMode, numOfDecks, shipCounter, enemysfield, aisPointer))
			{
				j++;
				continue;
			}
			shipCounter++;
		}
	}
	Repaint(field, enemysfield);
	printf("\nCurrent actions:\n\nPress \"esc\" to exit or \n");
	printf("\nPress any keyboard button to continue...\n\n");
}

// Placing ships align and fill switching function.
bool Placing(int x, int y, int placingMode, int numOfDecks, int shipCounter, 
			 SeaCell(*field)[11][11], Player(*playersPointer))
{
	// If we have ship with less than 2 decks we don't need to ask alignment.
	if (numOfDecks < 2)
	{
		if (PlacingCheck(x, y, field, playersPointer, numOfDecks, 1, 0))
		{
			FillShipInfo(field, playersPointer, x, y, shipCounter, numOfDecks, 1, 0);
			return true;
		}
		else
		{
			return false;
		}
	}
	switch (placingMode)
	{
		case 0:
		{
			if (PlacingCheck(x, y, field, playersPointer, numOfDecks, 1, 0))
			{
				FillShipInfo(field, playersPointer, x, y, shipCounter, numOfDecks, 1, 0);
			}
			else
			{
				return false;
			}
			break;
		}
		case 1:
		{
			if (PlacingCheck(x, y, field, playersPointer, numOfDecks, 0, 1))
			{
				FillShipInfo(field, playersPointer, x, y, shipCounter, numOfDecks, 0, 1);
			}
			else
			{
				return false;
			}
			break;
		}
	}
	return true;
}

// Filling ship's information in player's "profile".
void FillShipInfo(SeaCell(*field)[11][11], Player(*pointer), int x, int y, int shipsCounter, 
				  int totalNumOfDecks ,int xAugment, int yAugment)
{
	for (int currentNumOfDeck = 0; currentNumOfDeck < totalNumOfDecks; currentNumOfDeck++)
	{
		// Offset from start point(x, y) in direction given by xAugment and yAugment.
		int xOffset = x + currentNumOfDeck * xAugment;
		int yOffset = y + currentNumOfDeck * yAugment;
		(*field)[xOffset][yOffset] = ship;
		(*pointer).ship[shipsCounter].cell.x[currentNumOfDeck] = xOffset;
		(*pointer).ship[shipsCounter].cell.y[currentNumOfDeck] = yOffset;
		(*pointer).ship[shipsCounter].type = (ShipType)totalNumOfDecks;
	}
	switch ((ShipType)(totalNumOfDecks))
	{
		case patrol:
		{
			(*pointer).count.numOf1ShipsPl++;
			(*pointer).ship[shipsCounter].health = 1;
			break;
		}
		case destroyer:
		{
			(*pointer).count.numOf2ShipsPl++;
			(*pointer).ship[shipsCounter].health = 2;
			break;
		}
		case cruiser:
		{
			(*pointer).count.numOf3ShipsPl++;
			(*pointer).ship[shipsCounter].health = 3;
			break;
		}
		case carrier:
		{
			(*pointer).count.numOf4ShipsPl++;
			(*pointer).ship[shipsCounter].health = 4;
			break;
		}
	}					
}

// Ckecking if we can place ship in this coordinates.
bool PlacingCheck(int x, int y, SeaCell(*field)[11][11], Player (*player), int totalNumOfDecks, int xAugment, int yAugment)
{
	bool canPlace = false;
	for (int currentNumOfDeck = 0; currentNumOfDeck < totalNumOfDecks; currentNumOfDeck++)
	{
		// Offset from start point(x, y) in direction given by xAugment and yAugment.
		int xOffset = x + currentNumOfDeck * xAugment;
		int yOffset = y + currentNumOfDeck * yAugment;
		if ((*field)[xOffset][yOffset] == ship)
		{
			return false;
		}
		else
		{
			if ((xOffset > 9) || (yOffset > 9))
			{
				return false;
			}
			// Checking squares around point(x, y).
			for (int squaresX = 0; squaresX < 3; squaresX++)
			{
				for (int squaresY = 0; squaresY < 3; squaresY++)
				{
					switch ((*field)[xOffset - 1 + squaresY][yOffset - 1 + squaresX])
					{
						case empty:
						{
							canPlace = true;
							break;
						}
						case ship:
						{
							return false;
						}
					}
				}
			}
		}
	}
	if (!canPlace)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// Printing fields on screen.
void Print(SeaCell (*field)[11][11], SeaCell (*enemyField)[11][11])
{
	printf("  0 1 2 3 4 5 6 7 8 9\t  0 1 2 3 4 5 6 7 8 9\n");
	for (int j = 0; j < 10; j++)
	{
		printf("%i ", j);
		for (int i = 0; i < 10; i++)
		{
			switch ((*field)[i][j])
			{
				case empty:
				{
					printf(Free_Cell);
					break;
				}
				case ship:
				{
					printf(Ship_Cell);
					break;
				}
				case marked:
				{
					printf(Miss_Cell);
					break;
				}
				case kill:
				{
					printf(Killed_Cell);
					break;
				}
			}		
		}
		printf("\t%i ", j);
		for (int i = 0; i < 10; i++)
		{
			switch ((*enemyField)[i][j])
			{
				case empty:
				{
					printf(Free_Cell);
					break;
				}
				// TODO: CHANGE TO FREE_CELL
				case ship:
				{
					printf(Ship_Cell);
					break;
				}
				case marked:
				{
					printf(Miss_Cell);
					break;
				}
				case kill:
				{
					printf(Killed_Cell);
					break;
				}
			}
		}
		printf("\n");
	}
}

// Printing some additional information.
void Repaint(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11])
{
	system("cls");
	printf("BattleShips: Player vs AI(ip: localhost)\n\n");
	printf("\t Your's field \t\tEnemy's field\n\n");
	Print(field, enemyField);
}

// Main action 
bool Playing(SeaCell(*playersField)[11][11], SeaCell(*enemyField)[11][11], Player(*playersPointer), 
			 Player(*aisPointer), DamagedShipToBeDestroedByAI *shipToAttack)
{
	ShotResult result = none;
	do 
	{
		char charX, charY;
		Repaint(playersField, enemyField);
		printf("\nCurrent actions:\n\nPress \"esc\" to exit or \n");
		printf("Enter x coordinate for shoot\n");
		if (!GetNum(&charX, Left_Border, Right_Border))
		{
			return false;
		}

		printf("X coordinate is: %c\n\nEnter y coordinate\n", charX);
		if (!GetNum(&charY, Left_Border, Right_Border))
		{
			return false;
		}

		printf("Y coordinate is: %c\n\nShot result is: ", charY);
		int x = charX - '0';
		int y = charY - '0';

		result = ShootingChecker(&x, &y, enemyField, aisPointer);
		switch(result)
		{
			case repeatedShot:
			{
				(*enemyField)[x][y] = kill;
				printf("are you serious? You shot at this point already!\n");
				break;
			}
			case miss:
			{
				(*enemyField)[x][y] = marked;
				printf("missed. Lol.\n");
				break;
			}

			case damaged:
			{
				(*enemyField)[x][y] = kill;
				(*aisPointer).count.totalNumOfPlSquares--;
				printf("damaged.\n");
				printf("Your turn again. Press any keyboard button to continue...\n");
				_getch();
				continue;
			}

			case killed:
			{
				(*enemyField)[x][y] = kill;
				(*aisPointer).count.totalNumOfPlSquares--;
				printf("killed! Good job.\n");
				printf("Your turn again. Press any keyboard button to continue...\n");
				_getch();
				continue;
			}
			
		}
		printf("Now AI's turn. Press any keyboard button to continue...\n");
		_getch();

		// AI's turn
		if ((*aisPointer).count.totalNumOfPlSquares > 0)
		{
			TurnOfAI(playersField, playersPointer, shipToAttack, enemyField);
		}
		else
		{
			return true;
		}
	}
	while ((*playersPointer).count.totalNumOfPlSquares > 0);
	
	return false;
}

// Checking shot result.
ShotResult ShootingChecker(int *x, int *y, SeaCell(*field)[11][11], Player(*playersPointer))
{
	ShotResult result = miss;

	if ((*field)[*x][*y] == ship)
	{
		// Accessing to each player's ship.
		for (int numOfPlayersShips = 0; numOfPlayersShips < 10; numOfPlayersShips++)
		{
			// Accessing to each ship's deck.
			for (int numOfDeck = 0; numOfDeck < 4; numOfDeck++)
			{
				result = CompareCoord(x, y, field, playersPointer, numOfPlayersShips, numOfDeck);
				if (result != none)
				{
					return result;
				}
			}
		}
	}
	else if ((*field)[*x][*y] == kill)
	{
		result = repeatedShot;
	}
	return result;
}

// Checking coordinates if we hit ship.
ShotResult CompareCoord(int *x, int *y, SeaCell(*field)[11][11], Player(*playersPointer), int numOfShip, int numOfDeck)
{
	if (((*playersPointer).ship[numOfShip].cell.x[numOfDeck] == *x) && ((*playersPointer).ship[numOfShip].cell.y[numOfDeck] == *y))
	{
		(*playersPointer).ship[numOfShip].health--;

		if ((*playersPointer).ship[numOfShip].health == 0)
		{
			switch ((*playersPointer).ship[numOfShip].type)
			{
				case patrol:
				{
					(*playersPointer).count.numOf1ShipsPl--;
					break;
				}
				case destroyer:
				{
					(*playersPointer).count.numOf2ShipsPl--;
					break;
				}
				case cruiser:
				{
					(*playersPointer).count.numOf3ShipsPl--;
					break;
				}
				case carrier:
				{
					(*playersPointer).count.numOf4ShipsPl--;
					break;
				}
			}
			return killed;
		}
		else
		{
			return damaged;
		}
	}
	return none;
}

// Returns false if user pressed esc, gets pressed button.
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

// Checking if symbol in borders.
bool Check(char symbol, char leftBorder, char rightBorder)
{
	if ((symbol >= leftBorder) && (symbol <= rightBorder))
	{
		return true;
	}
	return false;
}