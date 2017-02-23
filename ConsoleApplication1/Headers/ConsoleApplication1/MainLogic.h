#pragma once

#define Left_Border '0'
#define Right_BorderOfAlign '1'
#define Right_Border '9'
#define Free_Cell ". "
#define Ship_Cell "s "
#define Miss_Cell "o "
#define Killed_Cell "x "

const unsigned short numOf4Ships = 1;
const unsigned short numOf3Ships = 2;
const unsigned short numOf2Ships = 3;
const unsigned short numOf1Ships = 4;
const unsigned short totalNumOfSqares = 20;

const int lowerFieldBound = 0;
const int upperFieldBound = 9;

enum ShipType
{
	null = 0, patrol = 1, destroyer = 2, cruiser = 3, carrier = 4
};

enum SeaCell
{
	empty, ship, kill, marked
};

enum Stage
{
	menu = 0, placing = 1, playing = 2, end = 3
};

enum ShotResult
{
	miss = 0, killed = 1, damaged = 2, none = 3, repeatedShot = 4
};

typedef struct
{
	short totalNumOfPlSquares = 0;
	unsigned short numOf1ShipsPl = 0;
	unsigned short numOf2ShipsPl = 0;
	unsigned short numOf3ShipsPl = 0;
	unsigned short numOf4ShipsPl = 0;
}PlayersShipsCounter;

typedef struct
{
	int x[4];
	int y[4];
}ShipCell;

typedef struct
{
	ShipType type = null;
	ShipCell cell;
	int health = 0;
} Ship;

typedef struct
{
	PlayersShipsCounter count;
	Ship ship[10];
}Player;



/*
 *
 * BEGIN: STUFF THAT AI NEEDS
 *
 */
// Coordinates chosen according to AI's tactic. Next shot attacks this cell.
typedef struct
{
	int x;
	int y;

}Coordinates;

// Keeps data about location of destroyed decks of currently attacked ship.
typedef struct
{
	int x[4];
	int y[4];

}DamagedCellsCoordinates;

// Gives AI an ability to deliberatly destroy damaged ship.
typedef struct
{
	// If 'true' stops AI's random shooting to make 
	// deliberate choice of next square to shoot.
	bool specialTactic = false;

	Coordinates coordinatesToShoot;

	DamagedCellsCoordinates shotShipDecksCoordinates;

	// Is needed to choose 'shotShipDecksCoordinates' index 
	// to fill coordinates of next destroyed deck.
	int numberOfDestroyedDecks = 0;

}DamagedShipToBeDestroedByAI;
/*
 *
 * END: STUFF THAT AI NEEDS
 *
 */
 

void PlacingShips(SeaCell(*field)[11][11], SeaCell(*enemysfield)[11][11], Player(*playersPointer), Player(*aisPointer));
bool Placing(int x, int y, int placingMode, int numOfDecks, int shipCounter, 
			 SeaCell(*field)[11][11], Player(*playersPointer));
void FillShipInfo(SeaCell(*field)[11][11], Player(*pointer), int x, int y, int shipsCounter,
				  int totalNumOfDecks, int xAugment, int yAugment);
bool PlacingCheck(int x, int y, SeaCell(*field)[11][11], Player(*player), int totalNumOfDecks, int xAugment, int yAugment);
void Print(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11]);
void ChooseRandomSquare(int(*x), int(*y), SeaCell(*playersField)[11][11]);
void Print(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11]);
void Repaint(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11]);
bool Playing(SeaCell(*playersField)[11][11], SeaCell(*enemyField)[11][11], Player(*playersPointer),
			 Player(*aisPointer), DamagedShipToBeDestroedByAI *shipToAttack);
ShotResult ShootingChecker(int *x, int *y, SeaCell(*field)[11][11], Player(*playersPointer));
ShotResult CompareCoord(int *x, int *y, SeaCell(*field)[11][11], Player(*playersPointer), int numOfShip, int numOfDeck);
bool GetNum(char *symbol, char leftBorder, char rightBorder);
bool Check(char symbol, char leftBorder, char rightBorder);