#include <iostream>
#include "globals.h"
#include "room.h"
#include "exit.h"
#include "item.h"
#include "creature.h"

// ----------------------------------------------------
Creature::Creature(const std::string& title, const std::string& description, Room* room, int id, CreatureType _type) :
	Entity(title, description, "", id, (Entity*)room) {
	type = CREATURE;
	creature_type = _type;
	hit_points = 1;
	min_damage = max_damage = min_protection = max_protection = 0;
	weapon = armour = NULL;
	combat_target = NULL;
}

// ----------------------------------------------------
Creature::~Creature()
{}

// ----------------------------------------------------
void Creature::Look(const std::vector<std::string>& args) const {
	if (IsAlive())
	{
		std::cout << name << "\n";
		std::cout << little_description << "\n";
	}
	else
	{
		std::cout << name << "'s corpse\n";
		std::cout << "Here lies dead: " << little_description << "\n";
	}
}

// ----------------------------------------------------
bool Creature::Go(const std::vector<std::string>& args)
{
	if (!IsAlive())
		return false;

	Exit* exit = GetRoom()->GetExit(args[1]);

	if (exit == NULL)
		return false;

	if (PlayerInRoom())
		std::cout << name << "goes " << args[1] << "...\n";

	ChangeParentTo(exit->GetDestinationFrom((Room*)parent));

	return true;
}

// ----------------------------------------------------
bool Creature::Pick(const std::vector<std::string>& args)
{
	if (!IsAlive())
		return false;

	Item* item = (Item*)parent->Find(args[1], ITEM);

	if (args.size() > 1)
	{
		// we could pick something from a container in our inventory ...
		if (item == NULL)
			item = (Item*)Find(args[1], ITEM);

		if (item == NULL)
			return false;

		Item* subitem = (Item*)item->Find(args[3], ITEM);

		if (subitem == NULL)
			return false;

		if (PlayerInRoom())
			std::cout << name << " looks into " << item->name << "...\n";

		item = subitem;
	}

	if (item == NULL)
		return false;

	if (PlayerInRoom())
		std::cout << name << " takes " << item->name << ".\n";

	item->ChangeParentTo(this);

	return true;
}

// ----------------------------------------------------
void Creature::Inventory() const
{
	std::vector<Entity*> items;
	FindAll(ITEM, items);

	if (items.size() == 0)
	{
		std::cout << name << " does not own any items\n";
		return;
	}

	std::cout << "\n" << name << " owns:\n";
	for (auto it : items)
	{
		if (it == weapon)
			std::cout << it->name << " (as weapon)\n";
		else if (it == armour)
			std::cout << it->name << " (as armour)\n";
		else
			std::cout << it->name << "\n";
	}
}

// ----------------------------------------------------
bool Creature::Equip(const std::vector<std::string>& args)
{
	if (!IsAlive())
		return false;

	Item* item = (Item*)Find(args[1], ITEM);

	if (item == NULL)
		return false;

	switch (item->type)
	{
	case WEAPON:
		weapon = item;
		break;

	case ARMOUR:
		armour = item;
		break;

	default:
		return false;
	}

	if (PlayerInRoom())
		std::cout << name << " equips " << item->name << "...\n";

	return true;
}

// ----------------------------------------------------
bool Creature::UnEquip(const std::vector<std::string>& args)
{
	if (!IsAlive())
		return false;

	Item* item = (Item*)Find(args[1], ITEM);

	if (item == NULL)
		return false;

	if (item == weapon)
		weapon = NULL;
	else if (item == weapon)
		armour = NULL;
	else
		return false;

	if (PlayerInRoom())
		std::cout << name << " un-equips " << item->name << "...\n";

	return true;
}

// ----------------------------------------------------
bool Creature::AutoEquip()
{
	if (!IsAlive())
		return false;

	std::vector<Entity*> items;
	FindAll(ITEM, items);

	for (auto it : items)
	{
		Item* i = (Item*) it;

		if (i->item_type == WEAPON) {
			if (weapon == NULL) weapon = i;
			else if ((weapon->max_value - weapon->min_value) < (i->max_value - i->min_value)) weapon = i;
		}
		if (i->item_type == ARMOUR) {
			if (armour == NULL) weapon = i;
			else if ((armour->max_value - armour->min_value) < (i->max_value - i->min_value)) armour = i;
		}
	}

	return true;
}

// ----------------------------------------------------
bool Creature::Lock(const std::vector<std::string>& args)
{
	if (!IsAlive())
		return false;

	Exit* exit = GetRoom()->GetExit(args[1]);

	if (exit == NULL || exit->locked == true)
		return false;

	Item* item = (Item*)Find(args[3], ITEM);

	if (item == NULL || exit->key != item)
		return false;

	if (PlayerInRoom())
		std::cout << "\n" << name << "locks " << exit->GetNameFrom((Room*)parent) << "...\n";

	exit->locked = true;

	return true;
}

// ----------------------------------------------------
bool Creature::UnLock(const std::vector<std::string>& args)
{
	if (!IsAlive())
		return false;

	Exit* exit = GetRoom()->GetExit(args[1]);

	if (exit == NULL || exit->locked == false)
		return false;

	Item* item = (Item*)Find(args[3], ITEM);

	if (item == NULL || exit->key != item)
		return false;

	if (PlayerInRoom())
		std::cout << "\n" << name << "unlocks " << exit->GetNameFrom((Room*)parent) << "...\n";

	exit->locked = false;

	return true;
}

// ----------------------------------------------------
bool Creature::Drop(const std::vector<std::string>& args)
{
	if (!IsAlive())
		return false;

	Item* item = (Item*)Find(args[1], ITEM);

	if (item == NULL)
		return false;

	if (PlayerInRoom())
		std::cout << name << " drops " << item->name << "...\n";

	item->ChangeParentTo(parent);

	return true;
}

// ----------------------------------------------------
Room* Creature::GetRoom() const
{
	return (Room*)parent;
}

// ----------------------------------------------------
bool Creature::PlayerInRoom() const
{
	return parent->Find(PLAYER) != NULL;
}

// ----------------------------------------------------
bool Creature::IsAlive() const
{
	return hit_points > 0;
}

// ----------------------------------------------------
void Creature::Tick()
{
	if (combat_target != NULL)
	{
		if (parent->Find(combat_target) == true)
			MakeAttack();
		else
			combat_target = NULL;
	}
}

// ----------------------------------------------------
bool Creature::Attack(const std::vector<std::string>& args)
{
	Creature *target = (Creature*)parent->Find(args[1], CREATURE);

	if (target == NULL)
		return false;

	combat_target = target;
	std::cout << "\n" << name << " attacks " << target->name << "!\n";
	return true;
}

// ----------------------------------------------------
int Creature::MakeAttack()
{
	if (!IsAlive() || !combat_target->IsAlive() || combat_rounds > 15)
	{
		combat_target = combat_target->combat_target = NULL;
		combat_rounds = 0;
		std::cout << "> ";
		return false;
	}

	int result = (weapon) ? weapon->GetValue() : Roll(min_damage, max_damage);

	if (PlayerInRoom())
		std::cout << name << " attacks " << combat_target->name << " for " << result << "\n";

	combat_target->ReceiveAttack(result);

	// make the attacker react and take me as a target
	if (combat_target->combat_target == NULL)
		combat_target->combat_target = this;

	++combat_rounds;

	return result;
}

// ----------------------------------------------------
int Creature::ReceiveAttack(int damage)
{
	int prot = (armour) ? armour->GetValue() : Roll(min_protection, max_protection);
	int received = damage - prot;

	hit_points -= received;

	if (PlayerInRoom())
		std::cout << name << " is hit for " << received << " damage (" << prot << " blocked) \n";

	if (IsAlive() == false)
		Die();

	return received;
}

// ----------------------------------------------------
void Creature::Die()
{
	if (PlayerInRoom())
		std::cout << name << " dies.\n";
}

// ----------------------------------------------------
bool Creature::Loot(const std::vector<std::string>& args)
{
	Creature *target = (Creature*)parent->Find(args[1], CREATURE);

	if (target == NULL && target->IsAlive() == false)
		return false;

	std::vector<Entity*> items;
	target->FindAll(ITEM, items);

	for (auto it : items)
	{
		Item* i = (Item*) it;
		i->ChangeParentTo(this);
	}

	std::cout << "\n" << name << " loots " << target->name << "'s corpse\n";

	return true;
}

// ----------------------------------------------------
void Creature::Stats() const
{
	std::cout << "\nHit Points: " << hit_points;
	std::cout << "\nAttack: (" << ((weapon) ? weapon->name : "no weapon") << ") ";
	std::cout << ((weapon) ? weapon->min_value : min_damage) << "-" << ((weapon) ? weapon->max_value : max_damage);
	std::cout << "\nProtection: (" << ((armour) ? armour->name : "no armour") << ") ";
	std::cout << ((armour) ? armour->min_value : min_protection) << "-" << ((armour) ? armour->max_value : max_protection);
	std::cout << "\n";
}