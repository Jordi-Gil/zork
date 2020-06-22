#ifndef __Player__
#define __Player__

#include "creature.h"

class Player : public Creature
{
public:

	Player(const std::string& name, const std::string& description, Room* room, int id);
	~Player();

	bool Go(const std::vector<std::string>& args, bool &change_room);
	void Look(const std::vector<std::string>& args) const;
	bool Pick(const std::vector<std::string>& args);
	bool Drop(const std::vector<std::string>& args);
	void Inventory() const;
	bool Equip(const std::vector<std::string>& args);
	bool UnEquip(const std::vector<std::string>& args);
	bool Examine(const std::vector<std::string>& args);
	bool Attack(const std::vector<std::string>& args);
	bool Loot(const std::vector<std::string>& args);
	bool Lock(const std::vector<std::string>& args);
	bool UnLock(const std::vector<std::string>& args);
	bool Open(const std::vector<std::string>& args);
	bool Close(const std::vector<std::string>& args);
	bool Interact(const std::vector<std::string>& args);

	void hit(Item *item);
	bool Unlock_exit(const std::vector<std::string>& args);
	bool Unlock_item(const std::vector<std::string>& args);
	bool Examine_creature(const std::vector<std::string>& args);
	bool Examine_item(const std::vector<std::string>& args);


};

#endif //__Player__