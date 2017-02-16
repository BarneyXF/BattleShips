#include "Header.h"
#include "Ship.h"



unsigned int Ship::Get_health()
{
	return health;
}

void Ship::Set_health(unsigned int x)
{
	health = x;
}

	Ship::Ship(unsigned int x)
	{
		Set_health(x);
	}

	Ship::~Ship()
	{
	}
