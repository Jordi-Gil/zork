#ifndef __Creature__
#define __Creature__

#include <string>
#include "entity.h"

class Room;
class Item;

enum CreatureType {
	PASSIVE,
	OFENSSIVE
};

class Creature : public Entity
{
public:
	Creature(const std::string& name, const std::string& description, Room* room, int id = -1, CreatureType _type = PASSIVE);
	~Creature();

	virtual bool Go(const std::vector<std::string>& args);
	virtual void Look(const std::vector<std::string>& args) const;
	virtual bool Pick(const std::vector<std::string>& args);
	virtual bool Drop(const std::vector<std::string>& args);
	virtual void Inventory() const;
	virtual bool Equip(const std::vector<std::string>& args);
	virtual bool UnEquip(const std::vector<std::string>& args);
	virtual bool AutoEquip();
	virtual bool Lock(const std::vector<std::string>& args);
	virtual bool UnLock(const std::vector<std::string>& args);
	virtual void Tick();

	virtual bool Attack(const std::vector<std::string>& args);
	virtual int MakeAttack();
	virtual int ReceiveAttack(int damage);
	virtual void Die();
	virtual bool Loot(const std::vector<std::string>& args);
	virtual void Stats() const;

	Room* GetRoom() const;
	bool PlayerInRoom() const;
	bool IsAlive() const;

public:

	int hit_points;
	int min_damage;
	int max_damage;
	int min_protection;
	int max_protection;
	Creature* combat_target;
	Item* weapon;
	Item* armour;

	CreatureType creature_type;

	bool knocked;

	int combat_rounds = 0;
};

#endif