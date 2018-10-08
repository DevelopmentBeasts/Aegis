#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "Player.h"
#include <math.h>

//Destructor

PlayerClass::~PlayerClass() {

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
	//Asigment of the values
	uint type1 = PlayerStartFile.child("player").attribute("type").as_uint();
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
	data.xpos = PlayerStartFile.child("player").attribute("xpos").as_uint();
	data.ypos = PlayerStartFile.child("player").attribute("ypos").as_uint();


	

	return ret;
}






bool PlayerClass::Awake(pugi::xml_node& config) {
	LOG("Loading Player parser");

	bool ret = true;

	folder.create(config.child("folder").child_value());
	return ret;
}