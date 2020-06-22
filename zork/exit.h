#ifndef __Exit__
#define __Exit__

#include "entity.h"

class Room;

class Exit : public Entity
{
public:
	Exit(const std::string& name, const std::string& opposite_name, const std::string& description, Room* origin, Room* destination, int id = -1, bool one_way = false);
	~Exit();

	void Look() const;

	const std::string& GetNameFrom(const Room* room) const;
	Room* GetDestinationFrom(const Room* room) const;

public:
	bool one_way;
	bool closed;
	bool locked;
	bool freeSpace;
	std::string opposite_name;
	Room* destination;
	Entity* key;
};

#endif