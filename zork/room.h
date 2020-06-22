#ifndef __Room__
#define __Room__

#include "entity.h"

class Exit;
class Item;

class Room : public Entity
{

public:
	Room(const std::string& name, const std::string& little_description, const std::string& deep_description, int id = -1);
	~Room();

	void Look() const;

	Exit* GetExit(const std::string& direction) const;

public:

};
#endif
