#include <iostream>
#include <algorithm>

#include "globals.h"
#include "Entity.h"
#include "Exit.h"

// ----------------------------------------------------
Entity::Entity(const std::string& name, const std::string& little_description, const std::string& deep_description, int id, Entity* parent = NULL) :
	name(name), little_description(little_description), deep_description(deep_description), id(id), parent(parent)
{
	type = ENTITY;

	if (parent != NULL)
		parent->container.push_back(this);
}

// ----------------------------------------------------
Entity::~Entity()
{}

// ----------------------------------------------------
void Entity::Look() const {
	std::cout << name << "\n";
	std::cout << little_description << "\n";
}

// ----------------------------------------------------
void Entity::Tick()
{}

// ----------------------------------------------------
void Entity::ChangeParentTo(Entity* new_parent) {
	if (parent != NULL) {
		
		std::vector<Entity *>::iterator pos;
		pos = std::remove( parent->container.begin(), parent->container.end(), this);
		(parent->container).erase(pos);
	}

	parent = new_parent;

	if (parent != NULL)
		parent->container.push_back(this);
}

// ----------------------------------------------------
bool Entity::Find(Entity* entity) const {
	for (auto it : container)
	{
		if (it == entity)
			return true;
	}

	return false;
}

// ----------------------------------------------------
Entity* Entity::Find(EntityType type) const {
	for (auto it : container)
	{
		if (it->type == type)
			return it;
	}

	return NULL;
}

// ----------------------------------------------------
Entity* Entity::Find(const std::string& name, EntityType type) const {
	for (auto it : container)
	{
		if (it->type == type)
		{
			if (Same(it->name, name))
				return it;
		}
	}
	return NULL;
}

// ----------------------------------------------------
void Entity::FindAll(EntityType type, std::vector<Entity*>& vector_to_fill) const {
	for (auto it : container) {
		if (it->type == type)
			vector_to_fill.push_back(it);
	}
}


void Entity::FindAllByParent(int id, std::vector<Entity *>& vector_to_fill) const {
	for (auto it : container) {
		if ((it->parent)->id == id)
			vector_to_fill.push_back(it);
	}
}

EntityType Entity::FindType(const std::string &name) {
	for (auto it : container) {
		if (Same(it->name, name)) {
			return it->type;
		}
		else {
			Exit *exit = (Exit *)it;
			if (exit != NULL) {
				if (Same(exit->opposite_name, name)) return exit->type;
			}
		}
	}
	return ENTITY;
}