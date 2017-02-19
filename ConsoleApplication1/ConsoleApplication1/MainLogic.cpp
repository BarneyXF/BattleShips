#include "stdafx.h"
#include <math.h>
#include <random>
#include <iostream> 
#include <ctime>
#include <conio.h>
#include <cstdio>
#include <cstdlib>

#include "MainLogic.h"
#include "AI.h"

int main()
{	
	//создание и инициализация полей и ссылок
	SeaCell playersBattleSea[11][11];
	SeaCell(*playersField)[11][11] = &playersBattleSea;
	SeaCell enemysBattleSea[11][11];
	SeaCell(*enemysField)[11][11] = &enemysBattleSea;
	Stage gameStage = menu;

	//создание и инициализация игроков и ссылок
	Player player, ai;
	Player(*playersPointer) = &player;
	Player(*aisPointer) = &ai;
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
	else
	{
		system("cls");
		printf("You loose(fi vam)!\n");
	}
	printf("Press any keyboard button to continue...\n");		

	_getch();
    return 0;
}

void PlacingShips(SeaCell(*field)[11][11], SeaCell(*enemysfield)[11][11], Player(*playersPointer), Player(*aisPointer))
{
	int numOfShipsOfType = 4;
	int shipCounter = 0;
	for (int numOfDecks = 1; numOfDecks <= 4; numOfDecks++)
	{
		numOfShipsOfType--;
		for (int j = numOfShipsOfType; j >= 0; j--)
		{
			char charX, charY;
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

			srand(time(0));
			int x = rand() % 10 + 0;
			int y = rand() % 10 + 0;
			int r = rand() % 2 + 0;

			switch (r) //switch (choose)
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
	printf("Placed! Please wait for ai's turn.\n");

	numOfShipsOfType = 4;
	shipCounter = 0;
	for (int numOfDecks = 1; numOfDecks <= 4; numOfDecks++)
	{
		numOfShipsOfType--;
		for (int j = numOfShipsOfType; j >= 0; j--)
		{	
			srand(time(0));
			int randomX = rand() % 10 + 0;
			int randomY = rand() % 10 + 0;
			int r = rand() % 2 + 0;

			switch (r)
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
// проверка положения
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
					printf(". ");
					break;
				}
				case ship:
				{
					printf("s ");
					break;
				}
				case marked:
				{
					printf("o ");
					break;
				}
				case kill:
				{
					printf("X ");
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
					printf(". ");
					break;
				}
				case ship:
				{
					printf("s ");
					break;
				}
				case marked:
				{
					printf("o ");
					break;
				}
				case kill:
				{
					printf("X ");
					break;
				}
			}
		}
		printf("\n");
	}
}
// доп информация
void Repaint(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11])
{
	printf("BattleShips: Player vs AI(ip: localhost)\n\n");
	printf("\t Your's field \t\tEnemy's field\n\n");
	Print(field, enemyField);
	printf("\nCurrent actions:\n\nPress \"esc\" to exit or \n");
}
//стадия игры
bool Playing(SeaCell(*playerField)[11][11], SeaCell(*enemyField)[11][11], Player(*playersPointer), Player(*aisPointer))
{
	do 
	{
		char charX, charY;
		system("cls");
		Repaint(playerField, enemyField);
		printf("Enter x coordinate for shoot\n");
		do
		{
			charX = _getch();
			if (charX == 27)
			{
				return false;
			}
		} while (!Check(charX));

		printf("X coordinate is: %c\n\nEnter y coordinate\n", charX);
		do
		{
			charY = _getch();
			if (charY == 27)
			{
				return false;
			}
		} while (!Check(charY));

		printf("Y coordinate is: %c\n\n Shot result is:", charY);
		int x = charX - '0';
		int y = charY - '0';

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

		// AI's turn
		if ((*aisPointer).count.totalNumOfPlSqares > 0)
		{
			;
			//BOTS TURN(PLACE YOUR CODE HERE)
			TurnOfAI(playerField, playersPointer);
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
//проверка попадания
ShotResult ShootingChecker(int *x, int *y, SeaCell(*field)[11][11], Player(*playersPointer))
{
	//Если указанная точка - корабль, то ищем его в данных игрока\бота чтобы уменьшить здоровье
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
