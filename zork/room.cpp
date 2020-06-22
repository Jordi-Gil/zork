#include <iostream>
#include "globals.h"
#include "exit.h"
#include "item.h"
#include "creature.h"
#include "room.h"

// ----------------------------------------------------
Room::Room(const std::string& title, const std::string& little_description, const std::string& deep_description, int id) :
	Entity(title, little_description, deep_description, id, NULL)
{
	type = ROOM;
}

// ----------------------------------------------------
Room::~Room()
{
}

// ----------------------------------------------------
void Room::Look() const
{
	std::cout << "\n" << name << "\n";
	std::cout << little_description;

	// List exits --
	for (auto it : container)
	{
		if (it->type == EXIT)
		{
			Exit* ex = (Exit*) it;
			std::cout << "\nDirection (" << ex->GetNameFrom(this) << ") you see " << ex->GetDestinationFrom(this)->name;
		}
	}

	// List items --
	for (auto it : container)
	{
		if (it->type == ITEM)
		{
			Item* item = (Item*) it;
			if (item->item_type != INVISIBLE)
				std::cout << "\nThere is an item here: " << item->name;
		}
	}

	// List creatures --
	for (auto it : container)
	{
		if (it->type == CREATURE)
		{
			Creature* cr = (Creature*) it;
			std::cout << "\nThere is someone else here: " << cr->name;
			if (cr->IsAlive() == false)
				std::cout << " (dead)";
		}
	}

	std::cout << "\n";
}

// ----------------------------------------------------
Exit* Room::GetExit(const std::string& direction) const
{
	for (auto it : container)
	{
		if (it->type == EXIT)
		{
			Exit* ex = (Exit*) it;
			if (Same(ex->GetNameFrom(this), direction))
				return ex;
		}
	}

	return NULL;
}


