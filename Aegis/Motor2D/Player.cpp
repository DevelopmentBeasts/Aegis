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

	velocity = { 0.0,0.0 };
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
	
		position.x += velocity.x;
		
	

	//Move the collider
	player_collider->SetPos(position.x, position.y);

	//Draw the player
	player_rect.x = position.x-879;
	player_rect.y = position.y-550;
	CurrentAnimationRect = current_animation->GetCurrentFrame();
	if (flip != SDL_FLIP_HORIZONTAL) {
		rotation *=-1;
	}
	SDL_RenderCopyEx(App->render->renderer, player_texture, &CurrentAnimationRect, &player_rect/*podemos controlar el tamaño*/, rotation * 10, 0, flip);

	//App->render->Blit(player_texture,position.x,position.y,&current_animation->GetCurrentFrame(),1,/*rotation is equal to jumpvelocity.y*/rotation * 10,flip,0,0,1);
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
	
	

	// key is pressed
	if (App->input->GetKey(SDL_SCANCODE_W) == j1KeyState::KEY_DOWN) {
		inputs.Push(IN_JUMP_DOWN);
		up = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_DOWN) {
		left = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_DOWN) {
		right = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == j1KeyState::KEY_DOWN) {
		if (jump) {
			deceleration = true;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_F) == j1KeyState::KEY_DOWN) {
		//CODE TO MAKE MEGA FALL
		if (jump) {
			LOG("MEGA FALL ATTACK");
			inputs.Push(IN_FALL_ATTACK);
		}
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
	if (velocity.x == 0) {
		goto provisional;
	}
	while (inputs.Pop(last_input))
	{
		switch (state)
		{
		case ST_IDLE:
			LOG("IM FUCKING IDL MEN");
			
			switch (last_input)
			{
			case IN_RIGHT_DOWN:
				state = ST_WALK_FORWARD;
				velocity = { 5,0 };
				flip = SDL_FLIP_HORIZONTAL;
				current_animation = &move;
				//LOG("INPUT----->FROM IDL TO WALK RIGHT");
				break;
			case IN_LEFT_DOWN:
				state = ST_WALK_BACKWARD;
				velocity = { -5,0 };
				flip = SDL_FLIP_NONE;
				current_animation = &move;
				//LOG("INPUT----->FROM IDL TO WALK LEFT");
				break;
			case IN_JUMP_DOWN:
				if (!jump) {
					state = ST_JUMP_NEUTRAL;
					jump = true;
					//LOG("INPUT----->JUMP_DOWN");
					current_animation = &move;
				}		
				break;
			}

			break;

		case ST_WALK_FORWARD:
			//LOG("WALKING RIGHT");
			switch (last_input)
			{
			case IN_RIGHT_UP:
				//LOG("STOP WALKING RIGHT");
				provisional:
				state = ST_IDLE;
				if (!jump) {
					velocity = { 0,0 };
					current_animation = &idle;
				}
				break;
			case IN_LEFT_AND_RIGHT:
				//LOG("WTF LEFT AND RIGHT WALKING ");
				state = ST_IDLE;
				velocity = { 0,0 };
				current_animation = &idle;
				break;
			case IN_JUMP_DOWN:	
				if (!jump) {
					state = ST_JUMP_FORWARD;
					//LOG("INPUT----->JUMP_DOWN");
					current_animation = &move;
					jump = true;
				}
				break;
			}
			break;

		case ST_WALK_BACKWARD:
			//LOG("WALKING LEFT");
			switch (last_input)
			{
			case IN_LEFT_UP:
				//LOG("STOP WALKING LEFT");
				state = ST_IDLE;
				if (!jump) {
					velocity = { 0,0 };
					current_animation = &idle;
				}
				break;
			case IN_LEFT_AND_RIGHT:
				//LOG("WTF LEFT AND RIGHT WALKING ");
				state = ST_IDLE;
				velocity = { 0,0 };
				current_animation = &idle;
				break;
			case IN_JUMP_DOWN:
				if (!jump) {
					state = ST_JUMP_BACKWARD;
					//LOG("INPUT----->JUMP_DOWN");
					current_animation = &move;
					jump = true;
				}		
				break;
			}
			break;
		case ST_JUMP_NEUTRAL:
			//LOG("THE JUMP IS NEUTRAL");
			if (!jump) {
				//LOG("CHANGING TO IDL");
				state = ST_IDLE;
			}
			switch (last_input)
			{
			case IN_RIGHT_DOWN:
				state = ST_WALK_FORWARD;
				velocity = { 5,0 };
				flip = SDL_FLIP_HORIZONTAL;
				current_animation = &move;
				//LOG("INPUT----->JUMPING RIGHT ------");
				break;
			case IN_LEFT_DOWN:
				state = ST_WALK_BACKWARD;
				velocity = { -5,0 };
				flip = SDL_FLIP_NONE;
				current_animation = &move;
				//LOG("INPUT----->JUMPING LEFT ------");
				break;
			}

		case ST_JUMP_FORWARD:
			LOG("THE JUMP IS FORWARD");
			if (!jump) {
				//LOG("CHANGING TO IDL");
				state = ST_IDLE;
			}
			switch (last_input)
			{
			case IN_RIGHT_DOWN:
				state = ST_WALK_FORWARD;
				velocity = { 5,0 };
				flip = SDL_FLIP_HORIZONTAL;
				current_animation = &move;
				//LOG("INPUT----->JUMPING RIGHT ------ ");
				break;
			case IN_LEFT_DOWN:
				state = ST_WALK_BACKWARD;
				velocity = { -5,0 };
				flip = SDL_FLIP_NONE;
				current_animation = &move;
				//LOG("INPUT----->JUMPING LEFT ------ ");
				break;
			}
			break;
		case ST_JUMP_BACKWARD:
			//LOG("THE JUMP IS BACKWARD");
			if (!jump) {
				//LOG("CHANGING TO IDL");
				state = ST_IDLE;
			}
			switch (last_input)
			{
			case IN_RIGHT_DOWN:
				state = ST_WALK_FORWARD;
				velocity = { 5,0 };
				flip = SDL_FLIP_HORIZONTAL;
				current_animation = &move;
				//LOG("INPUT----->JUMPING RIGHT ------ ");
				break;
			case IN_LEFT_DOWN:
				state = ST_WALK_BACKWARD;
				velocity = { -5,0 };
				flip = SDL_FLIP_NONE;
				current_animation = &move;
				//LOG("INPUT----->JUMPING LEFT ------ ");
				break;
			}
			break;
		case ST_FALL_ATTACK:
			LOG("NEED CODE FOR FALL ATTACK");
			break;
		default:
			break;
		}
	}
	if (jump == true) {
		Jump();
	}

	return state;
}

void PlayerClass::OnCollision(Collider *c1, Collider *c2) {
	//if(c1->type == player_collider)

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
void PlayerClass::Jump() {

	if (App->player->position.y < 851) {
		if (deceleration) {
			if (-1 * velocity.x < 0) { // if velocity is positive
				velocity.x -= 0.2;
			}
			if (-1 * velocity.x > 0) { // if velocity is negative
				velocity.x += 0.2;
			}
		}
		jumpvelocity.y += 0.2;
		rotation = jumpvelocity.y;
		App->player->position.y += jumpvelocity.y;
		if (App->player->position.y > 848 ) {
			if (right) {
				velocity.x = 5;
			}
			else if (left) {
				velocity.x = -5;
			}
			else if ((!left) && (!right)) {
				velocity.x = 0;
			}
			rotation = 0;
		}
		LOG("velocity.X       %f", velocity.x);

	}else if (App->player->position.y >= 851) {
		App->player->position.y = 850;
		//velocity.x = 0;
		jumpvelocity.y = -6;
		jump = false;
		deceleration = false;
		current_animation = &move;

	}
	
	
}