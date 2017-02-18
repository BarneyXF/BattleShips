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

// тип корабл€
enum ShipType
{
	null = 0, patrol = 1, destroyer = 2, cruiser = 3, carrier = 4
};

// состо€ние клетки
enum SeaCell
{
	empty = 0, ship = 1,kill = 2, marked = 3
};

// стади€ игры(не слишком полезно)
enum Stage
{
	menu = 0, placing = 1, playing = 2, end = 3
};

//результат выстрела
enum ShotResult
{
	miss, killed, wounded
};

//информаци€ о количестве кораблей
typedef struct
{
	short totalNumOfPlSqares = 0;
	unsigned short numOf1ShipsPl = 0;
	unsigned short numOf2ShipsPl = 0;
	unsigned short numOf3ShipsPl = 0;
	unsigned short numOf4ShipsPl = 0;
}PlayersShipsCounter;

//информаци€ о положении корабл€
typedef struct
{
	int x[4];
	int y[4];
}ShipCell;

//информаци€ о корабл€х
typedef struct
{
	ShipType type = null;
	ShipCell cell;
	int health = 0;
} Ship;

//¬едет счет кораблей у обеих сторон и содержит местополежни€ и тип каждого
typedef struct
{
	PlayersShipsCounter count;
	Ship ship[10];
}Player;

//функции
void PlacingShips(SeaCell(*field)[11][11], SeaCell(*enemysfield)[11][11], Player(*playersPointer), Player(*aisPointer));
bool PlacingCheck(int x, int y, SeaCell(*field)[11][11], Player (*playersPointer), int numOfDecks, int xP, int yP);
void Repaint(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11]);
bool Check(char c);
ShotResult ShootingChecker(int *x, int *y, SeaCell(*field)[11][11], Player(*playersPointer));
void Print(SeaCell (*field)[11][11], SeaCell(*battleField)[11][11]);
bool Playing(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11], Player(*playersPointer), Player(*aisPointer));
void FillShipInfo(SeaCell(*field)[11][11], Player(*pointer), int x, int y, int counter, int i, int xP, int yP);

int main()
{	
	//создание и инициализаци€ полей и ссылок
	SeaCell playersBattleSea[11][11];
	SeaCell(*playersField)[11][11] = &playersBattleSea;
	SeaCell enemysBattleSea[11][11];
	SeaCell(*enemysField)[11][11] = &enemysBattleSea;
	Stage gameStage = menu;

	//создание и инициализаци€ игроков и ссылок
	Player player, ai;
	Player(*playersPointer) = &player;
	Player(*aisPointer) = &ai;
	// TODO: Menu, fill players and ai's structs
	gameStage = placing;

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
		{
			playersBattleSea[i][j] = empty;
			enemysBattleSea[i][j] = empty;
		}
	//размещаем корабли
	PlacingShips(playersField, enemysField, playersPointer, aisPointer);
	gameStage = playing;
	player.count.totalNumOfPlSqares = 20;
	ai.count.totalNumOfPlSqares = 20;
	//играем
	if (Playing(playersField, enemysField, playersPointer, aisPointer))
	{
		system("cls");
		printf("You win!\n");
	}
	//Shooting(x, y);
	//printf("Shooting result is: \n");
	printf("Press any keyboard button to continue...\n");		
	//_getch();

	_getch();
    return 0;
}

void PlacingShips(SeaCell(*field)[11][11], SeaCell(*enemysfield)[11][11], Player(*playersPointer), Player(*aisPointer))
{
	int numOfPlace = 4;
	int shipCounter = 0;
	for (int i = 1; i <= 4; i++)
	{
		numOfPlace--;
		for (int j = numOfPlace; j >= 0; j--)
		{
		start:
			char xC, yC;
			system("cls");
			Repaint(field, enemysfield);
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
			printf("\nChoose horizontal(0) or vertical(1) mode\n");
			char c;
			do
			{
				c = _getch();
				if (c == 27)
				{
					return;
				}
			} while (!Check(c));
			printf("Checking position, please wait\n");
			int choose = c - '0';
			// TODO: MAKE FUNC
			switch (choose)
			{
				case 0:
				{
					if (PlacingCheck(x, y, field, playersPointer, i, 1, 0))
					{
						FillShipInfo(field, playersPointer, x, y, shipCounter, i, 1, 0);
					}
					else
					{
						goto start;
					}
					break;
				}
				case 1:
				{
					if (PlacingCheck(x, y, field, playersPointer, i, 0, 1))
					{
						FillShipInfo(field, playersPointer, x, y, shipCounter, i, 0, 1);
					}
					else
					{
						goto start;
					}
					break;
				}
				default:
				{
					if (PlacingCheck(x, y, field, playersPointer, i, 0, 1))
					{
						FillShipInfo(field, playersPointer, x, y, shipCounter, i, 0, 1);
					}
					else
					{
						goto start;
					}
					break;
				}
			}
			
			system("cls");		
			Repaint(field, enemysfield);
			printf("Placed! Please wait for ai's turn.\n");
		start1:
			
			srand(time(0));
			int randomX = rand() % 10 + 0;
			int randomY = rand() % 10 + 0;
			int r = rand() % 2 + 0;

			switch (r)
			{
				case 0:
				{
					if (PlacingCheck(randomX, randomY, enemysfield, aisPointer, i, 1, 0))
					{
						FillShipInfo(enemysfield, aisPointer, randomX, randomY, shipCounter, i, 1, 0);
					}
					else
					{
						goto start1;
					}
					break;
				}
				case 1:
				{
					if (PlacingCheck(randomX, randomY, enemysfield, aisPointer, i, 0, 1))
					{
						FillShipInfo(enemysfield, aisPointer, randomX, randomY, shipCounter, i, 0, 1);
					}
					else
					{
						goto start1;
					}
					break;
				}
				default:
				{
					if (PlacingCheck(randomX, randomY, enemysfield, aisPointer, i, 0, 1))
					{
						FillShipInfo(enemysfield, aisPointer, randomX, randomY, shipCounter, i, 0, 1);
					}
					else
					{
						goto start1;
					}
					break;
				}
			}
			shipCounter++;
		}
	}
	system("cls");
	Repaint(field, enemysfield);
	printf("\nPress any keyboard button to continue...\n\n");
}

bool Check(char c)
{
	if ((c >= '0') && (c <= '9'))
	{
		return true;
	}
	return false;
}
// заполнение информации о корабле в "профиль" игрока
void FillShipInfo(SeaCell(*field)[11][11], Player(*pointer), int x, int y, int counter, int i,int xP, int yP)
{
	for (int k = 0; k < i; k++)
	{
		(*field)[x + k * xP][y + k * yP] = ship;
		(*pointer).ship[counter].cell.x[k] = x + k * xP;
		(*pointer).ship[counter].cell.y[k] = y + k * yP;
		(*pointer).ship[counter].type = (ShipType)i;
	}
	switch ((ShipType)(i))
	{
		case patrol:
		{
			(*pointer).count.numOf1ShipsPl++;
			(*pointer).ship[counter].health = 1;
			break;
		}
		case destroyer:
		{
			(*pointer).count.numOf2ShipsPl++;
			(*pointer).ship[counter].health = 2;
			break;
		}
		case cruiser:
		{
			(*pointer).count.numOf3ShipsPl++;
			(*pointer).ship[counter].health = 3;
			break;
		}
		case carrier:
		{
			(*pointer).count.numOf4ShipsPl++;
			(*pointer).ship[counter].health = 4;
			break;
		}
	}					
}
// проверка положени€
bool PlacingCheck(int x, int y, SeaCell(*field)[11][11], Player (*player), int numOfDecks, int xP, int yP)
{
	int totalCount = 0;
	bool canPlace = false;
	for (int k = 0; k < numOfDecks; k++)
	{
		if ((*field)[x + k * xP][y + k * yP] == ship)
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
						case empty:
						{
							canPlace = true;
							break;
						}
						case ship:
						{
							// TODO: Error message
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
//вывод полей на экран
void Print(SeaCell (*field)[11][11], SeaCell (*enemyField)[11][11])
{
	printf(" \tABCDEFGHIG\t \tABCDEFGHIG\n");
	for (int j = 0; j < 10; j++)
	{
		printf("%i\t", j);
		for (int i = 0; i < 10; i++)
		{
			switch ((*field)[i][j])
			{
				case empty:
				{
					printf(".");
					break;
				}
				case ship:
				{
					printf(".");
					break;
				}
				case marked:
				{
					printf("o");
					break;
				}
				case kill:
				{
					printf("X");
					break;
				}
			}		
		}
		printf("\t%i\t", j);
		for (int i = 0; i < 10; i++)
		{
			switch ((*enemyField)[i][j])
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
				case marked:
				{
					printf("o");
					break;
				}
				case kill:
				{
					printf("X");
					break;
				}
			}
		}
		printf("\n");
	}
}
// доп информаци€
void Repaint(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11])
{
	printf("BattleShips: Player vs AI(ip: localhost)\n\n");
	printf("\tYour's field \t\tEnemy's field\n\n");
	Print(field, enemyField);
	printf("\nCurrent actions:\n\nPress \"esc\" to exit or \n");
}
//стади€ игры
bool Playing(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11], Player(*playersPointer), Player(*aisPointer))
{
	do 
	{
	start:
		char xC, yC;
		system("cls");
		Repaint(field, enemyField);
		printf("Enter x coordinate for shoot\n");
		do
		{
			xC = _getch();
			if (xC == 27)
			{
				return false;
			}
		} while (!Check(xC));
		printf("X coordinate is: %c\n\nEnter y coordinate\n", xC);
		do
		{
			yC = _getch();
			if (yC == 27)
			{
				return false;
			}
		} while (!Check(yC));
		printf("Y coordinate is: %c\n\n Shot result is:", yC);
		int x = xC - '0';
		int y = yC - '0';
		ShotResult result = ShootingChecker(&x, &y, enemyField, aisPointer);
		switch(result)
		{
			case miss:
			case marked:
			{
				(*enemyField)[x][y] = marked;
				break;
			}
			case wounded:
			case killed:
			{
				(*enemyField)[x][y] = kill;
				(*aisPointer).count.totalNumOfPlSqares--;
				break;
			}
			
		}
		if ((*aisPointer).count.totalNumOfPlSqares > 0)
		{
			//BOTS TURN(PLACE YOUR CODE HERE)
			srand(time(0));
			int x;
			int y;
			// TODO: check if 1-4 step behind we wonded the ship
			do
			{
				x = rand() % 10 + 0;
				y = rand() % 10 + 0;
			} while (((*field)[x + 10][y] == marked) || (((*field)[x + 10][y] == killed)));

			result = ShootingChecker(&x, &y, field, playersPointer);

			switch (result)
			{
				case miss:
				case marked:
				{
					(*field)[x][y] = marked;
					break;
				}
				case wounded:
				case killed:
				{
					(*field)[x][y] = kill;
					(*playersPointer).count.totalNumOfPlSqares--;
					break;
				}
			}
			//END OF THE BOTS TURN
		}
		else
		{
			return true;
		}
	}while ((*playersPointer).count.totalNumOfPlSqares > 0);
	return false;
}
//проверка попадани€
ShotResult ShootingChecker(int *x, int *y, SeaCell(*field)[11][11], Player(*playersPointer))
{
	//≈сли указанна€ точка - корабль, то ищем его в данных игрока\бота чтобы уменьшить здоровье
	if ((*field)[*x][*y] == ship)
	{
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (((*playersPointer).ship[i].cell.x[j] == *x) && ((*playersPointer).ship[i].cell.y[j] == *y))
				{
					(*playersPointer).ship[i].health--;
					if ((*playersPointer).ship[i].health == 0)
					{
						switch ((*playersPointer).ship[i].type)
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
						return wounded;
					}
				}
			}
		}
	}
	else
	{
		if ((*field)[*x][*y] == kill)
		{
			return killed;
		}
	}
	return miss;
}
