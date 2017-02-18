#include "stdafx.h"
#include <math.h>
#include <random>
#include <iostream> 
#include <ctime>
#include <conio.h>
#include <cstdio>
#include <cstdlib>

const unsigned short numOf4Ships = 1;	
const unsigned short numOf3Ships = 2;	
const unsigned short numOf2Ships = 3;	
const unsigned short numOf1Ships = 4;	
const unsigned short totalNumOfSqares = 20;

enum ShipType
{
	null = 0, patrol = 1, destroyer = 2, cruiser = 3, carrier = 4
};

enum SeaCell
{
	empty = 0, ship = 1, locked = 2, killed = 3, marked = 4
};

enum Stage
{
	menu = 0, placing = 1, playing = 2, end = 3
};

typedef struct
{
	unsigned short totalNumOfPlSqares = 0;
	unsigned short numOf1ShipsPl = 0;
	unsigned short numOf2ShipsPl = 0;
	unsigned short numOf3ShipsPl = 0;
	unsigned short numOf4ShipsPl = 0;
}PlayersShipsCounter;

typedef struct
{
	SeaCell cells[4];
	int x[4];
	int y[4];
}ShipCell;

typedef struct
{
	ShipType type = null;
	ShipCell cell;
} Ship;

typedef struct
{
	PlayersShipsCounter count;
	Ship ship;
}Player;


void PlacingShips(SeaCell(*field)[12][12], SeaCell(*enemysfield)[12][12], Player player, Player ai);
bool PlacingCheck(int x, int y, SeaCell(*field)[12][12], Player(*player), int numOfDecks, int xP, int yP);
void PlacingShips(int, int, SeaCell (*field)[12][12], PlayersShipsCounter(*shipsCounter));//
//bool PlacingChecker(int, int, SeaCell(*field)[12][12], PlayersShipsCounter(*shipsCounter));//
//int ShipsDeckCounter(int x, int y, int numX, int numY, SeaCell(*field)[12][12]);
void Repaint();
bool Check(char c);
// TODO: playing stage
void Shooting(int x, int y);
void ShootingChecker();
void Print();

// TODO: make local var
SeaCell playersBattleSea[12][12];
SeaCell (*playersField)[12][12] = &playersBattleSea;
SeaCell enemysBattleSea[12][12];
SeaCell(*enemysField)[12][12] = &enemysBattleSea;
Stage gameStage = menu;

Player player, ai;

int main()
{			
	gameStage = placing;
	srand(time(0));
	// TODO: Write asking from console or AI placing, repair sticking 1 and 2-decks ships

	PlacingShips(playersField, enemysField, player, ai);
	//Shooting(x, y);
	printf("Shooting result is: \n");
	printf("Press any keyboard button to continue...\n");		
	_getch();

	char c = _getch();
    return 0;
}

bool Check(char c)
{
	if ((c >= '0') && (c <= '9'))
	{
		return true;
	}
	return false;
}

void PlacingShips(SeaCell(*field)[12][12], SeaCell(*enemysfield)[12][12], Player player, Player ai)
{
	int numOfPlace = 4;
	for (int i = 1; i <= 4; i++)
	{
		numOfPlace--;
		for (int j = numOfPlace; j >= 0; j--)
		{
			start:
			char xC, yC;
			system("cls");
			Print();
			printf("Enter x coordinate for %i - deck's ship\n", i);
			do
			{
				xC = _getch();
				if (xC == 27)
				{
					return;
				}
			} while (!Check(xC));
			printf("X coordinate is: %c\n\nEnter y coordinate\n", xC);
			do
			{
				yC = _getch();
				if (yC == 27)
				{
					return;
				}
			} while (!Check(yC));
			printf("Y coordinate is: %c\n\n %i - deck's ships num is: %i", yC, i, j);
			int x = xC - '0';
			int y = yC - '0';
			printf("\nChoose horizontal(0) or vertical(1) mode:\n");
			char c;
			do
			{
				c = _getch();
				if (c == 27)
				{
					return;
				}
			} while (!Check(c));
			int choose = c - '0';

			switch (choose)
			{
				case 0:
				{
					if (PlacingCheck(x, y, field, &player, i, 1, 0))
					{
						for (int k = 0; k < i; k++)
						{
							(*field)[x + k][y] = ship;
							player.ship.cell.x[k] = x + k;
							player.ship.cell.y[k] = y + k;
							player.ship.type = (ShipType)j;
						}
						
					}
					else
					{
						goto start;
					}
					break;
				}
				case 1:
				{
					if(PlacingCheck(x, y, field, &player, i, 0, 1))
					{
						for (int k = 0; k < i; k++)
						{
							(*field)[x][y + k] = ship;
							player.ship.cell.x[k] = x + k;
							player.ship.cell.y[k] = y + k;
						}

					}
					else
					{
						goto start;
					}
					break;
				}
				default:
				{
					if (PlacingCheck(x, y, field, &player, i, 0, 1))
					{
						for (int k = 0; k < i; k++)
						{
							(*field)[x][y + k] = ship;
							player.ship.cell.x[k] = x + k;
							player.ship.cell.y[k] = y + k;
						}

					}
					else
					{
						goto start;
					}
					break;
				}
			}
			start1:
			srand(time(0));
			int randomX = rand() % 10 + 0;
			int randomY = rand() % 10 + 0;
			int r = rand() % 1 + 0;
			
			switch (r)
			{
				case 0:
				{
					if (PlacingCheck(randomX, randomY, enemysField, &ai, i, 1, 0))
					{
						for (int k = 0; k < i; k++)
						{
							(*enemysField)[randomX + k][randomY] = ship;
							ai.ship.cell.x[k] = randomX + k;
							ai.ship.cell.y[k] = randomY + k;
							ai.ship.type = (ShipType)j;
						}

					}
					else
					{
						goto start1;
					}
					break;
				}
				case 1:
				{
					if (PlacingCheck(randomX, randomY, enemysField, &ai, i, 0, 1))
					{
						for (int k = 0; k < i; k++)
						{
							(*enemysField)[randomX + k][randomY] = ship;
							ai.ship.cell.x[k] = randomX + k;
							ai.ship.cell.y[k] = randomY + k;
							ai.ship.type = (ShipType)j;
						}

					}
					else
					{
						goto start1;
					}
					break;
				}
				default:
				{
					if (PlacingCheck(randomX, randomY, enemysField, &ai, i, 0, 1))
					{
						for (int k = 0; k < i; k++)
						{
							(*enemysField)[randomX + k][randomY] = ship;
							ai.ship.cell.x[k] = randomX + k;
							ai.ship.cell.y[k] = randomY + k;
							ai.ship.type = (ShipType)j;
						}

					}
					else
					{
						goto start1;
					}
					break;
				}
			}
		}
	}
}

bool PlacingCheck(int x, int y, SeaCell(*field)[12][12], Player (*player), int numOfDecks, int xP, int yP)
{
	int totalCount = 0;
	bool canPlace = false;
	for (int k = 0; k < numOfDecks; k++)
	{
		if (((*field)[x + k * xP][y + k * yP] == ship) || ((*field)[x + k * xP][y + k * yP] == locked))
		{
			return false;
		}
		else
		{
			if ((x + k * xP > 9) || (y + k * yP > 9))
			{
				return false;
			}
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					switch ((*field)[x - 1 + j + k * xP][y - 1 + i + k * yP])
					{
						case locked:
						case empty:
						{
							canPlace = true;
							break;
						}
						case ship:
						{
							if ((i == j) || (abs(i - j) == 2))
							{
								// TODO: Error message
								return false;
							}
							else
							{
								int numX, numY;
								if ((i == 1) && (j > 1))
								{
									numX = 0;
									numY = 1;
								}
								else if ((i == 1) && (j < 1))
								{
									numX = 0;
									numY = -1;
								}
								else
								{
									numY = 0;
									if (i < 1)
									{
										numX = -1;
									}
									else
									{
										numX = 1;
									}
								}
								canPlace = true;
							}
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

int ShipsDeckCounter(int x, int y, int numX, int numY, SeaCell(*field)[12][12], int numOfDecks)
{
	int counter = 0;
	for (int i = 1; i <= abs(numX * numOfDecks) + 1; i++)
	{
		for (int j = 1; j <= abs(numY * numOfDecks) + 1; j++)
		{
			if ((x + i * numX > 10) || (y + j * numY > 10))
			{
				return -1;
			}
			switch ((*field)[x + i * numX][y + j * numY])
			{
			case ship:
			{
				counter++;
				continue;
			}
			case locked:
			case empty:
			{
				return counter;
			}
			}
		}
	}
	return counter;
}

void Print()
{
	printf(" \tABCDEFGHIG\t \tABCDEFGHIG\n");
	for (int j = 0; j < 10; j++)
	{
		printf("%i\t", j);
		for (int i = 0; i < 10; i++)
		{
			switch (playersBattleSea[i][j])
			{
				case empty:
				{
					printf(".");
					break;
				}
				case ship:
				{
					printf("s");
					break;
				}
				case locked:
				{
					printf("X");
					break;
				}	
			}		
		}
		printf("\t%i\t", j);
		for (int i = 0; i < 10; i++)
		{
			switch (enemysBattleSea[i][j])
			{
			case empty:
			{
				printf(".");
				break;
			}
			case ship:
			{
				printf("s");
				break;
			}
			case locked:
			{
				printf("X");
				break;
			}
			}
		}
		printf("\n");
	}
}

void Repaint()
{
	printf("BattleShips: Player vs AI(ip: localhost)\n\n");
	printf("\tYour's field \t\tEnemy's field\n\n");
	Print();
	printf("\nCurrent actions:\n\nPress \"esc\" to exit or enter x coordinate\n");
}

void Shooting(int x, int y)
{

}

void ShootingChecker()
{

}
