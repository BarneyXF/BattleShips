#include "stdafx.h"
#include <math.h>
#include <random>
#include <iostream> 
#include <ctime>
#include <conio.h>
#include <cstdio>
#include <cstdlib>
// Additional libraries.
#include "MainLogic.h"
#include "AI.h"
// Main function
int main()
{	
	// Creating fields and pointers initialization
	SeaCell playersBattleSea[11][11];
	SeaCell(*playersField)[11][11] = &playersBattleSea;
	SeaCell enemysBattleSea[11][11];
	SeaCell(*enemysField)[11][11] = &enemysBattleSea;
	//Stage gameStage = menu;

	// Creating players and pointers initialization 
	Player player, ai;
	Player(*playersPointer) = &player;
	Player(*aisPointer) = &ai;

	//gameStage = placing;

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
		{
			playersBattleSea[i][j] = empty;
			enemysBattleSea[i][j] = empty;
		}
	PlacingShips(playersField, enemysField, playersPointer, aisPointer);
	//gameStage = playing;
	// Total number of players decks.
	player.count.totalNumOfPlSqares = 20;
	ai.count.totalNumOfPlSqares = 20;

	if (Playing(playersField, enemysField, playersPointer, aisPointer))
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
	// Waiting for players reaction.
	_getch();
    return 0;
}
// Function for fill players fields
void PlacingShips(SeaCell(*field)[11][11], SeaCell(*enemysfield)[11][11], Player(*playersPointer), Player(*aisPointer))
{
	// Variable that controlling number of ships of each type.
	int numOfShipsOfType = 4;
	// Number of ships.
	int shipCounter = 0;
	for (int numOfDecks = 1; numOfDecks <= 4; numOfDecks++)
	{
		numOfShipsOfType--;
		for (int j = numOfShipsOfType; j >= 0; j--)
		{
			// TODO: UNCOMMENT
			//char charX, charY;
			//system("cls");
			//Repaint(field, enemysfield);
			/*printf("Enter x coordinate for %i - deck's ship\n", numOfDecks);
			do
			{
				charX = _getch();
				if (charX == 27)
				{
					return;
				}
			} while (!Check(charX));

			printf("X coordinate is: %c\n\nEnter y coordinate\n", charX);
			do
			{
				charY = _getch();
				if (charY == 27)
				{
					return;
				}
			} while (!Check(charY));

			printf("Y coordinate is: %c\n\n %i - deck's ships num is: %i", charY, numOfDecks, j);
			int x = charX - '0';
			int y = charY - '0';

			printf("\nChoose horizontal(0) or vertical(1) mode\n");
			char charAlign;
			do
			{
				charAlign = _getch();
				if (charAlign == 27)
				{
					return;
				}
			} while (!Check(charAlign) || (charAlign > '1'));

			printf("Checking position, please wait\n");
			int choose = charAlign - '0';*/
			// TODO: COMMENT TWO NEXT STRINGS
			system("cls");
			printf("Placing ships, please wait(%i)\n", numOfDecks);
			srand(time(0));
			int x = rand() % 10 + 0;
			int y = rand() % 10 + 0;
			if (numOfDecks > 2)
			{
				if (PlacingCheck(x, y, field, playersPointer, numOfDecks, 1, 0))
				{
					FillShipInfo(field, playersPointer, x, y, shipCounter, numOfDecks, 1, 0);
				}
				else
				{
					j++;
				}
				continue;
			}
			// If 1 then we place ship vertical, 0 - horizontal.
			// TODO: REWRITE TO GETTING FROM CONSOLE
			int placingMode = rand() % 2 + 0;

			switch (placingMode) //switch (choose)
			{
			case 0:
			{
				if (PlacingCheck(x, y, field, playersPointer, numOfDecks, 1, 0))
				{
					FillShipInfo(field, playersPointer, x, y, shipCounter, numOfDecks, 1, 0);
				}
				else
				{
					j++;
					continue;
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
					j++;
					continue;
				}
				break;
			}
			}
		}
	}
	system("cls");
	Repaint(field, enemysfield);
	printf("Placed! Please wait for ai's (he placing ships).\n");
	// AI placing ships
	numOfShipsOfType = 4;
	shipCounter = 0;
	for (int numOfDecks = 1; numOfDecks <= 4; numOfDecks++)
	{
		numOfShipsOfType--;
		for (int j = numOfShipsOfType; j >= 0; j--)
		{	
			// Randomizer of placing coordinates.
			srand(time(0));
			int randomX = rand() % 10 + 0;
			int randomY = rand() % 10 + 0;
			if (numOfDecks > 2)
			{
				if (PlacingCheck(randomX, randomY, enemysfield, aisPointer, numOfDecks, 1, 0))
				{
					FillShipInfo(enemysfield, aisPointer, randomX, randomY, shipCounter, numOfDecks, 1, 0);
				}
				else
				{
					j++;
				}
				continue;
			}
			int placingMode = rand() % 2 + 0;

			switch (placingMode)
			{
				case 0:
				{
					if (PlacingCheck(randomX, randomY, enemysfield, aisPointer, numOfDecks, 1, 0))
					{
						FillShipInfo(enemysfield, aisPointer, randomX, randomY, shipCounter, numOfDecks, 1, 0);
					}
					else
					{
						j++;
						continue;
					}
					break;
				}
				case 1:
				{
					if (PlacingCheck(randomX, randomY, enemysfield, aisPointer, numOfDecks, 0, 1))
					{
						FillShipInfo(enemysfield, aisPointer, randomX, randomY, shipCounter, numOfDecks, 0, 1);
					}
					else
					{
						j++;
						continue;
					}
					break;
				}
			}
			shipCounter++;
		}
	}
	system("cls");
	Repaint(field, enemysfield);
}
// Filling ship's information in player's "profile".
void FillShipInfo(SeaCell(*field)[11][11], Player(*pointer), int x, int y, int shipsNum, int numOfDecks,int xAugment, int yAugment)
{
	// Going through ship and filling it's coordinates and ship type.
	for (int k = 0; k < numOfDecks; k++)
	{
		int xOffset = x + k + xAugment;
		int yOffset = y + k + yAugment;
		(*field)[xOffset][yOffset] = ship;
		(*pointer).ship[shipsNum].cell.x[k] = xOffset;
		(*pointer).ship[shipsNum].cell.y[k] = yOffset;
		(*pointer).ship[shipsNum].type = (ShipType)numOfDecks;
	}
	// Fill player's ships counter.
	switch ((ShipType)(numOfDecks))
	{
		case patrol:
		{
			(*pointer).count.numOf1ShipsPl++;
			(*pointer).ship[shipsNum].health = 1;
			break;
		}
		case destroyer:
		{
			(*pointer).count.numOf2ShipsPl++;
			(*pointer).ship[shipsNum].health = 2;
			break;
		}
		case cruiser:
		{
			(*pointer).count.numOf3ShipsPl++;
			(*pointer).ship[shipsNum].health = 3;
			break;
		}
		case carrier:
		{
			(*pointer).count.numOf4ShipsPl++;
			(*pointer).ship[shipsNum].health = 4;
			break;
		}
	}					
}
// Checking if we can place ship at this(x, y) and another(xP) coordinates
bool PlacingCheck(int x, int y, SeaCell(*field)[11][11], Player (*player), int totalNumOfDecks, int xAugment, int yAugment)
{
	bool canPlace = false;
	
	for (int numOfDecks = 0; numOfDecks < totalNumOfDecks; numOfDecks++)
	{
		// Offset about point(x, y) in sqare 3 * 3.
		int xOffset = x + numOfDecks * xAugment;
		int yOffset = y + numOfDecks * yAugment;
		// If ship or we out of bounds
		if (((*field)[xOffset][yOffset] == ship) || ((xOffset > 9) || (yOffset > 9)))
		{
			return false;
		}
		// Going around point in 3 * 3 area and searching any reasons for stop operation.
		for (int areasX = 0; areasX < 3; areasX++)
			for (int areasY = 0; areasY < 3; areasY++)
				switch ((*field)[xOffset - 1 + areasY][yOffset - 1 + areasX])
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
	if (!canPlace)
	{
		return false;
	}
	return true;
}
// Playing function
bool Playing(SeaCell(*playerField)[11][11], SeaCell(*enemyField)[11][11], Player(*playersPointer), Player(*aisPointer))
{
	ShotResult result = none;
	do 
	{
		// Variables for char's representation of int's x and y.
		char charX, charY;
		// Clears screen.
		system("cls");
		Repaint(playerField, enemyField);
		printf("Enter x coordinate for shoot\n");
		charX = GetNum();
		printf("X coordinate is: %c\n\nEnter y coordinate\n", charX);
		charY = GetNum();
		printf("Y coordinate is: %c\n\n Shot result is:", charY);
		int x = charX - '0';
		int y = charY - '0';

		result = ShootingChecker(&x, &y, enemyField, aisPointer);
		switch(result)
		{
			case miss:
			{
				(*enemyField)[x][y] = marked;
				printf("miss.\n");
				break;
			}
			case wounded:
			{
				(*enemyField)[x][y] = kill;
				printf("got it.\n");
				(*aisPointer).count.totalNumOfPlSqares--;
				// If player kill emeny's ship, then he shoot again.
				continue;
			}
			case killed:
			{
				(*enemyField)[x][y] = kill;
				printf("got it.\n");
				(*aisPointer).count.totalNumOfPlSqares--;
				break;
			}
		}

		// AI's turn (if he has any ship).
		if ((*aisPointer).count.totalNumOfPlSqares > 0)
		{
			// TODO: Add repeating if ai wounded players ship.
			//BOTS TURN(PLACE YOUR CODE HERE)
			turnOfAI(playerField, playersPointer);
			//END OF THE BOTS TURN
		}
		else
		{
			return true;
		}
	}
	while ((*playersPointer).count.totalNumOfPlSqares > 0);
	
	return false;
}
// Printing additional info
void Repaint(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11])
{
	printf("BattleShips: Player vs AI(ip: localhost)\n\n");
	printf("\t Your's field \t\tEnemy's field\n\n");
	Print(field, enemyField);
	printf("\nCurrent actions:\n\nPress \"esc\" to exit or \n");
}
// Printing game fields.
void Print(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11])
{
	printf("  0 1 2 3 4 5 6 7 8 9\t  0 1 2 3 4 5 6 7 8 9\n");
	for (int y = 0; y < 10; y++)
	{
		printf("%i ", y);
		for (int x = 0; x < 10; x++)
		{
			switch ((*field)[x][y])
			{
			case empty:
			{
				printf(Free_Cell);
				break;
			}
			case ship:
			{
				printf(Alive_Ship);
				break;
			}
			case marked:
			{
				printf(Missed_Shot);
				break;
			}
			case kill:
			{
				printf(Killed_Ship);
				break;
			}
			}
		}
		printf("\t%i ", y);
		for (int i = 0; i < 10; i++)
		{
			switch ((*enemyField)[i][y])
			{
			case empty:
			{
				printf(Free_Cell);
				break;
			}
			case ship:
			{
				// TODO: PLACE Alive_Ship WITH Free_Cell
				printf(Alive_Ship);
				break;
			}
			case marked:
			{
				printf(Missed_Shot);
				break;
			}
			case kill:
			{
				printf(Killed_Ship);
				break;
			}
			}
		}
		printf("\n");
	}
}
// Checking if we kill the ship (or it's deck).
ShotResult ShootingChecker(int *x, int *y, SeaCell(*field)[11][11], Player(*playersPointer))
{
	if ((*field)[*x][*y] == ship)
	{
		// Accessing to each ship.
		for (int numOfPlayersShip = 0; numOfPlayersShip < 10; numOfPlayersShip++)
		{
			// Accessing to each deck.
			for (int numOfDeck = 0; numOfDeck < 4; numOfDeck++)
			{
				if (CompareCoord(x, y, field, playersPointer, numOfPlayersShip ,numOfDeck))
				{
					return wounded;
				}
			}
		}
	}
	else if ((*field)[*x][*y] == kill)
	{
		// If we shoot at the cell, that marked as, for example, "killed", then we just continue; 
		return killed;
	}
	return miss;
}
// Trying to find ship's cell in Player's data("profile").
bool CompareCoord(int *x, int *y, SeaCell(*field)[11][11], Player(*playersPointer), int numOfPlayersShip, int numOfDeck)
{
	if (((*playersPointer).ship[numOfPlayersShip].cell.x[numOfDeck] == *x) && ((*playersPointer).ship[numOfPlayersShip].cell.y[numOfDeck] == *y))
	{
		(*playersPointer).ship[numOfPlayersShip].health--;
		// If ship has no health(all cells are marked as "killed") then we decrementing ship's counter in player's data.
		if ((*playersPointer).ship[numOfPlayersShip].health == 0)
		{
			switch ((*playersPointer).ship[numOfPlayersShip].type)
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
			return true;
		}
		else
		{
			return true;
		}
	}
	return false;
}
// Reading number in char variable.
char GetNum()
{
	char num;
	do
	{
		num = _getch();
		// 27 is ESC - button code.
		if (num == 27)
		{
			system("cls");
			printf("You loose(fi vam)!\n");
			printf("Press any keyboard button to continue...\n");
			// Wait for player's reaction.
			_getch();
			exit(0);
		}
	} while (!Check(num));
	return num;
}
// Checking char symbol if it not a number. 
bool Check(char symbol)
{
	if ((symbol >= '0') && (symbol <= '9'))
	{
		return true;
	}
	return false;
}