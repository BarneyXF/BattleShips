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

typedef struct
{
	unsigned short totalNumOfPlSqares = 0;
	unsigned short numOf1ShipsPl = 0;
	unsigned short numOf2ShipsPl = 0;
	unsigned short numOf3ShipsPl = 0;
	unsigned short numOf4ShipsPl = 0;
}ShipsCounter;

enum SeaCell
{
	empty = 0, ship = 1, locked = 2, killed = 3, marked = 4
};

enum Stage
{
	menu = 0, placing = 1, playing = 2, end = 3
};

enum ShipType
{
	patrol = 1, destroyer = 2, cruiser = 3, carrier = 4
};

typedef struct
{
	int numOfDecks = 0;
	ShipType type;
} Ship;

void PlacingShips(int, int, SeaCell (*field)[10][10], ShipsCounter(*shipsCounter));
bool PlacingChecker(int, int, SeaCell(*field)[10][10], ShipsCounter(*shipsCounter));
int ShipsDeckCounter(int x, int y, int numX, int numY, SeaCell(*field)[10][10]);
void Repaint();
bool Check(char c);
// TODO: playing stage
void Shooting(int x, int y);
void ShootingChecker();
void Print();

// TODO: make local var
SeaCell playersBattleSea[10][10];
SeaCell (*playersField)[10][10] = &playersBattleSea;
SeaCell enemysBattleSea[10][10];
SeaCell(*enemysField)[10][10] = &enemysBattleSea;
ShipsCounter playersShips, aiShips;
Stage gameStage = menu;

int main()
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
		{
			playersBattleSea[i][j] = empty;
			enemysBattleSea[i][j] = empty;
		}
			
	gameStage = placing;
	srand(time(0));
	// TODO: Write asking from console or AI placing, repair sticking 1 and 2-decks ships
	do
	{
		if (gameStage == playing)
		{
			break;
		}
		int randomX = rand() % 10 + 0;
		int randomY = rand() % 10 + 0;
		PlacingShips(randomX, randomY, playersField, &playersShips);

	} while (playersShips.totalNumOfPlSqares != totalNumOfSqares);
	do
	{
		int randomX = rand() % 10 + 0;
		int randomY = rand() % 10 + 0;
		PlacingShips(randomX, randomY, enemysField, &aiShips);
	} while (aiShips.totalNumOfPlSqares != totalNumOfSqares);
	//playing
	do 
	{
		char xC, yC;
		system("cls");
		Repaint();
		printf("%i\n%i\n%i\n%i\n%i\n", aiShips.numOf1ShipsPl, aiShips.numOf2ShipsPl, aiShips.numOf3ShipsPl, aiShips.numOf4ShipsPl, aiShips.totalNumOfPlSqares);
		do
		{
			xC = _getch();
			if (xC == 27)
			{
				return 0;
			}
		} while (!Check(xC));
		printf("X coordinate is: %c\n", xC);
		do
		{
			yC = _getch();
			if (yC == 27)
			{
				return 0;
			}
		} while (!Check(yC));
		printf("Y coordinate is: %c\n\n", yC);
		int x = xC - '0';
		int y = yC - '0';
		Shooting(x, y);
		printf("Shooting result is: \n");
		printf("Press any keyboard button to continue...\n");		
		_getch();
	} while (true);

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

void PlacingShips(int x, int y, SeaCell (*field)[10][10], ShipsCounter (*shipsCounter))
{
	switch ((*field)[x][y])
	{
		case empty:
		{
			if (PlacingChecker(x, y, field, shipsCounter))
			{
				(*field)[x][y] = ship;
				(*shipsCounter).totalNumOfPlSqares++;
			}
			break;
		}
		case ship:
		{
			// TODO: Error message
			break;
		}
		case locked:
		{
			// TODO: Error message
			break;
		}
	}
	if ((*shipsCounter).totalNumOfPlSqares == totalNumOfSqares)
	{
		gameStage = playing;
	}
}

bool PlacingChecker(int x, int y, SeaCell (*field)[10][10], ShipsCounter (*shipsCounter))
{
	int totalCount = 0;
	bool canPlace = false;

	if ((x > 0) && (y > 0) && (x < 10) && (y < 10))
	{
		for (int i = 0; i <= 2; i++)
		{
			for (int j = 0; j <= 2; j++)
			{
				switch ((*field)[x - 1 + i][y - 1 + j])
				{
				case empty:
				case locked:
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
						totalCount += ShipsDeckCounter(x, y, numX, numY, field);
						canPlace = true;
					}
				}
				}
			}
		}
	}
	else if ((x == 0) && (y == 0))
	{
		for (int i = 0; i <= 1; i++)
		{
			for (int j = 0; j <= 1; j++)
			{
				switch ((*field)[i][j])
				{
				case empty:
				case locked:
				{
					canPlace = true;
					break;
				}
				case ship:
				{
					if (i == j)
					{
						// TODO: Error message
						return false;
					}
					else
					{
						int numX, numY;
						if ((i == 0) && (j > 0))
						{
							numX = 0;
							numY = 1;
						}
						else
						{
							numY = 0;
							if (i > 0)
							{
								numX = 1;
							}
						}
						totalCount += ShipsDeckCounter(x, y, numX, numY, field);
						canPlace = true;
					}
				}
				}
			}
		}
	}
	else if ((x == 10) && (y == 10))
	{
		for (int i = 9; i <= 10; i++)
		{
			for (int j = 9; j <= 10; j++)
			{
				switch ((*field)[i][j])
				{
				case empty:
				case locked:
				{
					canPlace = true;
					break;
				}
				case ship:
				{
					if (i == j)
					{
						// TODO: Error message
						return false;
					}
					else
					{
						int numX, numY;
						if ((i == 10) && (j < 10))
						{
							numX = 0;
							numY = -1;
						}
						else
						{
							numY = 0;
							if (i < 10)
							{
								numX = -1;
							}
						}
						totalCount += ShipsDeckCounter(x, y, numX, numY, field);
						canPlace = true;
					}
				}
				}
			}
		}
	}//
	else if ((x == 10) && (y == 0))
	{
		for (int i = 9; i <= 10; i++)
		{
			for (int j = 0; j <= 1; j++)
			{
				switch ((*field)[i][j])
				{
				case empty:
				case locked:
				{
					canPlace = true;
					break;
				}
				case ship:
				{
					if (abs(i - j) == 7)
					{
						// TODO: Error message
						return false;
					}
					else
					{
						int numX, numY;
						if ((i == 10) && (j > 1))
						{
							numX = 0;
							numY = 1;
						}
						else
						{
							numY = 0;
							if (i < 10)
							{
								numX = -1;
							}
						}
						totalCount += ShipsDeckCounter(x, y, numX, numY, field);
						canPlace = true;
					}
				}
				}
			}
		}

	}//
	else if ((x == 0) && (y == 10))
	{
		for (int i = 0; i <= 1; i++)
		{
			for (int j = 9; j <= 10; j++)
			{
				switch ((*field)[i][j])
				{
				case empty:
				case locked:
				{
					canPlace = true;
					break;
				}
				case ship:
				{
					if (abs(i - j) == 7)
					{
						// TODO: Error message
						return false;
					}
					else
					{
						int numX, numY;
						if ((i == 0) && (j < 10))
						{
							numX = 0;
							numY = -1;
						}
						else
						{
							numY = 0;
							if (i > 0)
							{
								numX = 1;
							}
						}
						totalCount += ShipsDeckCounter(x, y, numX, numY, field);
						canPlace = true;
					}
				}
				}
			}
		}
	}//
	else if ((x > 0) && (y == 0) && (x < 10))
	{
		for (int i = 0; i <= 2; i++)
		{
			for (int j = 0; j <= 1; j++)
			{
				switch ((*field)[x - 1 + i][y + j])
				{
				case empty:
				case locked:
				{
					canPlace = true;
					break;
				}
				case ship:
				{
					if (abs(i - j) == 1)
					{
						// TODO: Error message
						return false;
					}
					else
					{
						int numX, numY;
						if ((i == 1) && (j > 0))
						{
							numX = 0;
							numY = 1;
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
						totalCount += ShipsDeckCounter(x, y, numX, numY, field);
						canPlace = true;
					}
				}
				}
			}
		}
	}
	else if ((x > 0) && (y == 10) && (x < 10))
	{
		for (int i = 0; i <= 2; i++)
		{
			for (int j = 0; j <= 1; j++)
			{
				switch ((*field)[x - 1 + i][y - 1 + j])
				{
				case empty:
				case locked:
				{
					canPlace = true;
					break;
				}
				case ship:
				{
					if (abs(i - j) == 1)
					{
						// TODO: Error message
						return false;
					}
					else
					{
						int numX, numY;
						if ((i == 1) && (j > 0))
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
						totalCount += ShipsDeckCounter(x, y, numX, numY, field);
						canPlace = true;
					}
				}
				}
			}
		}
	}
	else if ((y > 0) && (x == 0) && (y < 10))
	{
		for (int i = 0; i <= 1; i++)
		{
			for (int j = 0; j <= 2; j++)
			{
				switch ((*field)[x + i][y - 1 + j])
				{
				case empty:
				case locked:
				{
					canPlace = true;
					break;
				}
				case ship:
				{
					if (abs(i - j) == 1)
					{
						// TODO: Error message
						return false;
					}
					else
					{
						int numX, numY;
						if ((j == 1) && (i > 0))
						{
							numX = 0;
							numY = 1;
						}
						else
						{
							numY = 0;
							if (j < 1)
							{
								numX = -1;
							}
							else
							{
								numX = 1;
							}
						}
						totalCount += ShipsDeckCounter(x, y, numX, numY, field);
						canPlace = true;
					}
				}
				}
			}
		}
	}
	else if ((y > 0) && (x == 10) && (y < 10))
	{
		for (int i = 0; i <= 1; i++)
		{
			for (int j = 0; j <= 2; j++)
			{
				switch ((*field)[x - 1 + i][y - 1 + j])
				{
					case empty:
					case locked:
					{
						canPlace = true;
						break;
					}
					case ship:
					{
						if (abs(i - j) == 2)
						{
							// TODO: Error message
							return false;
						}
						else
						{
							int numX, numY;
							if ((j == 1) && (i > 0))
							{
								numX = 0;
								numY = -1;
							}
							else
							{
								numY = 0;
								if (j < 1)
								{
									numX = -1;
								}
								else
								{
									numX = 1;
								}
							}
							totalCount += ShipsDeckCounter(x, y, numX, numY, field);
							canPlace = true;
						}
				}
				}
			}
		}
	}
	if ((totalCount > 3) || (!canPlace))
	{
		return false;
	}
	else
	{
		switch (totalCount)
		{
			case 0:
			{
				if ((*shipsCounter).numOf1ShipsPl == numOf1Ships)
				{
					return false;
				}
				(*shipsCounter).numOf1ShipsPl++;
				break;
			}
			case 1:
			{
				if ((*shipsCounter).numOf2ShipsPl == numOf2Ships)
				{
					return false;
				}
				(*shipsCounter).numOf1ShipsPl--;
				(*shipsCounter).numOf2ShipsPl++;
				break;
			}
			case 2:
			{
				if ((*shipsCounter).numOf3ShipsPl == numOf3Ships)
				{
					return false;
				}
				(*shipsCounter).numOf2ShipsPl--;
				(*shipsCounter).numOf3ShipsPl++;
				break;
			}
			case 3:
			{
				if ((*shipsCounter).numOf4ShipsPl == numOf4Ships)
				{
					return false;
				}
				(*shipsCounter).numOf3ShipsPl--;
				(*shipsCounter).numOf4ShipsPl++;
				break;
			}
			default:
			{
				return false;
			}
		}
		return true;
	}
}

int ShipsDeckCounter(int x, int y, int numX, int numY, SeaCell(*field)[10][10])
{
	int counter = 0;
	for (int i = 1; i <= abs(numX * 4) + 1; i++)
	{
		for (int j = 1; j <= abs(numY * 4) + 1; j++)
		{
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
