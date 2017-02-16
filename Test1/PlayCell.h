#pragma once
#include "Ship.h"
class PlayCell
{
public:
	PlayCell();
	~PlayCell();
	bool Get_was_fired();
	bool Get_possible_to_set_ship();
	void Set_was_fired(bool);
	void Set_possible_to_set_ship(bool);

	Ship* Get_ship_instance();
	void Set_ship_instance(Ship*);

private:
	Ship *ship_instance;
	bool was_fired;
	bool possible_to_set_ship;
};

