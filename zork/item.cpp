#include <iostream>
#include "room.h"
#include "exit.h"
#include "globals.h"
#include "item.h"

// ----------------------------------------------------
Item::Item(const std::string& title, const std::string& little_description, const std::string& deep_description, Entity* parent, ItemType item_type, int id) :
	Entity(title, little_description, deep_description, id, parent), item_type(item_type)
{
	type = ITEM;
	min_value = max_value = 0;
}

// ----------------------------------------------------
Item::~Item()
{}

// ----------------------------------------------------
void Item::Look() const
{
	std::cout << "\n" << name << "\n";
	std::cout << little_description << "\n";

	std::vector<Entity*> stuff;
	FindAll(ITEM, stuff);

	if (stuff.size() > 0)
	{
		std::cout << "It contains: " << "\n";
		for (auto it : stuff)
			std::cout << it->name << "\n";
	}
}

// ----------------------------------------------------
int Item::GetValue() const
{
	return Roll(min_value, max_value);
}

void Item::Examine() {
	std::cout << "\n" << name << "\n";
	std::cout << deep_description << "\n";
}