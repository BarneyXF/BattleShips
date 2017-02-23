#pragma once

const unsigned short numOf4Ships = 1;
const unsigned short numOf3Ships = 2;
const unsigned short numOf2Ships = 3;
const unsigned short numOf1Ships = 4;
const unsigned short totalNumOfSqares = 20;

const int lowerFieldBound = 0;
const int upperFieldBound = 9;

// тип корабля
enum ShipType
{
	null = 0, patrol = 1, destroyer = 2, cruiser = 3, carrier = 4
};

// состояние клетки
enum SeaCell
{
	empty, ship, kill, marked
};

// стадия игры(не слишком полезно)
enum Stage
{
	menu = 0, placing = 1, playing = 2, end = 3
};

//результат выстрела
enum ShotResult
{
	miss, killed, damaged
};

//информация о количестве кораблей
typedef struct
{
	short totalNumOfPlSqares = 0;
	unsigned short numOf1ShipsPl = 0;
	unsigned short numOf2ShipsPl = 0;
	unsigned short numOf3ShipsPl = 0;
	unsigned short numOf4ShipsPl = 0;
}PlayersShipsCounter;

//информация о положении корабля
typedef struct
{
	int x[4];
	int y[4];
}ShipCell;

//информация о кораблях
typedef struct
{
	ShipType type = null;
	ShipCell cell;
	int health = 0;
} Ship;

//Ведет счет кораблей у обеих сторон и содержит местополежния и тип каждого
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
 



//функции
void PlacingShips(SeaCell(*field)[11][11], SeaCell(*enemysfield)[11][11], Player(*playersPointer), Player(*aisPointer));
bool PlacingCheck(int x, int y, SeaCell(*field)[11][11], Player(*playersPointer), int numOfDecks, int xP, int yP);
void Repaint(SeaCell(*field)[11][11], SeaCell(*enemyField)[11][11]);
bool Check(char c);
ShotResult ShootingChecker(int *x, int *y, SeaCell(*field)[11][11], Player(*playersPointer));
void Print(SeaCell(*field)[11][11], SeaCell(*battleField)[11][11]);
bool Playing(SeaCell(*playersField)[11][11], SeaCell(*enemyField)[11][11], Player(*playersPointer),
			 Player(*aisPointer), DamagedShipToBeDestroedByAI *shipToAttack);
void FillShipInfo(SeaCell(*field)[11][11], Player(*pointer), int x, int y, int counter, int i, int xP, int yP);
