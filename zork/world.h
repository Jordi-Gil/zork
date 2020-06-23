#ifndef __World__
#define __World__

#include <string>
#include <vector>
#include <time.h>

#define TICK_FREQUENCY 0.5f

class Entity;
class Player;

class World
{
public:

	World(const std::string namePlayer, const std::string descriptionPlayer);
	~World();

	bool Tick(std::vector<std::string>& args, bool &change_room, bool &playerDead, bool &playerExited);
	bool ParseCommand(std::vector<std::string>& args, bool &change_room);
	void GameLoop();

	void loadRooms(const std::string &fileName);
	void loadExits(const std::string &fileName);
	void loadItems(const std::string &fileName);
	void loadCreatures(const std::string &fileName);

	std::string takeActualRoomName();

private:
	clock_t tick_timer;
	std::vector<Entity *> entities;
	Player *player;
};

#endif