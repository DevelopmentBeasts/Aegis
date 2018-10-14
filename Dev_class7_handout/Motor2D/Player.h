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

	PlayerTypes type; // used for future adding of types of players

	bool PlayerOnTop;
	bool PlayerOnBot;  //CONTROL IF THE PLAYER IS COLLISIONING WITH A COLLIDER AND WHERE IS THE PLAYER RESPECT THE COLLIDER
	bool PlayerOnLeft;
	bool PlayerOnRight;

	bool PlayerColliding; //CONTROL IF PLAYER IS COLLIDING WITH SOMETHING (TOP,BOT,LEFT OR RIGHT)

	bool Falling; // IT IS NOT THE SAME TO JUST FALL THAN TO BE IN A JUMPING PRABOLA
	
};


class PlayerClass : public j1Module {

public:
	PlayerClass();

	SDL_Rect aux_rect = { 50,50,0,0 };

	//destructor
	virtual ~PlayerClass();
	bool Start();
	//bool Awake(pugi::xml_node &config);

	//bool CleanUp();

	// Load / Save

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	
	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;


	bool Update(float dt);
	void MovePlayer();
	void MovePlayerCollider();
	void PlayerAnims();
	void OnCollision(Collider *c1, Collider *c2);

	void Die();

	void GodMode();

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
	bool godmode_activated = false;				//God Mode will allow the player to fly around the map ignoring collisions

	bool map_loaded;

	bool jumping = false;   //bool to know when there is a jump
	
	bool bot_reached = false;
	bool top_reached = false;


	bool automatic_right = false;  //two bools on left and right to know if the player is moving in the air(xdirection) automatically after stop pressing A or D
	bool automatic_left = false;

	bool fall_atack = false; //bool to know if the fall attack is being USED (still not beig coded)

	bool movingright = false;  //usefool for the idle left or idle right anims
	bool movingleft = false; 

	bool activateleftmovement = false;
	bool activaterightmovement = false;

	bool UpdatePlayer; //update player start
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
	Animation death;

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
