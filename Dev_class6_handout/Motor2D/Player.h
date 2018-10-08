#ifndef PLAYER_H_
#define PLAYER_H_

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

enum PlayerTypes {
	FIRE_WISP = 0,
	WATER_WISP,
	ROCK_WISP,
	WIND_WISP

};

struct PlayerData {
	uint xpos;
	uint ypos;
	float yvel;
	float xvel;
	PlayerTypes type;
};


class PlayerClass: public j1Module {

public:
	char name;
	PlayerClass();

	//destructor
	virtual ~PlayerClass();
	bool Start();
	//bool Awake(pugi::xml_node &config);

	void Draw();

	//bool CleanUp();

	bool Load(const char* path);

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;


	bool Update(float dt);
	
private:

	/*bool LoadPlayer();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
*/
public:

	//MapData data;
	PlayerData data;

private:

	pugi::xml_document	PlayerStartFile;
	pugi::xml_node PlayerXmlNode;
	p2SString			folder;
	bool				map_loaded;
	
public:
	SDL_Rect rect;
public:
	bool jumping = false;
	bool bot_reached = false;
	bool top_reached = false;
	uint max_height;
	uint min_height;
	uint yposaux;
};

#endif
