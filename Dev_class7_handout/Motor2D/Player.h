#ifndef PLAYER_H_
#define PLAYER_H_

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "Animation.h"


struct SDL_Texture;
struct Collider;

enum PlayerTypes {
	FIRE_WISP = 0,
	WATER_WISP,
	ROCK_WISP,
	WIND_WISP

};

struct PlayerData {
	int xpos;
	int ypos;
	float yvel;
	float xvel;
	float yvel2;
	PlayerTypes type;
	bool PlayerOnTop;
	bool PlayerOnBot;  //CONTROL IF THE PLAYER IS COLLISIONING WITH A COLLIDER AND WHERE IS THE PLAYER RESPECT THE COLLIDER
	bool PlayerOnLeft;
	bool PlayerOnRight;
	bool PlayerColliding;
};


class PlayerClass : public j1Module {

public:
	char name;
	PlayerClass();

	//destructor
	virtual ~PlayerClass();
	bool Start();
	//bool Awake(pugi::xml_node &config);

	//bool CleanUp();

	// Load / Save
	//bool Load(pugi::xml_node&);
	//bool Save(pugi::xml_node&) const;
	
	

	bool Update(float dt);
	void MovePlayer();
	void MovePlayerCollider();
	void PlayerAnims();
	void OnCollision(Collider *c1, Collider *c2);
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
	pugi::xml_document AnimsDoc;
	
	pugi::xml_node AnimsNode;
	pugi::xml_node PlayerXmlNode;
	p2SString			folder;


public:
	SDL_Rect playerrect;
	SDL_Rect StaminaRect;
	SDL_Rect CurrentAnimationRect;

public:
	bool map_loaded;

	bool jumping = false;  //bool to know when is jumping 
	bool bot_reached = false;
	bool top_reached = false;
	uint yposaux;
	bool automatic_right = false;  //two bools on left and right to know if the player is moving in the air(xdirection) automatically after stop pressing A or D
	bool automatic_left = false;
	bool fall_atack = false; //bool to know if the fall attack is being done
	bool movingright = false;  //usefool for the idle left or idle right anims
	bool movingleft = false;
	bool activateleftmovement = false;
	bool activaterightmovement = false;

public:


	SDL_Texture* Textures = nullptr;
	SDL_Texture* Texturesflipped = nullptr;

	Animation* current_animation = nullptr;

	
	Animation idle_left;
	
	Animation run_left;
	Animation jump_right;
	Animation jump_left;
	
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
public:
	Collider *PlayerCollider;
	Collider *TheWallCollider;  // the collider that we are interacting with in each moment
};

#endif
