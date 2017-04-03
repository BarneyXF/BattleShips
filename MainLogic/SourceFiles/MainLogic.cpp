// Header files.
#include "stdafx.h"
#include "../../Headers/Logic/MainLogic.h"
#include "../../Headers/Console/ConsoleLogic.h"
#include "../../Headers/AI/AI.h"
#include "../../Headers/ServerClient/ClientServerPart.h"

/*
*
*
// Logic without any graphics.
*
*
*/

// TODO: MAKE FUNC FOR SWITCHES

// Main function.
int main()
{
	Menu();
    return 0;
}

// Placing ships on the fields.
bool PlacingShips(SeaCell(*field)[11][11], SeaCell(*enemysfield)[11][11], 
			Player(*playersPointer), Player(*aisPointer), char random, bool vsAI)
{
	int numOfShipsOfType = 0;
	int shipCounter = 0;
	if (vsAI)
	{
		Repaint(field, enemysfield, single);
	}
	else
	{
		Repaint(field, enemysfield, server);
	}
	
	switch (random)
	{
	case 'y':
	{
		for (int numOfDecks = 4; numOfDecks >= 1; numOfDecks--)
		{
			numOfShipsOfType++;
			for (int j = numOfShipsOfType; j > 0; j--)
			{
				char charX = 0, charY = 0;
				ClearInfoScreen();
				RepaintCell(0, 15, "", infoMode);
				PlayInformation(currentPlaceAction, numOfDecks + '0');
				bool horAlign;
				ShipCell ship = ShipsPlaceSelector(numOfDecks, field, enemysfield, &horAlign);
				if (ship.x[0] == -1)
				{
					return false;
				}
				char charAlign = '\0';
				if (horAlign)
				{
					charAlign = 'h';
				}
				else
				{
					charAlign = 'v';
				}
				int x = ship.x[0];
				int y = ship.y[0];
				if (!Placing(x, y, charAlign, numOfDecks, shipCounter, field, playersPointer))
				{
					ClearInfoScreen();
					RepaintCell(0, 15, "", infoMode);
					PlacingInformation(wrong, '\0', 0, 0);
					j++;
					continue;
				}
				
				
				for (int i = 0; i < numOfDecks; i++)
				{
					RepaintCell(playersPointer->ship[shipCounter].cell.x[i], playersPointer->ship[shipCounter].cell.y[i], Ship_Cell, fieldMode);
				}
					
				shipCounter++;
			}
		}
		break;
	}
	// Using randomizer.
	case 'n':
	{
		for (int numOfDecks = 4; numOfDecks >= 1; numOfDecks--)
		{
			numOfShipsOfType++;
			for (int j = numOfShipsOfType; j > 0; j--)
			{
				int x = rand() % 10 + 0;
				int y = rand() % 10 + 0;
				int placingMode = rand() % 2 + 0;
				char placingChar;
				switch (placingMode)
				{
					case 0:
					{
						placingChar = 'h';
						break;
					}
					case 1:
					{
						placingChar = 'v';
						break;
					}
				}
				if (!Placing(x, y, placingChar, numOfDecks, shipCounter, field, playersPointer))
				{
					j++;
					continue;
				}
				for (int i = 0; i < numOfDecks; i++)
				{
					RepaintCell(playersPointer->ship[shipCounter].cell.x[i], playersPointer->ship[shipCounter].cell.y[i], Ship_Cell, fieldMode);
				}
				shipCounter++;
			}
		}
		break;
	}
	}

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 40; j++)
			RepaintCell(j, i + 15, " ", infoMode);

	RepaintCell(0, 0, "", infoMode);
	ClearInfoScreen();
	RepaintCell(0, 15, "", infoMode);
	PlacingInformation(wait, '\0', 0, 0);
	if (vsAI)
	{
		// AI placing ships
		numOfShipsOfType = 0;
		shipCounter = 0;
		for (int numOfDecks = 4; numOfDecks >= 1; numOfDecks--)
		{
			numOfShipsOfType++;
			for (int j = numOfShipsOfType; j > 0; j--)
			{
				srand(time(0));
				int x = rand() % 10 + 0;
				int y = rand() % 10 + 0;
				int placingMode = rand() % 2 + 0;
				char placingChar;
				switch (placingMode)
				{
				case 0:
				{
					placingChar = 'h';
					break;
				}
				case 1:
				{
					placingChar = 'v';
					break;
				}
				}
				if (!Placing(x, y, placingChar, numOfDecks, shipCounter, enemysfield, aisPointer))
				{
					j++;
					continue;
				}
				shipCounter++;
			}
		}
		Repaint(field, enemysfield, single);
	}
	else
	{
		Repaint(field, enemysfield, server);
	}
	return true;
}

// Placing ships align and fill switching function.
bool Placing(int x, int y, char placingMode, int numOfDecks, int shipCounter, 
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
		case 'h':
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
		case 'v':
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

// Ckecking if we can place ship in this coordinates.
bool PlacingCheck(int x, int y, SeaCell(*field)[11][11], Player (*player), 
			int totalNumOfDecks, int xAugment, int yAugment)
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
						case emptyCell:
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

// Main action.
bool Playing(SeaCell(*playersField)[11][11], SeaCell(*enemyField)[11][11], Player(*playersPointer), 
			 Player(*enemysPointer), DamagedShipToBeDestroedByAI *shipToAttack, bool vsAI, SOCKET *socket, sockaddr_in *dest_addr)
{
	ShotResult result = none;
	do 
	{
		char charX, charY;
		ClearInfoScreen();
		RepaintCell(0, 15, "", infoMode);
		PlayInformation(currentShotAction, '\0');

		ShipCell ship = ShipToShootSelector(playersField, enemyField);
		int x = ship.x[0];
		int y = ship.y[0];
		if (x == -1)
		{
			return false;
		}
		if (vsAI)
		{
			result = ShootingChecker(&x, &y, enemyField, enemysPointer);
		}
		else
		{
			SendToCheck(x, y, &result, socket, dest_addr);
		}
		ClearInfoScreen();
		RepaintCell(0, 15, "", infoMode);
		PlayInformation(shotResult, '\0');
		switch (result)
		{
			case repeatedShot:
			{
				InitializingSound(missSound);
				PlayInformation(repeat, '\0');
				break;
			}
			case miss:
			{
				InitializingSound(missSound);
				(*enemyField)[x][y] = marked;
				PlayInformation(missed, '\0');
				RepaintCell(x + 12, y, Miss_Cell, playMode);	
				break;
			}

			case damaged:
			{
				InitializingSound(explosionSound);
				(*enemyField)[x][y] = kill;
				(*enemysPointer).count.totalNumOfPlSquares--;
				RepaintCell(x + 12, y, Killed_Cell, playMode);
				RepaintCell(16, 15, "", infoMode);
				PlayInformation(damage, '\0');
				RepaintCell(x + 12, y, Killed_Cell, repeatMode);
				RepaintCell(16, 15, "", infoMode);
				continue;
			}

			case killed:
			{
				InitializingSound(explosionSound);
				(*enemyField)[x][y] = kill;
				(*enemysPointer).count.totalNumOfPlSquares--;
				RepaintCell(x + 12, y, Killed_Cell, playMode);
				RepaintCell(16, 15, "", infoMode);
				PlayInformation(killing, '\0');
				RepaintCell(x + 12, y, Killed_Cell, repeatMode);
				RepaintCell(16, 15, "", infoMode);
				if ((*enemysPointer).count.totalNumOfPlSquares == 0)
				{
					return true;
				}
				continue;
			}
		}

		RepaintCell(0, 16, "", infoMode);
		
		
		if ((*enemysPointer).count.totalNumOfPlSquares > 0)
		{
			if (vsAI)
			{
				// AI's turn
				PlayInformation(AIturn, '\0');
				TurnOfAI(playersField, playersPointer, shipToAttack, enemyField);
			}
			else
			{
				// Enemy's turn
				char s[22];
				ClientInformation(clientWait, &s, 0, 0);
				Timer(5, 14, 18);
				ResultOfTurn isWin = EnemysTurn(playersField, enemyField, playersPointer, enemysPointer, socket, dest_addr, x, y);
				if (isWin == win)
				{
					return true;
				}
				else if (isWin == loose)
				{
					return false;
				}
				else if (isWin == disconnected)
				{
					system("cls");
					PlayInformation(disconnect, '\0');
					return false;
				}
			}
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
	else if ((*field)[*x][*y] != emptyCell)
	{
		result = repeatedShot;
	}
	return result;
}

// Checking coordinates if we hit ship.
ShotResult CompareCoord(int *x, int *y, SeaCell(*field)[11][11], 
			Player(*playersPointer), int numOfShip, int numOfDeck)
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

// Checking if symbol in borders.
bool Check(char symbol, char leftBorder, char rightBorder)
{
	if ((symbol >= leftBorder) && (symbol <= rightBorder))
	{
		return true;
	}
	return false;
}

// Replaces waiting for  "press any key to continue..."
void Timer(int time, int x, int y)
{
	for (int timer = time; timer > 0; timer--)
	{
		char charTime[2];
		charTime[0] = (timer + '0');
		charTime[1] = '\0';
		RepaintCell(x, y, &charTime[0], infoMode);
		Sleep(1005);
		printf("\a");
	}
}