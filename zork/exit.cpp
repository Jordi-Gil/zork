#include <iostream>
#include "room.h"
#include "Exit.h"

// ----------------------------------------------------
Exit::Exit(const std::string& name, const std::string& opposite_name, const std::string& description, Room* origin, Room* destination, int id, ExitType _type, bool one_way) :
	Entity(name, description, "", id, (Entity*)origin),
	closed(false), locked(false), key(NULL), one_way(one_way), destination(destination), opposite_name(opposite_name), exit_type(_type)
{
	type = EXIT;

	if (one_way == false)
		destination->container.push_back(this);
}

// ----------------------------------------------------
Exit::~Exit()
{
}


// ----------------------------------------------------
void Exit::Look() const
{
	std::cout << name << " to " << opposite_name << "\n";
	std::cout << little_description << "\n";
}

// ----------------------------------------------------
const std::string& Exit::GetNameFrom(const Room* room) const
{
	if (room == parent)
		return name;
	if (room == destination)
		return opposite_name;

	return name; // error ?
}

// ----------------------------------------------------
Room* Exit::GetDestinationFrom(const Room* room) const
{
	if (room == parent)
		return destination;
	if (room == destination)
		return (Room*)parent;

	return NULL;
}