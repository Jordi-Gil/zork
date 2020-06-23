#ifndef __BREAKABLE__
#define __BREAKABLE__


#include "item.h"

class Breakable : public Item
{
public:
	Breakable(const std::string& name, const std::string& little_description, const std::string& deep_description, Entity* parent, int id = -1);
	~Breakable();

	bool Interact(int damage);

public:

	int durability;

};

#endif // __BREAKABLE__