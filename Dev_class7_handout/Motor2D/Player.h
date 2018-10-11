#ifndef PLAYER_H_
#define PLAYER_H_

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "Animation.h"


struct SDL_Texture;


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
	float yvel2;
	PlayerTypes type;
};


class PlayerClass : public j1Module {

public:
	char name;
	PlayerClass();

	//destructor
	virtual ~PlayerClass();
	bool Start();
	//bool Awake(pugi::xml_node &config);

	void Draw();

	//bool CleanUp();

	// Load / Save
	//bool Load(pugi::xml_node&);
	//bool Save(pugi::xml_node&) const;

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;


	bool Update(float dt);
	void MovePlayer();
	void PlayerAnims();
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
	SDL_Rect playerrect;
	SDL_Rect StaminaRect;
	SDL_Rect CurrentAnimationRect;
public:
	bool jumping = false;
	bool bot_reached = false;
	bool top_reached = false;
	uint max_height;
	uint min_height;
	uint yposaux;
	bool usethisbool;
	bool automatic_right = false;
	bool automatic_left = false;
	bool fall_atack = false;
	bool movingright = false;
	bool movingleft = false;
	bool activateleftmovement = false;
	bool activaterightmovement = false;
public:


	SDL_Texture* Textures = nullptr;
	SDL_Texture* Texturesflipped = nullptr;
	Animation* current_animation = nullptr;

	Animation idle_right;
	Animation idle_left;
	Animation run_right;
	Animation run_left;
	Animation jump_right;
	Animation jump_left;
	Animation fall_right;
	Animation fall_left;
	Animation attack_right;
	Animation attack_left;
public:
	bool SCANCODE_A = false;
	bool SCANCODE_D = false;
	bool SCANCODE_W = false;
	bool SCANCODE_S = false;
	bool LastDirectionLeft = false;
	bool LastDirectionRight = false;
};

#endif
