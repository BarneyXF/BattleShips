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
				char charX, charY;
				ClearInfoScreen();
				RepaintCell(0, 15, "", infoMode);
				PlacingInformation(currentAction, '\0', 0, 0);
				PlacingInformation(xForPlace, '\0', numOfDecks, 0);
				if (!GetNum(&charX, Left_Border, Right_Border))
				{
					return false;
				}
				PlacingInformation(yIs, charX, 0, 0);
				if (!GetNum(&charY, Left_Border, Right_Border))
				{
					return false;
				}
				PlacingInformation(yForPlace, charY, numOfDecks, j);
				int x = charX - '0';
				int y = charY - '0';

				
				char charAlign = '\0';
				// If we have ship with less than 2 decks we don't need to ask alignment.

				if (numOfDecks > 1)
				{
					PlacingInformation(placeMode, '\0', 0, 0);
					do
					{
						if (!GetNum(&charAlign, 'A', 'z'))
						{
							return false;
						}
						if ((charAlign == 'H') || (charAlign == 'V'))
						{
							charAlign += 32;
						}
					} while ((charAlign != 'v') && (charAlign != 'h'));
				}
				
				PlacingInformation(checking, '\0', 0, 0);

				if (!Placing(x, y, charAlign, numOfDecks, shipCounter, field, playersPointer))
				{
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

				if (!Placing(x, y, placingMode, numOfDecks, shipCounter, enemysfield, aisPointer))
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
		//Repaint(playersField, enemyField);
		ClearInfoScreen();
		RepaintCell(0, 14, "", infoMode);
		PlayInformation(currentAction, '\0');
		PlayInformation(xCoordinate, '\0');
		if (!GetNum(&charX, Left_Border, Right_Border))
		{
			return false;
		}
		PlayInformation(xIs, charX);
		if (!GetNum(&charY, Left_Border, Right_Border))
		{
			return false;
		}
		PlayInformation(yIs, charY);
		int x = charX - '0';
		int y = charY - '0';

		if (vsAI)
		{
			result = ShootingChecker(&x, &y, enemyField, enemysPointer);
		}
		else
		{
			ResultOfTurn isConnect = SendToCheck(x, y, &result, socket, dest_addr);
			if (isConnect == disconnect)
			{
				system("cls");
				PlayInformation(disconnect, '\0');
				return false;
			}
			//SendInfoForCheck(x, y, &result, socket, dest_addr);
		}
		switch (result)
		{
			case repeatedShot:
			{
				PlayInformation(repeat, '\0');
				break;
			}
			case miss:
			{

				(*enemyField)[x][y] = marked;
				PlayInformation(missed, '\0');
				RepaintCell(x + 12, y, Miss_Cell, playMode);
				break;
			}

			case damaged:
			{
				(*enemyField)[x][y] = kill;
				(*enemysPointer).count.totalNumOfPlSquares--;
				RepaintCell(x + 12, y, Killed_Cell, playMode);
				RepaintCell(16, 23, "", infoMode);
				PlayInformation(damage, '\0');
				continue;
			}

			case killed:
			{
				(*enemyField)[x][y] = kill;
				(*enemysPointer).count.totalNumOfPlSquares--;
				RepaintCell(x + 12, y, Killed_Cell, playMode);
				RepaintCell(16, 23, "", infoMode);
				PlayInformation(killing, '\0');
				if ((*enemysPointer).count.totalNumOfPlSquares == 0)
				{
					return true;
				}
				continue;
			}
		}

		RepaintCell(0, 25, "", infoMode);
		PlayInformation(AIturn, '\0');
		
		if ((*enemysPointer).count.totalNumOfPlSquares > 0)
		{
			if (vsAI)
			{
				// AI's turn
				TurnOfAI(playersField, playersPointer, shipToAttack, enemyField);
			}
			else
			{
				// Enemy's turn
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
	else if ((*field)[*x][*y] != empty)
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