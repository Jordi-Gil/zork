#include <iostream>
#include "globals.h"
#include "room.h"
#include "exit.h"
#include "item.h"
#include "player.h"

// ----------------------------------------------------
Player::Player(const std::string& title, const std::string& description, Room* room, int id) :
	Creature(title, description, room, id)
{
	type = PLAYER;
}

// ----------------------------------------------------
Player::~Player()
{
}

// ----------------------------------------------------
void Player::Look(const std::vector<std::string>& args) const
{
	if (args.size() > 1)
	{
		for (auto it : parent->container)
		{
			if (Same(it->name, args[1]) || (it->type == EXIT && Same(args[1], ((Exit*) it)->GetNameFrom((Room*)parent))))
			{
					it->Look();
					return;
			}
		}

		if (Same(args[1], "me"))
		{
			std::cout << "\n" << name << "\n";
			std::cout << little_description << "\n";
		}
	}
	else
	{
		parent->Look();
	}
}

// ----------------------------------------------------
bool Player::Go(const std::vector<std::string>& args, bool &change_room) {
	
	if (!IsAlive()) {
		std::cout << "\nYou're death\n";
		return true;
	}
	
	Exit* exit = GetRoom()->GetExit(args[1]);

	if (this->immobilezed) {
		std::cout << "\nYou can't move it. You're chained. \n";
		return false;
	}

	if (exit == NULL) {
		std::cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	if (exit->locked) {
		std::cout << "\nThat exit is locked.\n";
		return false;
	}

	if (exit->closed) {
		std::cout << "\nThat exit is closed.\n";
		return false;
	}

	change_room = true;

	std::cout << "\nYou take direction " << exit->GetNameFrom((Room*)parent) << "...\n";
	ChangeParentTo(exit->GetDestinationFrom((Room*)parent));
	parent->Look();

	std::vector<Entity *> ofenssive_creatures;
	
	parent->FindAll(CREATURE, ofenssive_creatures);

	for (auto it : ofenssive_creatures) {
		Creature *creature = (Creature *)it;
		if (creature->IsAlive()) {
			if (creature->creature_type == OFENSSIVE) {
				creature->combat_target = this;
				creature->AutoEquip();
			}
		}
	}
	
	if (Same(exit->name, "Yard")) exited = true;

	return true;
}

// ----------------------------------------------------
bool Player::Pick(const std::vector<std::string>& args)
{

	if (args.size() == 4)
	{
		Item* item = (Item*) parent->Find(args[3], ITEM);

		// we could pick something from a container in our inventory ...
		if (item == NULL)
			item = (Item*)Find(args[3], ITEM);

		if (item == NULL)
		{
			std::cout << "\nCannot find '" << args[3] << "' in this room or in your inventory.\n";
			return false;
		}

		Item* subitem = (Item*)item->Find(args[1], ITEM);

		if (subitem == NULL)
		{
			std::cout << "\n" << item->name << " does not contain '" << args[1] << "'.\n";
			return false;
		}

		std::cout << "\nYou take " << subitem->name << " from " << item->name << ".\n";
		subitem->ChangeParentTo(this);
	}
	else if (args.size() == 2)
	{
		Item* item = (Item*) parent->Find(args[1], ITEM);

		if (item == NULL)
		{
			std::cout << "\nThere is no item here with that name.\n";
			return false;
		}
		else if (item->item_type == BREAKABLE || item->item_type == DECORATIVE || item->item_type == INVISIBLE || item->item_type == SPYHOLE || item->item_type == STATIC || item->item_type == UNLOCKER || item->item_type == LOCKER) {
			std::cout << "\nThis is item cannot be taken.\n";
			return false;
		}

		std::cout << "\nYou take " << item->name << ".\n";
		item->ChangeParentTo(this);
	}

	return false;
}

// ----------------------------------------------------
void Player::Inventory() const
{
	std::vector<Entity*> items;
	FindAll(ITEM, items);

	if (items.size() == 0)
	{
		std::cout << "\nYou do not own any item.\n";
		return;
	}

	for (auto it : items)
	{
		if (it == weapon)
			std::cout << "\n" << it->name << " (as weapon)";
		else if (it == armour)
			std::cout << "\n" << it->name << " (as armour)";
		else
			std::cout << "\n" << it->name;
	}

	std::cout << "\n";
}

// ----------------------------------------------------
bool Player::Drop(const std::vector<std::string>& args)
{
	if (args.size() == 2)
	{
		Item* item = (Item*)Find(args[1], ITEM);

		if (item == NULL)
		{
			std::cout << "\nThere is no item on you with that name.\n";
			return false;
		}

		std::cout << "\nYou drop " << item->name << "...\n";
		item->ChangeParentTo(parent);

		return true;
	}
	else if (args.size() == 4)
	{
		Item* item = (Item*)Find(args[1], ITEM);

		if (item == NULL)
		{
			std::cout << "\nCan not find '" << args[1] << "' in your inventory.\n";
			return false;
		}

		Item* container = (Item*)parent->Find(args[3], ITEM);

		if (container == NULL)
		{
			container = (Item*)Find(args[3], ITEM);
			std::cout << "\nCan not find '" << args[3] << "' in your inventory or in the room.\n";
			return false;
		}

		std::cout << "\nYou put " << item->name << " into " << container->name << ".\n";
		item->ChangeParentTo(container);

		return true;
	}

	return false;
}

// ----------------------------------------------------
bool Player::Equip(const std::vector<std::string>& args)
{
	Item* item = (Item*)Find(args[1], ITEM);

	if (item == NULL)
	{
		std::cout << "\nCannot find '" << args[1] << "' is not in your inventory.\n";
		return false;
	}

	switch (item->item_type)
	{
	case WEAPON:
		weapon = item;
		break;

	case ARMOUR:
		armour = item;
		break;

	default:
		std::cout << "\n" << item->name << " cannot be equipped.\n";
		return false;
	}

	std::cout << "\nYou equip " << item->name << "...\n";

	return true;
}

// ----------------------------------------------------
bool Player::UnEquip(const std::vector<std::string>& args) {
	if (!IsAlive())
		return false;

	Item* item = (Item*)Find(args[1], ITEM);

	if (item == NULL)
	{
		std::cout << "\n" << item->name << " is not in your inventory.\n";
		return false;
	}

	if (item == weapon)
		weapon = NULL;
	else if (item == armour)
		armour = NULL;
	else
	{
		std::cout << "\n" << item->name << " is not equipped.\n";
		return false;
	}

	std::cout << "\nYou un-equip " << item->name << "...\n";

	return true;
}

bool Player::Examine_creature(const std::vector<std::string> &args) {

	Creature *target = (Creature*) parent->Find(args[1], CREATURE);

	if (target == NULL)
	{
		std::cout << "\n" << args[1] << " is not here.\n";
		return false;
	}

	target->Inventory();
	target->Stats();

	return true;
}

bool Player::Examine_item(const std::vector<std::string> &args) {
	
	Item *item = (Item *) Find(args[1], ITEM);

	if (item == NULL) {

		item = (Item *) parent->Find(args[1], ITEM);

		if (item == NULL) {
			std::cout << "\nCannot find '" << args[1] << "', is not in your inventory or room.\n";
			return false;
		}
		
		item->Examine();
		return true;
	}

	item->Examine();
	return true;
}

// ----------------------------------------------------
bool Player::Examine(const std::vector<std::string>& args) {

	EntityType type = FindType(args[1]);

	type = (type == ENTITY) ? parent->FindType(args[1]) : type;

	switch (type)
	{
		case CREATURE:
			return Examine_creature(args);
		case ITEM:
			return Examine_item(args);
		default:
			break;
	}

	return false;
}

// ----------------------------------------------------
bool Player::Attack(const std::vector<std::string>& args) {
	Creature *target = (Creature*)parent->Find(args[1], CREATURE);

	if (target == NULL)
	{
		std::cout << "\n" << args[1] << " is not here.";
		return false;
	}

	combat_target = target;
	std::cout << "\nYou jump to attack " << target->name << "!\n";
	return true;
}

// ----------------------------------------------------
bool Player::Loot(const std::vector<std::string>& args) {
	Creature *target = (Creature*)parent->Find(args[1], CREATURE);

	if (target == NULL)
	{
		std::cout << "\n" << args[1] << " is not here.\n";
		return false;
	}

	if (target->IsAlive() == true)
	{
		std::cout << "\n" << target->name << " cannot be looted until it is killed.\n";
		return false;
	}

	std::vector<Entity*> items;
	target->FindAll(ITEM, items);

	if (items.size() > 0)
	{
		std::cout << "\nYou loot " << target->name << "'s corpse:\n";

		for (auto it : items)
		{
			Item* i = (Item*) it;
			std::cout << "You find: " << i->name << "\n";
			i->ChangeParentTo(this);
		}
	}
	else
		std::cout << "\nYou loot " << target->name << "'s corpse, but find nothing there.\n";

	return true;
}

// ----------------------------------------------------
bool Player::Lock(const std::vector<std::string>& args)
{
	if (!IsAlive())
		return false;

	Exit* exit = GetRoom()->GetExit(args[1]);

	if (exit == NULL)
	{
		std::cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	if (exit->locked)
	{
		std::cout << "\nThat exit is already locked.\n";
		return false;
	}

	Item* item = (Item*)Find(args[3], ITEM);

	if (item == NULL)
	{
		std::cout << "\nItem '" << args[3] << "' not found in your inventory.\n";
		return false;
	}

	if (exit->key != item)
	{
		std::cout << "\nItem '" << item->name << "' is not the key for " << exit->GetNameFrom((Room*)parent) << ".\n";
		return false;
	}

	std::cout << "\nYou lock " << exit->GetNameFrom((Room*)parent) << "...\n";

	exit->locked = true;

	return true;
}

bool Player::Unlock_item(const std::vector<std::string> &args) {

	Item *item_locked = (Item *)parent->Find(args[1], ITEM);

	if (item_locked == NULL) {
		std::cout << "\nThe item locked not exists.\n";
		return false;
	}

	if (!item_locked->locked) {
		std::cout << "\nThe item is already unlocked.\n";
		return false;
	}

	Item *item_unlocker = (Item *)Find(args[3], ITEM);

	if (item_unlocker == NULL) {
		std::cout << "\nItem '" << args[3] << "' not found in your inventory.\n";
		return false;
	}

	if (item_locked->interactor == NULL) {
		if (Same(item_locked->code, args[3])) std::cout << "\nCode correct!\n";
		else {
			std::cout << "\nCode correct!\nTry again\n";
			return true;
		}
	}
	else if ((item_locked->interactor)->id != item_unlocker->id) {
		std::cout << "\n" << item_locked->interactor->name << std::endl;
		std::cout << "\n" << item_unlocker->name << " is not the key for " << item_locked->name << ".\n";
		return false;
	}

	std::cout << "\nYou unlock " << item_locked->name << ".\n";
	item_locked->locked = false;

	if (Same(item_locked->name, "Padlock")) this->immobilezed = false;

	return true;

}

bool Player::Unlock_with_key(Exit *exit, const std::vector<std::string> &args) {
	
	Item* item = (Item* )Find(args[3], ITEM);

	if (item == NULL)
	{
		std::cout << "\nKey '" << args[3] << "' not found in your inventory.\n";
		return false;
	}

	if (exit->key != item)
	{
		std::cout << "\nKey '" << item->name << "' is not the key for " << exit->GetNameFrom((Room*)parent) << ".\n";
		return false;
	}

	std::cout << "\nYou unlock " << exit->GetNameFrom((Room*)parent) << "...\n";

	exit->locked = false;

	return true;
}

bool Player::Unlock_with_code(Exit *exit, const std::vector<std::string> &args) {

	if (Same(exit->code, args[3])) {
		std::cout << "\nCode correct!\nYou unlock " << exit->GetNameFrom((Room*)parent) << "...\n";

		exit->locked = false;

		return true;
	}

	std::cout << "\nCode incorrect!\nTry again\n";

	return false;
}

bool Player::Unlock_exit(const std::vector<std::string>& args) {

	Exit* exit = GetRoom()->GetExit(args[1]);

	if (exit == NULL)
	{
		std::cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	if (exit->locked == false)
	{
		std::cout << "\nThat exit is not locked.\n";
		return false;
	}

	switch (exit->exit_type)
	{
		case KEY:
			return Unlock_with_key(exit, args);
		case CODE:
			return Unlock_with_code(exit, args);
		case MECHANISM:
			std::cout << "\nThis door is unlocked by a mechanism.\n";
			return true;
		default:
			return false;
			break;
	}

}

// ----------------------------------------------------
bool Player::UnLock(const std::vector<std::string>& args)
{
	if (!IsAlive())
		return false;

	EntityType type = FindType(args[1]);

	std::cout << "Type of " << args[1] << ": " << type << "\n";

	if (type == ENTITY) type = parent->FindType(args[1]);

	std::cout << "Type of " << args[1] << ": " << type << "\n";

	switch (type) {
	case ITEM:
		return Unlock_item(args);
	case EXIT:
		return Unlock_exit(args);
	default:
		std::cout << "Item or Exit not found.\n";
		return true;
	}

	return false;

}

// Entry: open [direction]
bool Player::Open(const std::vector<std::string>& args) {

	if (!IsAlive())
		return false;

	if (this->immobilezed) {
		std::cout << "\nYou can't move it. You're chained. \n";
		return false;
	}

	// We get the room where the player is located, and once we have it we look if there is the specified exit in that room;
	Exit *exit = GetRoom()->GetExit(args[1]);

	// If the exit not exist, then return false
	if (exit == NULL) {
		std::cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	// If the exit is locked you need to unlock first
	if (exit->locked) {
		std::cout << "\nThe exit is locked, please unlocked first.\n";
		return false;
	}

	if (!exit->closed && !exit->freeSpace) {
		std::cout << "\nThe exit is already opened.\n";
		return false;
	}

	if (exit->freeSpace) {
		std::cout << "\nThe exit doesn't have any door mechanism, cannot be opened. The exit is always opened.\n";
		return false;
	}

	std::cout << "\nYou open " << exit->GetNameFrom((Room*)parent) << "...\n";

	exit->closed = false;

	return true;

}

bool Player::Close(const std::vector<std::string>& args) {

	if (!IsAlive())
		return false;

	if (this->immobilezed) {
		std::cout << "\nYou can't move it. You're chained. \n";
		return false;
	}

	// We get the room where the player is located, and once we have it we look if there is the specified exit in that room;
	Exit *exit = GetRoom()->GetExit(args[1]);

	// If the exit not exist, then return false
	if (exit == NULL) {
		std::cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	// If the exit is locked you need to unlock first
	if (exit->locked) {
		std::cout << "\nThe exit is already locked.\n";
		return false;
	}

	if (exit->closed) {
		std::cout << "\nThe exit is already closed.\n";
		return false;
	}

	if (exit->freeSpace) {
		std::cout << "\nThe exit doesn't have any door mechanism, cannot be closed.\n";
		return false;
	}

	std::cout << "\nYou close " << exit->GetNameFrom((Room*)parent) << "...\n";

	exit->closed = true;

	return true;

}

void Player::hit(Item *item) {

	if (item->min_value <= 0) {
		std::cout << "\nThe " << item->name << " is already broken.\n";
		return;
	}

	std::cout << "\nYou hit the " << item->name << ".\n";
	item->min_value -= max_damage;

	if (item->min_value <= 0) {
		std::cout << "You broke the " << item->name << " and drops...\n";
		std::vector<Entity *> items;
		item->FindAllByParent(item->id, items);
		Room *room = GetRoom();

		for (auto it : items) {
			std::cout << it->name << std::endl;
			Item *subitem = (Item *) it;
			subitem->ChangeParentTo(room);
		}

	}
	else {
		std::cout << "The item " << item->name << " has " << item->min_value << " durability points left.\n";
	}
}

void Player::move(Item *item) {
	
	if (!item->moved) {
		std::cout << "\nYou move the " << item->name;
		item->moved = true;

		std::vector<Entity *> items;
		item->FindAllByParent(item->id, items);

		if (items.size() > 0) std::cout << " and you realize there's something behind it...\n";
		else std::cout << "\n";

		Room *room = GetRoom();

		for (auto it : items) {
			std::cout << it->name << std::endl;
			Item *subitem = (Item *)it;
			subitem->ChangeParentTo(room);
		}

	}
	else {
		std::cout << "\n" << item->name << " is already moved.\n";
	}
}

bool Player::intUnlocker(Item *item) {
	
	//if mechanism is obstructed, try to unlock
	if (item->obstructed) {

		Item *subitem = (Item *) Find(item->interactor->name, ITEM);

		if (subitem == NULL) {
			std::cout << "\nThe " << item->interactor->name << " needed to unlock the mechanism is not in your inventory.\n";
			return false;
		}

		item->obstructed = false;
		std::cout << "You've unlocked the mechanism\n";
	}
	else if(!item->obstructed && !item->exit->locked){
		std::cout << "\nThe mechanism is already unlocked\n";
		return true;
	}

	std::cout << "\n* faraway noise * It seems the mechanism of " << item->name << " has worked, some door has opened...\n";
	item->exit->locked = false;
	item->exit->closed = false;

	return true;
}

bool Player::Heal(Item *item) {

	std::cout << "\nItem: " << item->name << " value: " << item->min_value << " " << item->max_value << std::endl;

	int result = Roll(item->min_value, item->max_value);

	this->hit_points += result;

	std::cout << "\nYou You've healed " << result << " points of life.\n";

	item->ChangeParentTo(NULL);

	this->Stats();

	return true;

}

bool Player::Interact(const std::vector<std::string>& args) {

	if (args.size() == 2) {
		Item* item = (Item*) parent->Find(args[1], ITEM);

		if (item == NULL) {

			item = (Item *) Find(args[1], ITEM);

			if (item == NULL) {
				std::cout << "\nThere is no item here with that name in your inventory or in this room.\n";
				return false;
			}
			
		}
		else if (item->type == HEALER) {
			std::cout << "\nThere is no item here with that name in your inventory.\n";
			return false;
		}


		switch (item->item_type)
		{
			case BREAKABLE: {
				hit(item);
				return true;
			}
			case DECORATIVE: {
				move(item);
				return true;
				break;
			}
			case SPYHOLE:
				std::cout << "I can see something through " << item->name;
				item->interactor->Examine();
				return true;
				break;
			case UNLOCKER:
				return intUnlocker(item);
				break;
			case HEALER:
				return Heal(item);
				break;
			default: {
				std::cout << "\nYou can't interact with this item.\n";
				return true;
			}
		}

	}

	return false;

}

bool Player::Throw(const std::vector<std::string>& args) {

	Item *item = (Item *)Find(args[1], ITEM);

	if (item == NULL) {
		std::cout << "\nItem '" << args[1] << "' not found in your inventory.\n";
		return false;
	}

	Item *target = (Item *) parent->Find(args[3], ITEM);

	if (target == NULL) {
		std::cout << "\nItem '" << args[3] << "' not found in this room.\n";
		return false;
	}

	if (target->min_value <= 0) {
		std::cout << "\nThis item is already broken.\n";
		return false;
	}

	target->min_value -= item->min_value;

	if (target->min_value <= 0) {
		std::cout << "\nYou broke " << target->name;

		std::vector<Entity *> items;
		target->FindAllByParent(target->id, items);
		Room *room = GetRoom();

		//the item thrown now is in the room
		item->ChangeParentTo(room);
		
		if (items.size() > 0) std::cout << " and drops:\n";
		else std::cout << "\n";

		for (auto it : items) {
			std::cout << it->name << std::endl;
			Item *subitem = (Item *)it;
			subitem->ChangeParentTo(room);
		}

	}

	return true;
}