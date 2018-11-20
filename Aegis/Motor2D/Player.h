#ifndef PLAYER_H_
#define PLAYER_H_

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "Animation.h"
#include "p2Queue.h"
#include "Entity.h"


struct SDL_Texture;
struct Collider;

enum PlayerTypes {

	FIRE_WISP = 0,
	WATER_WISP,
	ROCK_WISP,
	WIND_WISP

};

enum player_states {
	ST_UNKNOWN, 
	ST_IDLE,
	ST_WALK_FORWARD,
	ST_WALK_BACKWARD,
	ST_JUMP_NEUTRAL,
	ST_JUMP_FORWARD,
	ST_JUMP_BACKWARD,
	ST_FRONT_ATTACK
};

enum player_inputs {
	IN_LEFT_UP,
	IN_LEFT_DOWN, 
	IN_RIGHT_UP,
	IN_RIGHT_DOWN, 
	IN_LEFT_AND_RIGHT,
	IN_JUMP_DOWN,
	IN_JUMP_UP,
	IN_FRONT_ATTACK
};

class PlayerClass : public j1Entity {

public:

	PlayerClass(iPoint pos);

	virtual ~PlayerClass();
	bool Start();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	bool Update(float dt);
	void MovePlayer();
	void OnCollision(Collider *c1, Collider *c2);

	//Player states and behaviour system
	bool ExternalInput(p2Queue<player_inputs>&inputs);					///Add keyboard inputs to the list
	void InternalInput(p2Queue<player_inputs>&inputs);					///Add internal inputs to the list
	player_states process_fsm(p2Queue<player_inputs>&inputs,float dt);	///Act depending on the inputs

	void Jump();
	

	//God Mode will allow the player to fly around the map ignoring collisions
	void GodMode(float dt);

public:
	SDL_Rect player_rect;

	bool map_loaded;

	bool jump = false;
	bool front_attack = false;
	bool JumpRotation = false;
	bool FallRotation = false;
	bool deceleration = false;

	bool left = false;
	bool right = false;
	bool down = false;
	bool up = false;

	bool ToLeft = false;
	bool ToRight = false;

	bool Gravity = false;
	
	bool DoLogic = false;

	SDL_Texture* player_texture = nullptr;
	
	//Colliders
	Collider *player_collider;

	//Player data
	                 		///position in X & Y axis
	
	fPoint jumpvelocity = {0.0,-6.0};    ///velocity in X & Y axis
	float rotation;			///rotation for blit
	PlayerTypes avatar;		///current character
	SDL_RendererFlip flip;	///animation flip
	
	SDL_Rect CurrentAnimationRect;
	float PlayerScale;
	float GravityValue;
	float JumpForce;

	int CurrFrame = 0;
	
	Animation* current_animation = nullptr;	///Animation being shown at the moment
private:

	//Call godmode if true	
	bool godmode_activated = false;

	//Animationss
	
	Animation idle;							///In all the animations the character is facing the left side 
	Animation move;
	Animation death;

	//Inputs list
	p2Queue<player_inputs> inputs;

	//XML
	pugi::xml_document	PlayerStartFile;
	pugi::xml_document	PlayerStartFile_;
	pugi::xml_document AnimsDoc;
	pugi::xml_document ExplosionDoc;

	pugi::xml_node AnimsNode;
	pugi::xml_node PlayerXmlNode;
	pugi::xml_node PlayerXmlNode_;
	p2SString			folder;

	 

};

#endif
