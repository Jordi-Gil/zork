#ifndef __Entity__
#define __Entity__

#include <string>
#include <vector>

enum EntityType
{
	ENTITY,
	ROOM,
	EXIT,
	ITEM,
	CREATURE,
	PLAYER
};

class Entity
{
public:

	Entity(const std::string &name, const std::string &little_description, const std::string &deep_description, int id, Entity* parent);
	virtual ~Entity();

	virtual void Look() const;
	virtual void Tick();

	void	ChangeParentTo(Entity* new_parent);
	bool    Find(Entity* entity) const;
	Entity* Find(EntityType type) const;
	Entity* Find(const std::string& name, EntityType type) const;
	void	FindAll(EntityType type, std::vector<Entity*>& vector_to_fill) const;
	void	FindAllByParent(int id, std::vector<Entity *>& vector_to_fill) const;
	EntityType FindType(const std::string &name);

public:

	EntityType type;
	std::string name;
	std::string little_description;
	std::string deep_description;

	int id;

	Entity *parent;

	std::vector<Entity *> container;

};

#endif // __ENTITY__