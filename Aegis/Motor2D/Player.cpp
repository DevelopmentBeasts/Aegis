#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "Player.h"
#include <math.h>
#include "j1Input.h"
#include "j1Scene.h"

//Destructor

PlayerClass::~PlayerClass() {

}
PlayerClass::PlayerClass() {   //DO PUSHBACKS WITH XML
	name.create("player");

	pugi::xml_parse_result result = AnimsDoc.load_file("PlayerAnims.xml");

	if (result == NULL) {
		LOG("The xml file that contains the pushbacks for the animations is not working.PlayerAnims.xml. error: %s",result.description());
	}
	
	AnimsNode = AnimsDoc.child("config").child("AnimsCoords").child("idle_left");
	idle.LoadPushbacks(AnimsNode);
	AnimsNode = AnimsDoc.child("config").child("AnimsCoords").child("move_left");
	move.LoadPushbacks(AnimsNode);
	AnimsNode = AnimsDoc.child("config").child("AnimsCoords").child("death");
	death.LoadPushbacks(AnimsNode);
}

bool PlayerClass::Start() {
	
	bool ret = true;
	pugi::xml_parse_result result = PlayerStartFile.load_file("StartPlayerConfig.xml");
	if (result == NULL) {
		LOG("Could not load StartPlayerConfig.xml. pugi error: %s", result.description());
		ret = false;
	}
	if (ret == true) {
		//Load all Player starter info
		LOG("Loading player info at the start of the game");
	}
	PlayerXmlNode = PlayerStartFile.child("config").child("player");
	if (PlayerXmlNode == NULL) {
		LOG("PlayerXmlNode is not reading correctly");
	}
	//Asigment of the values
	const char* type1 = PlayerXmlNode.attribute("type").as_string();
	
	if (type1 == "FIRE_WISP") {
		avatar = FIRE_WISP;
	}
	if (type1 == "WATER_WISP") {
		avatar = WATER_WISP;
	}
	if (type1 == "ROCK_WISP") {
		avatar = ROCK_WISP;
	}
	if (type1 == "WISP_WISP") {
		avatar = WIND_WISP;
	}

	
	velocity.y = PlayerXmlNode.attribute("yvel").as_int(); //player stats
	velocity.x = PlayerXmlNode.attribute("xvel").as_int();

	player_rect.w = PlayerXmlNode.child("playerrect").attribute("w").as_int(); //the rect that contains the player in wich we blit()
	player_rect.h = PlayerXmlNode.child("playerrect").attribute("h").as_int();


	LOG("Resseting anims");

	//Reset animations
	idle.Reset();
	move.Reset();

	LOG("LOADING PLAYER TEXTURES");

	player_texture = App->tex->Load("textures/Fire_Wisp/fireSheet.png");

	current_animation = &move;

	LOG("CREATING PLAYER COLLIDER");
	player_collider = App->collision->AddCollider({ position.x, position.y, player_rect.w, player_rect.h }, COLLIDER_PLAYER, this);

	velocity = { 0,0 };
	current_animation = &idle;

	return ret;
}


bool PlayerClass::Update(float dt) {

	if (ExternalInput(inputs))
	{
		//InternalInput(inputs);
		process_fsm(inputs);
	}

	//Move the player
	position += velocity;

	//Move the collider
	player_collider->SetPos(position.x, position.y);

	//Draw the player
	App->render->Blit(player_texture,position.x,position.y,&current_animation->GetCurrentFrame(),1,rotation,flip);

	return true;
}

void PlayerClass::MovePlayer() {
	
	
}

bool PlayerClass::Save(pugi::xml_node& node)const{
	
	pugi::xml_node pos = node.append_child("position");

	pos.append_attribute("x") = position.x;
	pos.append_attribute("y") = position.y;

	return true;
}

bool PlayerClass::Load(pugi::xml_node& node) {

	position.x = node.child("position").attribute("x").as_int();
	position.y = node.child("position").attribute("y").as_int();
	return true;
}

bool PlayerClass::ExternalInput(p2Queue<player_inputs> &inputs) {
	
	static bool left = false;
	static bool right = false;
	static bool down = false;
	static bool up = false;

	

	// key is pressed
	if (App->input->GetKey(SDL_SCANCODE_W) == j1KeyState::KEY_DOWN) {
		inputs.Push(IN_JUMP);
		up = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_DOWN) {
		left = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_DOWN) {
		right = true;
	}

	// key is released
	if (App->input->GetKey(SDL_SCANCODE_A)== j1KeyState::KEY_UP) {
		inputs.Push(IN_LEFT_UP);
		left = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_D)== j1KeyState::KEY_UP) {
		inputs.Push(IN_RIGHT_UP);
		right = false;
	}
			
	
	if (left && right) 
		inputs.Push(IN_LEFT_AND_RIGHT);
	
	else{
		if (left) 
			inputs.Push(IN_LEFT_DOWN);
		
		if (right)
			inputs.Push(IN_RIGHT_DOWN);
	}

	return true;
}

player_states PlayerClass::process_fsm(p2Queue<player_inputs> &inputs) {
	
	static player_states state = ST_IDLE;
	player_inputs last_input;

	while (inputs.Pop(last_input))
	{
		switch (state)
		{
		case ST_IDLE:
			switch (last_input)
			{
			case IN_RIGHT_DOWN:state = ST_WALK_FORWARD; velocity = {5,0 };		flip = SDL_FLIP_HORIZONTAL; current_animation = &move;	break;
			case IN_LEFT_DOWN:state = ST_WALK_BACKWARD;	velocity = { -3,0 };	flip = SDL_FLIP_NONE;		current_animation = &move; break;
			case IN_JUMP:state = ST_JUMP_NEUTRAL; break;
			}
			break;
		
		case ST_WALK_FORWARD:
			switch (last_input)
			{
			case IN_RIGHT_UP:state = ST_IDLE;		velocity = { 0,0 }; current_animation = &idle; break;
			case IN_LEFT_AND_RIGHT:state = ST_IDLE; velocity={ 0,0 };	current_animation = &idle; break;
			}
			break;

		case ST_WALK_BACKWARD:
			switch (last_input)
			{
			case IN_LEFT_UP:state = ST_IDLE;		velocity = { 0,0 }; current_animation = &idle; break;
			case IN_LEFT_AND_RIGHT:state = ST_IDLE; velocity={ 0,0 };	current_animation = &idle; break;
			}
			break;
		case ST_JUMP_NEUTRAL:
			break;
		case ST_JUMP_FORWARD:
			break;
		case ST_JUMP_BACKWARD:
			break;
		default:
			break;
		}
	}
	
	
	return state;
}

void PlayerClass::OnCollision(Collider *c1, Collider *c2) {

	
}

void PlayerClass::GodMode() {
	
	/*current_animation = &idle;

	if (App->input->GetKey(SDL_SCANCODE_A)==KEY_REPEAT)
		data.xpos -= data.xvel;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		data.xpos += data.xvel;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		data.ypos -= data.xvel;

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		data.ypos += data.xvel;

	App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &current_animation->GetCurrentFrame(), 1, 0, SDL_FLIP_NONE, 1, 1, 1.0);*/

}

void PlayerClass::Die() {

	
	App->player->position.x = App->map->data.start_position.x;
	App->player->position.y = App->map->data.start_position.y;

	App->render->find_player = true;
}