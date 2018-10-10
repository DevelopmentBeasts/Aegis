#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "Player.h"
#include <math.h>
#include "j1Input.h"

//Destructor

PlayerClass::~PlayerClass() {

}
PlayerClass::PlayerClass() {
	this->name = 'a';
}
bool PlayerClass::Start() {
	
	bool ret = true;
	pugi::xml_parse_result result = PlayerStartFile.load_file("StartPlayerConfig.xml");
	if (result == NULL) {
		LOG("Could not load StartPlayerConfig.xml. pugi error: %s", result.description());
		//ret = false;
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
	uint type1 = PlayerXmlNode.attribute("type").as_uint();
	switch (type1) {
	case 0:
		data.type = FIRE_WISP;
	case 1: 
		data.type = WATER_WISP;
	case 2: 
		data.type = ROCK_WISP;
	case 3:
		data.type = WIND_WISP;
	}
	/*data.xpos = PlayerXmlNode.attribute("xpos").as_uint();
	data.ypos = PlayerXmlNode.attribute("ypos").as_uint();*/
	/*data.yvel = PlayerXmlNode.attribute("yvel").as_uint();*/
	/*data.xvel = PlayerXmlNode.attribute("xvel").as_uint(1);*/
	data.xpos = 300;
	data.ypos = 500;
	data.yvel = 0.0;
	data.xvel = 5.0;


	rect.w = 40;//rect used for the player
	rect.h = 40;

	
	StaminaRect.w = 10;
	StaminaRect.h = 40;

	StaminaRect.x = 30;
	StaminaRect.y = 700;
	
	return ret;
}


bool PlayerClass::Update(float dt) {

	MovePlayer();

	
	return true;

}





void PlayerClass::MovePlayer() {
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		automatic_left = false;
		if (!automatic_right) {
			data.xpos += data.xvel;
		}
		if (jumping) {
			automatic_right = true;
		}
	}
	if (automatic_right) {
		data.xpos += (data.xvel + 1.5);
	}
	//__________________

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		automatic_right = false;
		if (!automatic_left) {
			data.xpos -= data.xvel;
		}
		if (jumping) {
			automatic_left = true;
		}
	}
	if (automatic_left) {
		data.xpos -= (data.xvel + 1.5);
	}

	if (data.ypos == yposaux) {
		automatic_right = false;
		automatic_left = false;
	}
	//_________________

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
		if (data.yvel > -3 && jumping) {
			if (StaminaRect.w >= 121) {
				StaminaRect.w -= 120;
				if (jumping) {
					fall_atack = true;
					jumping = false;
					automatic_left = false;
					automatic_right = false;

				}
			}
		}
	}
	//____________________
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {

		if (jumping == false) {
			//data.yvel = 1.0;
			bot_reached = false;
			top_reached = false;
			jumping = true;
			data.yvel = 10;
			yposaux = data.ypos;
		}
	}
	if (jumping) {
		data.ypos -= data.yvel;
		data.yvel -= 0.3;

		if (data.ypos >= yposaux) {
			data.ypos = yposaux;
			jumping = false;
		}
	}
	if (!jumping) {
		data.yvel = 0.0;
		if (StaminaRect.w <= 300) {
			StaminaRect.w += 1;
		}
	}
	if (fall_atack && !jumping) {
		data.yvel = 25.0;
		data.ypos += data.yvel;
		data.yvel += 2;
		if (data.ypos >= yposaux) {
			fall_atack = false;
			data.ypos = yposaux;
		}
	}
	//_____________

	rect.x = data.xpos;
	rect.y = data.ypos;

	App->render->DrawQuad(rect, 0, 255, 0, 100);

	App->render->DrawQuad(StaminaRect, 0, 0, 255, 100);

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		StaminaRect.y -= 2;
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		StaminaRect.y += 2;
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		StaminaRect.x -= 2;
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		StaminaRect.x += 2;
	}
}