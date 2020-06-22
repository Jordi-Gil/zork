#ifndef __Item__
#define __Item__

#include "entity.h"

class Room;

enum ItemType
{
	COMMON,
	BREAKABLE,
	DECORATIVE,
	UNLOCKER,
	WEAPON,
	ARMOUR,
	INVISIBLE
};

class Item : public Entity
{
public:
	Item(const std::string& name, const std::string& little_description, const std::string& deep_description, Entity* parent, ItemType item_type = COMMON, int id = -1);
	~Item();

	void Look() const;
	int GetValue() const;
	void Examine();

public:

	int min_value;
	int max_value;

	ItemType item_type;

	Item *key;
	bool locked = false;
};

#endif