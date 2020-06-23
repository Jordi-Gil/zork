#include <iostream>
#include "breakable.h"

Breakable::Breakable(const std::string& name, const std::string& little_description, const std::string& deep_description, Entity* parent, int id) :
Item(name, little_description, deep_description, parent, BREAKABLE, id)
{
	durability = 0;
}


Breakable::~Breakable()
{
}

bool Breakable::Interact(int damage) {

	if (durability <= 0) {
		std::cout << "\nThe " << name << " is already broken.\n";
		return false;
	}

	std::cout << "\nYou hit the " << name << ".\n";
	durability -= damage;

	if (durability <= 0) {
		std::cout << "You broke the " << name << ".\n";
		std::vector<Entity *> items;
		FindAllByParent(id, items);
		Room *room = GetRoom();

		if (items.size() > 1) std::cout << name << " drops:\n";

		for (auto it : items) {
			Item *subitem = (Item *)it;
			subitem->ChangeParentTo((Entity *) room);
		}

	}
	else {
		std::cout << "The item " << name << " has " << durability << " durability points left.\n";
	}

	return true;
}