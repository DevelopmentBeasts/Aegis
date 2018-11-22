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
#include "Brofiler/Brofiler.h"
#include "j1App.h"
#include "Enemy.h"
#include "Entity.h"
#include "EntityManager.h"
//Destructor

PlayerClass::~PlayerClass() {

}
PlayerClass::PlayerClass(iPoint pos) : j1Entity(pos, ENTITY_TYPE::PLAYER)  {   //DO PUSHBACKS WITH XML
	//name.create("player");
	position = pos;


	pugi::xml_parse_result result = AnimsDoc.load_file("PlayerAnims.xml");

	if (result == NULL ) {
		LOG("The xml file that contains the pushbacks for the animations is not working.PlayerAnims.xml.  error: %s",result.description());
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

	//____________________

	pugi::xml_parse_result result_ = PlayerStartFile_.load_file("config.xml");
	if (result == NULL) {
		LOG("Could not load config.xml. pugi error: %s", result.description());
		ret = false;
	}
	if (ret == true) {
		//Load config info
		LOG("Loading config info in the player update at the start of the game");
	}
	PlayerXmlNode_ = PlayerStartFile_.child("config").child("UpdateInfo").child("player");
	if (PlayerXmlNode_ == NULL) {
		LOG("config.xml is not reading correctly");
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
	PlayerScale = PlayerXmlNode.attribute("scale").as_float();
	GravityValue = PlayerXmlNode.child("worldplayerinteraction").attribute("GravityValue").as_float();
	JumpForce = PlayerXmlNode.child("worldplayerinteraction").attribute("JumpForce").as_float();
	speedpowervalue = PlayerXmlNode.attribute("speedpower").as_int();
	AvailableDistanceright = PlayerXmlNode.child("sensors").attribute("sensor_distance").as_int();
	AvailableDistanceleft = PlayerXmlNode.child("sensors").attribute("sensor_distance").as_int();
	//dt stuff
	//movement_period = PlayerXmlNode_.attribute("period").as_float(33.0f);



	//Reset animations
	LOG("Resseting anims");
	idle.Reset();
	move.Reset();

	LOG("LOADING PLAYER TEXTURES");

	player_texture = App->tex->Load("textures/Fire_Wisp/fireSheet.png");

	current_animation = &move;

	LOG("CREATING PLAYER COLLIDER");
	player_collider = App->collision->AddEntCollider({ position.x, position.y, player_rect.w*(int)PlayerScale, player_rect.h*(int)PlayerScale }, COLLIDER_PLAYER, this);
	sensor_collider1 = App->collision->AddEntCollider({ position.x + player_rect.w,position.y ,300,player_rect.h-10 }, COLLIDER_SENSOR, this);
	sensor_collider2 = App->collision->AddEntCollider({ position.x - 300,position.y ,300,player_rect.h-10 }, COLLIDER_SENSOR, this);
	velocity = { 0.0,0.0 };
	current_animation = &idle;
	
	return ret;
}


bool PlayerClass::Update(float dt) {
	BROFILER_CATEGORY("PlayerUpdate();", Profiler::Color::Green);
    
	//FRAMERATE CONTROL
	if (App->framerate_cap_activated) {
		dt = 30;
		idle.speed = 0.22;
		move.speed = 0.22;
	}
	else if (!(App->framerate_cap_activated)) {
		idle.speed = 0.22;
		move.speed = 0.22;
		idle.speed *= (dt / 30);
		move.speed *= (dt / 30);
	}

	//GOD MODE
	if (App->input->GetKey(SDL_SCANCODE_F10) == j1KeyState::KEY_DOWN) {
		godmode_activated = !godmode_activated;
	}
	 
	if (!godmode_activated && position.y > 2300) {
		Die();
	}
	//INPUTS
	if (ExternalInput(inputs))
				process_fsm(inputs, dt);
	
	//Move the player
	if (!godmode_activated) {
			
		position.x += velocity.x*(dt/30);
		if (Gravity) {
			velocity.y += GravityValue * (dt / 30);
			position.y += velocity.y*(dt/30);
		}
	}
	else
		GodMode(dt);
	
	//SPEEDPOWER
	
	
	//Move the colliders
	player_collider->SetPos(position.x, position.y);
	sensor_collider1->SetPos(position.x+player_rect.w, position.y);
	sensor_collider2->SetPos(position.x - 300, position.y);
	
	//DRAW THE PLAYER
	CurrentAnimationRect = current_animation->GetCurrentFrame();
	
	
	if (JumpRotation && rotation < 27  ) {
		rotation = velocity.y;
	}
	else if (!JumpRotation) {
		rotation = 0;
	}
	if (flip != SDL_FLIP_HORIZONTAL) {
		rotation *= -1;
	}
	
	App->render->Blit(player_texture,position.x,position.y,&current_animation->GetCurrentFrame(),1,/*rotation is equal to jumpvelocity.y*/rotation * 3,flip,0,0, PlayerScale);
	return true;
}
bool PlayerClass::PostUpdate() {
	//LOG("POSTUPDATE");
	sensorcollidingright = false;
	sensorcollidingleft = false;
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
	
		if (App->input->GetKey(SDL_SCANCODE_G) == j1KeyState::KEY_DOWN) {
			Gravity = true;
		}
		if (App->input->GetKey(SDL_SCANCODE_N) == j1KeyState::KEY_DOWN) {
			if (!sensorcollidingleft) {
				AvailableDistanceleft = PlayerXmlNode.child("sensors").attribute("sensor_distance").as_int();
			}
			SpeedPowerActivatedLeft = true;
			AvailableDistanceRightNow = AvailableDistanceleft;
		}
		if (App->input->GetKey(SDL_SCANCODE_M) == j1KeyState::KEY_DOWN) {
			if (!sensorcollidingright) {
				AvailableDistanceright = PlayerXmlNode.child("sensors").attribute("sensor_distance").as_int();
			}
			SpeedPowerActivatedRight = true;
			AvailableDistanceRightNow = AvailableDistanceright;
		}
		
		// key is pressed
		if (App->input->GetKey(SDL_SCANCODE_W ) == j1KeyState::KEY_DOWN && !JumpRotation && Gravity == true) {
			inputs.Push(IN_JUMP_DOWN);
			JumpRotation = true;
			up = true;
			
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_DOWN) {
			left = true;
			//DIRECTIONAL BOOLS FOR COLLIDERS
			ToLeft = true;
			Gravity = true;
			ToRight = false;
			//LOG("TOLEFT = TRUE  /   TORIGHT = FALSE");
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_DOWN) {
			right = true;
			//DIRECTIONAL BOOLS FOR COLLIDERS
			ToLeft = false;
			ToRight = true;
			Gravity = true;
			//LOG("TOLEFT = FALSE  /   TORIGHT = TRUE");
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == j1KeyState::KEY_DOWN) {
			if (jump) {
				deceleration = true;
			}
		}

		// key is released
		if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_UP) {
			inputs.Push(IN_LEFT_UP);
			left = false;
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_UP) {
			inputs.Push(IN_RIGHT_UP);
			right = false;
		}

		if (left && right)
			inputs.Push(IN_LEFT_AND_RIGHT);

		else {
			if (left)
				inputs.Push(IN_LEFT_DOWN);

			if (right)
				inputs.Push(IN_RIGHT_DOWN);
		}
	
	return true;
}

player_states PlayerClass::process_fsm(p2Queue<player_inputs> &inputs,float dt) {
	
	
	if (!godmode_activated) {
	
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
				//LOG("IM  IDL ");
				
				switch (last_input)
				{
				case IN_RIGHT_DOWN:
					state = ST_WALK_FORWARD;
					velocity.x = 10;
					if (Gravity) {
						velocity.x =  10;
					}
					flip = SDL_FLIP_HORIZONTAL;
					current_animation = &move;
					//LOG("INPUT----->FROM IDL TO WALK RIGHT");
					break;
				case IN_LEFT_DOWN:
					state = ST_WALK_BACKWARD;
					velocity.x = -10;
					if (Gravity) {
						velocity.x = -10;
					}
					flip = SDL_FLIP_NONE;
					current_animation = &move;
					//LOG("INPUT----->FROM IDL TO WALK LEFT");
					break;
				case IN_JUMP_DOWN:
					if (!jump) {
						state = ST_JUMP_NEUTRAL;
						jump = true;
						//LOG("INPUT----->JUMP_DOWN");
					}
					break;
				
				}

				break;

			case ST_WALK_FORWARD:
				//LOG("WALKING RIGHT");
				if (!SpeedPowerActivatedLeft && !SpeedPowerActivatedRight) {
					velocity.x = 15;
				}
				switch (last_input)
				{
				case IN_RIGHT_UP:
					//LOG("STOP WALKING RIGHT");
				provisional:
					state = ST_IDLE;
					if (!jump) {
						velocity.x = 0;
						current_animation = &idle;
					}
					break;
				case IN_LEFT_AND_RIGHT:
					//LOG("WTF LEFT AND RIGHT WALKING ");
					state = ST_IDLE;
					velocity.x = 0;
					current_animation = &idle;
					break;
				case IN_JUMP_DOWN:
					if (!jump) {
						state = ST_JUMP_FORWARD;
						//LOG("INPUT----->JUMP_DOWN");
						jump = true;
				
					}
					break;
				}
				break;

			case ST_WALK_BACKWARD:
				//LOG("WALKING LEFT");
				if (!SpeedPowerActivatedLeft && !SpeedPowerActivatedRight) {
					velocity.x = -15;
				}
				switch (last_input)
				{
				case IN_LEFT_UP:
					//LOG("STOP WALKING LEFT");
					state = ST_IDLE;
					if (!jump) {
						velocity.x = 0;
						current_animation = &idle;
					}
					break;
				case IN_LEFT_AND_RIGHT:
					//LOG("WTF LEFT AND RIGHT WALKING ");
					state = ST_IDLE;
					velocity.x = 0;
					current_animation = &idle;
					break;
				case IN_JUMP_DOWN:
					if (!jump) {
						state = ST_JUMP_BACKWARD;
						//LOG("INPUT----->JUMP_DOWN");
						jump = true;
				
					}

					break;
				}
				break;
			case ST_JUMP_NEUTRAL:
				LOG("THE JUMP IS NEUTRAL");
				if (!jump) {
					//LOG("CHANGING TO IDL");
					state = ST_IDLE;
				}
				switch (last_input)
				{
				case IN_RIGHT_DOWN:
					state = ST_WALK_FORWARD;
					velocity.x = 10;
					flip = SDL_FLIP_HORIZONTAL;
					current_animation = &move;
					//LOG("INPUT----->JUMPING RIGHT ------");
					break;
				case IN_LEFT_DOWN:
					state = ST_WALK_BACKWARD;
					velocity.x = -10;
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
					velocity.x = 10;
					flip = SDL_FLIP_HORIZONTAL;
					current_animation = &move;
					//LOG("INPUT----->JUMPING RIGHT ------ ");
					break;
				case IN_LEFT_DOWN:
					state = ST_WALK_BACKWARD;
					velocity.x = -10;
					flip = SDL_FLIP_NONE;
					current_animation = &move;
					//LOG("INPUT----->JUMPING LEFT ------ ");
					break;
				}
				break;
			case ST_JUMP_BACKWARD:
				LOG("THE JUMP IS BACKWARD");

				if (!jump) {
					//LOG("CHANGING TO IDL");
					state = ST_IDLE;
				}
				switch (last_input)
				{
				case IN_RIGHT_DOWN:
					state = ST_WALK_FORWARD;
					velocity.x = 10;
					flip = SDL_FLIP_HORIZONTAL;
					current_animation = &move;
					//LOG("INPUT----->JUMPING RIGHT ------ ");
					break;
				case IN_LEFT_DOWN:
					state = ST_WALK_BACKWARD;
					velocity.x = -10;
					flip = SDL_FLIP_NONE;
					current_animation = &move;
					//LOG("INPUT----->JUMPING LEFT ------ ");
					break;
				case IN_FRONT_ATTACK:
					state = ST_FRONT_ATTACK;
					break;
				}
				break;
			

			default:
				break;
			}
		}
		if (velocity.y != 0) {
			current_animation = &move;
		}
		if (jump == true) {
			Jump();
			jump = false;
		}
		if (SpeedPowerActivatedRight) {
			flip = SDL_FLIP_HORIZONTAL;
			current_animation = &move;
			AvailableDistanceRightNow -= speedpowervalue;
			SpeedPower(speedpowervalue, AvailableDistanceRightNow+30,dt);
			
		}
		if (SpeedPowerActivatedLeft) {
			flip = SDL_FLIP_NONE;
			current_animation = &move;
			AvailableDistanceRightNow -= speedpowervalue;
			SpeedPower(-speedpowervalue, AvailableDistanceRightNow+30, dt);
			
		}
		if (deceleration) {
			velocity.x = 0;
			deceleration = false;
		}
	return state;
	}
}

void PlayerClass::OnCollision(Collider *c1, Collider *c2) {

	//if (c1 != sensor_collider1 && c1 != sensor_collider2) {
	//	AvailableDistance = 200;
	//}
	
		if (c2->type == COLLIDER_WALL) 
		{
			if (c1->type == COLLIDER_SENSOR) {
				if (c1 == sensor_collider1) {
					//LOG("RIGHT SENSOR ACTIVATED");
					sensorcollidingright = true;
					AvailableDistanceright = c2->rect.x - c1->rect.x;
					LOG(" AvailableDistance IS %i", AvailableDistanceright);
				}
				if (c1 == sensor_collider2) {
					//LOG("LEFT SENSOR ACTIVATED");
					sensorcollidingleft = true;
					AvailableDistanceleft =(300 - (c2->rect.x + c2->rect.w) - c1->rect.x);
					LOG(" AvailableDistance IS %i", AvailableDistanceleft);
				}
				
			}
			if (c1->type == COLLIDER_PLAYER) 
			{
				//Calculating an error margin of collision to avoid problems with colliders corners
				int error_margin = 0;

				if (ToRight) {
					error_margin = (c1->rect.x + c1->rect.w) - c2->rect.x;
				}
				else if (ToLeft) {
					error_margin = (c2->rect.x + c2->rect.w) - c1->rect.x;
				}

				//If the player falls less than a pixel over a collider, it falls (and it looks ok)
				if (error_margin > 1) {

					//Checking Y Axis Collisions
					if (c1->rect.y <= c2->rect.y + c2->rect.h && c1->rect.y >= c2->rect.y + c2->rect.h - 20/*- ((velocity.y*-1)-velocity.y)*/) { //Colliding down (jumping)
																																				 //LOG("BOOL COLLIDING DOWN TRUE");
						if (velocity.y * -1 < 0) {
							velocity.y += (velocity.y*-1);

						}
						jump = false;
						position.y = c2->rect.y + c2->rect.h + 1;
						velocity.y = 0;/*c1->rect.y + c2->rect.h - (c1->rect.y - c2->rect.y) + 3;*/
					}



					if (c1->rect.y + c1->rect.h >= c2->rect.y && c1->rect.y + c1->rect.h <= c2->rect.y + velocity.y) { //Colliding Up (falling)
																													   //LOG("COLLIDING UP");
																													   //LOG("BOOL COLLIDING UP TRUE");
						jump = false;
						velocity.y = 0;
						position.y = c1->rect.y - ((c1->rect.y + c1->rect.h) - c2->rect.y);
						JumpRotation = false;
					}

				}

				//Checking X Axis Collisions
				if (c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x + c1->rect.w <= c2->rect.x + velocity.x) { //Colliding Left (going right)
					LOG("COLLIDING LEFT");
					velocity.x = 0;
					position.x -= (c1->rect.x + c1->rect.w) - c2->rect.x + 4;
				}
				else if (c1->rect.x <= c2->rect.x + c2->rect.w && c1->rect.x >= c2->rect.x + c2->rect.w - ((velocity.x*-1) - velocity.x)) { //Colliding Right (going left)
					LOG("COLLIDING RIGHT");
					velocity.x = 0;
					position.x += (c2->rect.x + c2->rect.w) - c1->rect.x + 4;
				}
		}
			
	}
	
}

void PlayerClass::GodMode(float dt) {

	current_animation = &idle;
	
	
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		    //LOG("<----GODMODE");
		    velocity.x = 10 ;
		    position.x -= velocity.x*(dt/30);
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		    //LOG("GODMODE---->");
			velocity.x = 10 ;
			position.x += velocity.x*(dt / 30);
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		    //LOG("GODMODE UP");
		    velocity.y = -10 ;
		    position.y -= velocity.x*(dt / 30);
	}	
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		    //LOG("GODMODE DOWN");
		    velocity.y =10 ;
		    position.y += velocity.x*(dt / 30);
	}
}

void j1Entity::Die() {
	position.x = App->map->data.start_position.x;
	position.y = App->map->data.start_position.y;
	//current_animation = &death;
	//if(current_animation->Finished())
	App->render->find_player = true;
}

void PlayerClass::Jump() {
	velocity.y -= JumpForce;
}

void PlayerClass::SpeedPower(int boost,int  AvailableDistanceRightNow,float dt) {
	
	if (boost*-1 < 0) {//right movement
		if (AvailableDistanceRightNow>0) {
			position.x += boost;
		}
		else if (AvailableDistanceRightNow <= 0) {
			deceleration = true;
			SpeedPowerActivatedRight = false;
			position.x += AvailableDistanceRightNow;
		}
	}
	if (boost*-1 > 0) {//left movement
		if (AvailableDistanceRightNow > 0) {
			position.x -= boost;
		}
		else if (AvailableDistanceRightNow <= 0) {
			deceleration = true;
			SpeedPowerActivatedLeft = false;
			position.x += AvailableDistanceRightNow;
		}
	}
}