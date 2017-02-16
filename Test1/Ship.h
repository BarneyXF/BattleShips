#pragma once


	class Ship
	{
	public:
		Ship(unsigned int);
		~Ship();
		unsigned int Get_health();
		void Set_health(unsigned int);

	private:
		unsigned int health;
		
	};
