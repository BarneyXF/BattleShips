#include "PlayCell.h"


bool PlayCell::Get_was_fired()
{
	return was_fired;
}

bool PlayCell::Get_possible_to_set_ship()
{
	return possible_to_set_ship;
}

void PlayCell::Set_was_fired(bool b)
{
	was_fired = b;
}

void PlayCell::Set_possible_to_set_ship(bool b)
{
	possible_to_set_ship = b;
}

Ship* PlayCell::Get_ship_instance()
{
	return ship_instance;
}

void PlayCell::Set_ship_instance(Ship* s)
{
	ship_instance = s;
}

PlayCell::PlayCell()
{
	Set_was_fired(false);
	Set_possible_to_set_ship(true);
	Set_ship_instance(nullptr);
}


PlayCell::~PlayCell()
{
}
