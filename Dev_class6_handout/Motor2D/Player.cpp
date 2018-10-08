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
		ret = false;
	}
	if (ret == true) {
		//Load all Player starter info
		LOG("Loading player info at the start of the game");
	}
	PlayerXmlNode = PlayerStartFile.child("config").child("player");
	if (PlayerXmlNode == NULL) {
		LOG("PENE");
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
	data.xpos = PlayerXmlNode.attribute("xpos").as_uint();
	data.ypos = PlayerXmlNode.attribute("ypos").as_uint();
	/*data.yvel = PlayerXmlNode.attribute("yvel").as_uint();*/
	/*data.xvel = PlayerXmlNode.attribute("xvel").as_uint(1);*/
	data.yvel = 0.0;
	data.xvel = 5.0;
	rect.w = 40;
	rect.h = 80;

	max_height = data.ypos - 300;
	min_height = data.ypos;

	return ret;
}


bool PlayerClass::Update(float dt) {

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		data.xpos += data.xvel;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		data.xpos -= data.xvel;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		data.ypos += data.yvel;
	}
	//if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
	//	
	//	if (jumping == false) {
	//		data.yvel = 10.0;
	//		bot_reached = false;
	//		top_reached = false;
	//		jumping = true;
	//		data.yvel = 10;
	//		yposaux = data.ypos;
	//	}
	//	if (jumping /*&& !top_reached*/) {
	//		data.ypos -= data.yvel;
	//		data.yvel -= 0.15;
	//		usethisbool = true;
	//		if (data.ypos >= yposaux) {
	//			jumping = false;
	//		}
	//	}

	//}
	//if (jumping && usethisbool) {
	//	data.ypos -= data.yvel;
	//	data.yvel -= 0.15;

	//	if (data.ypos >= yposaux) {
	//		jumping = false;
	//	}
	//}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {

		if (jumping == false) {
			data.yvel = 10.0;
			bot_reached = false;
			top_reached = false;
			jumping = true;
			data.yvel = 10;
			yposaux = data.ypos;
		}
		if (jumping /*&& !top_reached*/) {
			data.ypos -= data.yvel;
			data.yvel -= 0.15;
			usethisbool = true;
			if (data.ypos >= yposaux) {
				jumping = false;
			}
		}

	}
	if (jumping && usethisbool) {
		data.ypos -= data.yvel;
		data.yvel -= 0.15;

		if (data.ypos >= yposaux) {
			jumping = false;
		}
	}
	//if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
	//	if (!jumping) {
	//		data.yvel = 12.0f;
	//		bot_reached = false;
	//		top_reached = false;
	//		jumping = true;
	//	}
	//}
	//if (jumping) {
	//	if (!top_reached) {
	//		data.ypos -= data.yvel;
	//		//data.yvel - 0.05;
	//		if (data.ypos <= max_height)
	//			top_reached = true;
	//	}
	//	else if (!bot_reached) {
	//		data.ypos += data.yvel;
	//		if (data.ypos >= min_height) {
	//			data.yvel = 0.0f;
	//			min_height = data.ypos;
	//			max_height = data.ypos - 300;
	//			jumping = false;
	//			bot_reached = true;
	//		}

	//	}
	//}



	rect.x = data.xpos;
	rect.y = data.ypos;

	App->render->DrawQuad(rect, 0, 255, 0, 100);




	return true;

}
//bool PlayerClass::CleanUp()
//{
//	LOG("Freeing Player");
//
//	return true;
//}




//
//bool PlayerClass::Awake(pugi::xml_node& config) {
//	LOG("Loading Player parser");
//
//	bool ret = true;
//
//	folder.create(config.child("folder").child_value());
//	return ret;
//}