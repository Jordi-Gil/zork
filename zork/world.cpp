#include <iostream>
#include <fstream>
#include <sstream>

#include <Windows.h>
#include <io.h>
#include <fcntl.h>

#include "globals.h"
#include "entity.h"
#include "creature.h"
#include "item.h"
#include "exit.h"
#include "room.h"
#include "player.h"
#include "world.h"

Entity *findEntity(const std::vector<Entity *> &entities, int id) {

	for (auto it : entities) {

		Room *room = (Room *) it;

		if (room->id != -1 && room->id == id) {
			return room;
		}
	}
	return NULL;
}

// ----------------------------------------------------
World::World(const std::string namePlayer, const std::string descriptionPlayer)
{
	tick_timer = clock();

	loadRooms("Resources/rooms.txt");

	// Player ----
	Room *spawnRoom = (Room *)findEntity(entities, 1);

	player = new Player(namePlayer.c_str(), descriptionPlayer.c_str(), spawnRoom, 0);
	player->hit_points = 30;
	player->max_damage = 1;
	player->immobilezed = true;
	entities.push_back(player);

	loadCreatures("Resources/creatures.txt");
	loadExits("Resources/exits.txt");
	loadItems("Resources/items.txt");
	
}

// ----------------------------------------------------
World::~World()
{
	for (auto it : entities)
		delete it;

	entities.clear();
}

// ----------------------------------------------------
bool World::Tick(std::vector<std::string>& args, bool &change_room, bool &playerDead, bool &playerExited)
{

	if (!player->IsAlive()) {
		playerDead = true;
		return false;
	}

	if (player->exited) {
		playerExited = true;
		return false;
	}

	bool ret = true;

	if (args.size() > 0 && args[0].length() > 0)
		ret = ParseCommand(args, change_room);
	else if (args.size() > 0 && args[0].length() <= 0) ret = false;

	GameLoop();

	return ret;
}

// ----------------------------------------------------
void World::GameLoop()
{
	clock_t now = clock();

	if ((now - tick_timer) / CLOCKS_PER_SEC > TICK_FREQUENCY)
	{
		for (auto it : entities)
			it->Tick();

		tick_timer = now;
	}
}

// ----------------------------------------------------
bool World::ParseCommand(std::vector<std::string>& args, bool &change_room)
{
	bool ret = true;

	switch (args.size()) {
	case 1: // commands with no arguments ------------------------------
	{
		if (Same(args[0], "look") || Same(args[0], "l")) {
			player->Look(args);
		}
		else if (Same(args[0], "north") || Same(args[0], "n")) {
			args.push_back("north");
			player->Go(args, change_room);
		}
		else if (Same(args[0], "south") || Same(args[0], "s")) {
			args.push_back("south");
			player->Go(args, change_room);
		}
		else if (Same(args[0], "east") || Same(args[0], "e")) {
			args.push_back("east");
			player->Go(args, change_room);
		}
		else if (Same(args[0], "west") || Same(args[0], "w")) {
			args.push_back("west");
			player->Go(args, change_room);
		}
		else if (Same(args[0], "north-west") || Same(args[0], "nw")) {
			args.push_back("north-west");
			player->Go(args, change_room);
		}
		else if (Same(args[0], "north-east") || Same(args[0], "ne")) {
			args.push_back("north-east");
			player->Go(args, change_room);
		}
		else if (Same(args[0], "south-west") || Same(args[0], "sw")) {
			args.push_back("south-west");
			player->Go(args, change_room);
		}
		else if (Same(args[0], "south-east") || Same(args[0], "se")) {
			args.push_back("south-east");
			player->Go(args, change_room);
		}
		else if (Same(args[0], "up") || Same(args[0], "u")) {
			args.push_back("up");
			player->Go(args, change_room);
		}
		else if (Same(args[0], "down") || Same(args[0], "d")) {
			args.push_back("down");
			player->Go(args, change_room);
		}
		else if (Same(args[0], "stats") || Same(args[0], "st")) {
			player->Stats();
		}
		else if (Same(args[0], "inventory") || Same(args[0], "i")) {
			player->Inventory();
		}
		else
			ret = false;
		break;
	}
	case 2: // commands with one argument ------------------------------
	{
		if (Same(args[0], "look") || Same(args[0], "l")) {
			player->Look(args);
		}
		else if (Same(args[0], "go")) {
			player->Go(args, change_room);
		}
		else if (Same(args[0], "take") || Same(args[0], "pick")) {
			player->Pick(args);
		}
		else if (Same(args[0], "drop") || Same(args[0], "put")) {
			player->Drop(args);
		}
		else if (Same(args[0], "equip") || Same(args[0], "eq")) {
			player->Equip(args);
		}
		else if (Same(args[0], "unequip") || Same(args[0], "uneq")) {
			player->UnEquip(args);
		}
		else if (Same(args[0], "examine") || Same(args[0], "ex")) {
			player->Examine(args);
		}
		else if (Same(args[0], "attack") || Same(args[0], "at")) {
			player->Attack(args);
		}
		else if (Same(args[0], "loot") || Same(args[0], "lt")) {
			player->Loot(args);
		}
		else if (Same(args[0], "open")) {
			player->Open(args);
		}
		else if (Same(args[0], "close")) {
			player->Close(args);
		}
		else if (Same(args[0], "interact")) {
			player->Interact(args);
		}
		else
			ret = false;
		break;
	}
	case 3: // commands with two arguments ------------------------------
	{
		ret = false;
		break;
	}
	case 4: // commands with three arguments ------------------------------
	{
		if (Same(args[0], "unlock") || Same(args[0], "unlk")) {
			player->UnLock(args);
		}
		else if (Same(args[0], "lock") || Same(args[0], "lk")) {
			player->Lock(args);
		}
		else if (Same(args[0], "take") || Same(args[0], "pick")) {
			player->Pick(args);
		}
		else if (Same(args[0], "drop") || Same(args[0], "put")) {
			player->Drop(args);
		}
		else if (Same(args[0], "throw")) {
			player->Throw(args);
		}
		else
			ret = false;
		break;
	}
	default:
		ret = false;
	}

	return ret;
}

void World::loadRooms(const std::string &fileName) {

	std::ifstream raw;
	std::string line;
	std::stringstream sstream;

	raw.open(fileName.c_str());

	if (!raw.is_open()) {
		std::cout << "File rooms not found" << std::endl;
		exit(99);
	}

	while (getline(raw, line)) {

		std::vector<std::string> parameters;

		split(line, parameters,";");

		Room* room = new Room(parameters[0].c_str(), parameters[1].c_str(), parameters[2].c_str(), stoi(parameters[3]));

		entities.push_back(room);

	}

}

void World::loadExits(const std::string &fileName) {

	std::ifstream raw;
	std::string line;
	std::stringstream sstream;

	raw.open(fileName.c_str());

	if (!raw.is_open()) {
		std::cout << "File exits not found" << std::endl;
		exit(99);
	}

	while (getline(raw, line)) {

		std::vector<std::string> parameters;

		split(line, parameters, ";");

		Room *origin, *destination;

		origin = (Room *)findEntity(entities, std::stoi(parameters[3]));
		destination = (Room *)findEntity(entities, std::stoi(parameters[4]));

		Exit* exit = new Exit(parameters[0].c_str(), parameters[1].c_str(), parameters[2].c_str(), origin, destination, std::stoi(parameters[8]),static_cast<ExitType>(std::stoi(parameters[9])));

		exit->freeSpace = (parameters[5] == "true") ? true : false;
		exit->closed = (parameters[6] == "true") ? true : false;
		exit->locked = (parameters[7] == "true") ? true : false;

		// If the exit represents a "free space"  like a Path or Arc, then cannot be locked or closed
		if (exit->freeSpace) {
			exit->closed = exit->locked = false;
		}

		switch (exit->exit_type)
		{
			case CODE:
				exit->code = parameters[10];
				exit->one_way = Same(parameters[11], "true") ? true : false;
				break;
			case MECHANISM:
				exit->key = (Item *) findEntity(entities, std::stoi(parameters[9]));
				exit->one_way = Same(parameters[10], "true") ? true : false;
				break;
			default:
				exit->one_way = Same(parameters[10], "true") ? true : false;
				break;
		}

		entities.push_back(exit);

	}

}

void World::loadItems(const std::string &fileName) {

	std::ifstream raw;
	std::string line;
	std::stringstream sstream;

	raw.open(fileName.c_str());

	if (!raw.is_open()) {
		std::cout << "File items not found" << std::endl;
		exit(99);
	}

	while (getline(raw, line)) {

		std::vector<std::string> parameters;

		split(line, parameters, ";");

		Entity *parent;

		parent = findEntity(entities, std::stoi(parameters[2]));

		Item* item = new Item(parameters[0].c_str(), parameters[1].c_str(), (parameters[5] == "true") ? parameters[6].c_str() : "", parent, static_cast<ItemType>(stoi(parameters[3])), std::stoi(parameters[4]));

		std::cout << "Item: " << item->name << std::endl;

		switch (item->item_type)
		{
		case COMMON:
			if (Same(parameters[7], "true")) {
				for (unsigned int i = 8; i < parameters.size(); i++) {
					Exit *exit = (Exit *)findEntity(entities, std::stoi(parameters[i]));
					std::cout << "Key: " << item->name << " for exit: " << ((exit == NULL) ? "NULL" : std::to_string(exit->id)) << std::endl;
					exit->key = item;
				}
			}
			break;
		case BREAKABLE:
			item->min_value = std::stoi(parameters[7]);
			break;
		case ARMOUR:
			item->min_value = std::stoi(parameters[7]);
			item->max_value = std::stoi(parameters[8]);
			break;
		case WEAPON:
			item->min_value = std::stoi(parameters[7]);
			item->max_value = std::stoi(parameters[8]);
			break;
		case INVISIBLE:
			item->interactor = (Item *)findEntity(entities, std::stoi(parameters[7]));
			item->locked = true;
			break;
		case SPYHOLE:
			item->interactor = (Item *)findEntity(entities, std::stoi(parameters[7]));
			break;
		case UNLOCKER:
			item->exit = (Exit *)findEntity(entities, std::stoi(parameters[7]));
			item->obstructed = Same(parameters[8], "true") ? true : false;
			if(item->obstructed) item->interactor = (Item *)findEntity(entities, std::stoi(parameters[9]));
			break;
		case THROWABLE:
			item->min_value = std::stoi(parameters[7]);
			break;
		case HEALER:
			item->min_value = std::stoi(parameters[7]);
			item->max_value = std::stoi(parameters[8]);
			break;
		case LOCKER:
			if (std::stoi(parameters[7])) item->code = parameters[8];
			else item->interactor = (Item *)findEntity(entities, std::stoi(parameters[8]));
			item->locked;
			break;
		default:
			break;
		}

		entities.push_back(item);

	}

}

void World::loadCreatures(const std::string &fileName) {

	std::ifstream raw;
	std::string line;
	std::stringstream sstream;

	raw.open(fileName.c_str());

	if (!raw.is_open()) {
		std::cout << "File items not found" << std::endl;
		exit(99);
	}

	while (getline(raw, line)) {

		std::vector<std::string> parameters;

		split(line, parameters, ";");

		Room *parent;

		parent = (Room *) findEntity(entities, std::stoi(parameters[2]));

		Creature* creature = new Creature(parameters[0], parameters[1], parent, std::stoi(parameters[3]), static_cast<CreatureType>(stoi(parameters[4])));

		creature->hit_points = std::stoi(parameters[5]);
		creature->min_damage = std::stoi(parameters[6]);
		creature->max_damage = std::stoi(parameters[7]);

		entities.push_back(creature);

	}
}

std::string World::takeActualRoomName() {
	return (player->parent)->name;
}