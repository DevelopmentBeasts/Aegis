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
	rect.w = 40;
	rect.h = 40;

	max_height = data.ypos - 300;
	min_height = data.ypos;

	return ret;
}


bool PlayerClass::Update(float dt) {

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
		data.xpos += (data.xvel+1.5);
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
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		data.ypos -= data.yvel;
	}
	

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
	if (jumping /*&& usethisbool*/) {
		data.ypos -= data.yvel;
		data.yvel -= 0.3;

		if (data.ypos >= yposaux) {
			data.ypos = yposaux;
			jumping = false;
		}
	}
	



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