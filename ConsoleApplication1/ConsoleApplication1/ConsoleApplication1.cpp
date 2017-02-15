#include "stdafx.h"
#include <math.h>
#include <random>
#include <iostream> 
#include <ctime>

const unsigned short numOf4Ships = 1;	unsigned short numOf4ShipsPl = 0;
const unsigned short numOf3Ships = 2;	unsigned short numOf3ShipsPl = 0;
const unsigned short numOf2Ships = 3;	unsigned short numOf2ShipsPl = 0;
const unsigned short numOf1Ships = 4;	unsigned short numOf1ShipsPl = 0;
const unsigned short totalNumOfSqares = 20;	unsigned short totalNumOfPlSqares = 0;

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

void PlacingShips(int, int);
bool PlacingChecker(int, int);
int ShipsDeckCounter(int x, int y, int numX, int numY);
// TODO: playing stage
void Shooting();
void ShootingChecker();
void Print();

// TODO: make local var
SeaCell battleSea[10][10];
Stage gameStage = menu;

int main()
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			battleSea[i][j] = empty;
	gameStage = placing;
	srand(time(0));
	// TODO: Write asking from console or AI placing
	do
	{
		if (gameStage == playing)
		{
			break;
		}
		int randomX = rand() % 10 + 0;
		int randomY = rand() % 10 + 0;
		PlacingShips(randomX, randomY);
	} while (totalNumOfPlSqares != totalNumOfSqares);
	Print();
	printf("\n%i\n%i\n%i\n%i\n\n%i", numOf1ShipsPl, numOf2ShipsPl, numOf3ShipsPl, numOf4ShipsPl, totalNumOfPlSqares);
	char c = getc(stdin);
    return 0;
}

void PlacingShips(int x, int y)
{
	switch (battleSea[x][y])
	{
		case empty:
		{
			if (PlacingChecker(x, y))
			{
				battleSea[x][y] = ship;
				totalNumOfPlSqares++;
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
	if (totalNumOfPlSqares == totalNumOfSqares)
	{
		gameStage = playing;
	}
}

bool PlacingChecker(int x, int y)
{
	int totalCount = 0;
	bool canPlace = false;

	if ((x > 0) && (y > 0) && (x < 10) && (y < 10))
	{
		for (int i = 0; i <= 2; i++)
		{
			for (int j = 0; j <= 2; j++)
			{
				switch (battleSea[x - 1 + i][y - 1 + j])
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
						totalCount += ShipsDeckCounter(x, y, numX, numY);
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
				switch (battleSea[i][j])
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
						totalCount += ShipsDeckCounter(x, y, numX, numY);
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
				switch (battleSea[i][j])
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
						totalCount += ShipsDeckCounter(x, y, numX, numY);
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
				switch (battleSea[i][j])
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
						totalCount += ShipsDeckCounter(x, y, numX, numY);
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
				switch (battleSea[i][j])
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
						totalCount += ShipsDeckCounter(x, y, numX, numY);
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
				switch (battleSea[x - 1 + i][y + j])
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
						totalCount += ShipsDeckCounter(x, y, numX, numY);
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
				switch (battleSea[x - 1 + i][y - 1 + j])
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
						totalCount += ShipsDeckCounter(x, y, numX, numY);
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
				switch (battleSea[x + i][y - 1 + j])
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
						totalCount += ShipsDeckCounter(x, y, numX, numY);
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
				switch (battleSea[x - 1 + i][y - 1 + j])
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
							totalCount += ShipsDeckCounter(x, y, numX, numY);
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
				if (numOf1ShipsPl == numOf1Ships)
				{
					return false;
				}
				numOf1ShipsPl++;
				break;
			}
			case 1:
			{
				if (numOf2ShipsPl == numOf2Ships)
				{
					return false;
				}
				numOf1ShipsPl--;
				numOf2ShipsPl++;
				break;
			}
			case 2:
			{
				if (numOf3ShipsPl == numOf3Ships)
				{
					return false;
				}
				numOf2ShipsPl--;
				numOf3ShipsPl++;
				break;
			}
			case 3:
			{
				if (numOf4ShipsPl == numOf4Ships)
				{
					return false;
				}
				numOf3ShipsPl--;
				numOf4ShipsPl++;
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

int ShipsDeckCounter(int x, int y, int numX, int numY)
{
	int counter = 0;
	for (int i = 1; i <= abs(numX * 4) + 1; i++)
	{
		for (int j = 1; j <= abs(numY * 4) + 1; j++)
		{
			switch (battleSea[x + i * numX][y + j * numY])
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
	printf(" \tABCDEFGHIG\n");
	for (int j = 0; j < 10; j++)
	{
		printf("%i\t", j + 1);
		for (int i = 0; i < 10; i++)
		{
			switch (battleSea[i][j])
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

void Shooting()
{

}

void ShootingChecker()
{

}
