#ifndef __Item__
#define __Item__

#include "entity.h"

class Room;
class Exit;

enum ItemType
{
	COMMON,
	BREAKABLE,
	DECORATIVE,
	UNLOCKER,
	WEAPON,
	ARMOUR,
	INVISIBLE,
	STATIC,
	SPYHOLE,
	THROWABLE,
	HEALER,
	LOCKER
};

class Item : public Entity
{
public:
	Item(const std::string& name, const std::string& little_description, const std::string& deep_description, Entity* parent, ItemType item_type = COMMON, int id = -1);
	~Item();

	virtual bool Interact(int damage);

	void Look() const;
	int GetValue() const;
	void Examine();

	Room* GetRoom() const;

public:

	int min_value;
	int max_value;

	ItemType item_type;

	//Key for items locked by and item
	Item *interactor;
	//Key for items locked by a code
	std::string code;
	
	//for invisible (padlock)
	bool locked = false;
	
	//for decoration
	bool moved = false;
	
	//for mechanism
	Exit *exit;
	bool obstructed = true;
};

#endif