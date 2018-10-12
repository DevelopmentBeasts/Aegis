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
PlayerClass::PlayerClass() {   //DO PUSHBACKS WITH XML
	this->name = 'a';

	pugi::xml_parse_result result = AnimsDoc.load_file("PlayerAnims.xml");

	if (result == NULL) {
		LOG("The xml file that contains the pushbacks for the animations is not working.PlayerAnims.xml. error: %s",result.description());
	}
	
	AnimsNode = AnimsDoc.child("config").child("AnimsCoords").child("idle_left");
	LoadPushbacks(AnimsNode, idle_left);
	AnimsNode = AnimsDoc.child("config").child("AnimsCoords").child("run_left");
	LoadPushbacks(AnimsNode, run_left);

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
		data.type = FIRE_WISP;
	}
	if (type1 == "WATER_WISP") {
		data.type = WATER_WISP;
	}
	if (type1 == "ROCK_WISP") {
		data.type = ROCK_WISP;
	}
	if (type1 == "WISP_WISP") {
		data.type = WIND_WISP;
	}
	data.xpos = PlayerXmlNode.attribute("xpos").as_uint();
	data.ypos = PlayerXmlNode.attribute("ypos").as_uint(); 
	data.yvel = PlayerXmlNode.attribute("yvel").as_uint(); //player stats
	data.xvel = PlayerXmlNode.attribute("xvel").as_uint();

	playerrect.w = PlayerXmlNode.child("playerrect").attribute("w").as_int(); //the rect that contains the player in wich we blit()
	playerrect.h = PlayerXmlNode.child("playerrect").attribute("h").as_int();

	StaminaRect.w = PlayerXmlNode.child("StaminaRect").attribute("w").as_int();
	StaminaRect.h = PlayerXmlNode.child("StaminaRect").attribute("h").as_int();
	StaminaRect.x = PlayerXmlNode.child("StaminaRect").attribute("x").as_int(); //info of the stamina rect
	StaminaRect.y = PlayerXmlNode.child("StaminaRect").attribute("y").as_int();
	//___________________________________________________________________________
	LOG("Resseting anims");
	
	idle_left.Reset();
	run_left.Reset();

	jump_right.Reset();
	jump_left.Reset();
	
	fall_left.Reset();
	attack_right.Reset();
	attack_left.Reset();

	LOG("LOADING PLAYER TEXTURES");

	Textures = App->tex->Load("textures/Fire_Wisp/fireSheet.png");
	current_animation = &idle_left;

	return ret;
}


bool PlayerClass::Update(float dt) {

	MovePlayer();
	PlayerAnims();
	//SDL_RenderCopyEx();
	



	

	return true;

}





void PlayerClass::MovePlayer() {
	
	//by default you are not moving (usefull bools for anim system)
	movingleft = false;
	movingright = false;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		movingleft = false;
		movingright = true;
		automatic_left = false;
		if (!automatic_right) {
			data.xpos += data.xvel;
		}
		if (jumping) {
			automatic_right = true; //automatic left or right is a bool that allows you to stop pressing "D" in the air without stoping the x movement of the character.
		}
	}
	if (automatic_right) {
		data.xpos += (data.xvel + 3); // a little boost of the speed in the air to make the jump more interesting in a plataformer game
	}
	//__________________
	
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) { // the same as "D"
		movingleft = true;
		movingright = false;
		automatic_right = false;
		if (!automatic_left) {
			data.xpos -= data.xvel;
		}
		if (jumping) {
			automatic_left = true;
		}
	}
	if (automatic_left) {
		data.xpos -= (data.xvel + 3);
	}

	if (data.ypos == yposaux) {  //if the y position touches the ground stops the automatic left and right of the jump
		automatic_right = false;
		automatic_left = false;
	}
	//_________________

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
		
		if (data.yvel > -3 && jumping) { // if you are in the top of your jump you can press "S" to fall down doing a smash in the ground
			if (StaminaRect.w >= 121) { 
				StaminaRect.w -= 120; // the stamina bar have 300p and every time you press "S" in the air you use 120p
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

		if (jumping == false) { //this is here because we don't want the player to press w in the air and jump again if it is not allowed
			//data.yvel = 1.0;
			bot_reached = false;
			top_reached = false;
			jumping = true;
			data.yvel = 10;  //initial y speed
			yposaux = data.ypos;
		}
	}
	if (jumping) {
		data.ypos -= data.yvel; //this makes the y speed decrease and become negative to turn back in the top ending up falling down to the ground
		data.yvel -= 0.4;	
		/*playerrect.w = 60;*/
		if (data.ypos >= yposaux) {
			data.ypos = yposaux;
			jumping = false;
		}
	}
	if (!jumping) {
		data.yvel = 0.0;
	/*	playerrect.w = 32;*/
		if (StaminaRect.w <= 300) {  // here the stamina rect grows its points if you don't jump bc you are "in rest"
			StaminaRect.w += 1;
		}
	}
	if (fall_atack && !jumping) { // a big boost of the speed when you want to fall down with a smash to make it more impressive
		data.yvel = 25.0;
		data.ypos += data.yvel;
		data.yvel += 2;
		if (data.ypos >= yposaux) {
			fall_atack = false;
			data.ypos = yposaux;
		}
	}
	//_____________

	playerrect.x = data.xpos;  //here we put the SDL_Rect where we print the sprites in the player position.
	playerrect.y = data.ypos;
	

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
void PlayerClass::PlayerAnims() {

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		SCANCODE_D = true;
		SCANCODE_A = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		SCANCODE_A = true;
		SCANCODE_D = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		SCANCODE_W = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		SCANCODE_S = true;
	}
	

	//LAST DIRECTION RECORD (usefull to continue looking the same side when we are idl and we run or jump
	
	if (SCANCODE_D == true ) {  //RIGHT 
		LastDirectionLeft = false;
		LastDirectionRight = true;	
	}
	if (SCANCODE_A == true) {
		LastDirectionLeft = true;
		LastDirectionRight = false;
	}

	//ANIMS BASED ON LAST DIRECTION RECORD


	// JUMP UP STRAIGHT (going up)

	if (jumping  && (data.yvel > 0) && !movingleft && !movingright && !automatic_left && !automatic_right) {  //looking left
		current_animation = &run_left;

		CurrentAnimationRect = current_animation->GetCurrentFrame();

		SDL_RenderCopyEx(App->render->renderer, Textures, &CurrentAnimationRect, &playerrect, 90.0, NULL, SDL_FLIP_NONE);

		//App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &playerrect, NULL, 0.0, 1, 1, 1.0);
	}

	// JUMP UP STRAIGHT (going down)

	if (jumping && (data.yvel < 0) && !movingleft && !movingright && !automatic_left && !automatic_right) {  //looking left
		current_animation = &run_left;

		CurrentAnimationRect = current_animation->GetCurrentFrame();

		SDL_RenderCopyEx(App->render->renderer, Textures, &CurrentAnimationRect, &playerrect, 90.0, NULL, SDL_FLIP_HORIZONTAL);

		//App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &playerrect, NULL, 0.0, 1, 1, 1.0);
	}



	//GOING DOWN AS ATACK

	if (!jumping && (data.yvel < 0) && !movingleft && !movingright && !automatic_left && !automatic_right) { //looking the left
		current_animation = &run_left;
		CurrentAnimationRect = current_animation->GetCurrentFrame();

		SDL_RenderCopyEx(App->render->renderer, Textures, &CurrentAnimationRect, &playerrect, 90, NULL, SDL_FLIP_HORIZONTAL);

		//App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &playerrect, NULL, 0.0, 1, 1, 1.0);
	}


	//JUMP LEFT
	if (jumping && (data.yvel > 0) && LastDirectionLeft  && automatic_left) {  //JUMPING UP + LEFT
		current_animation = &run_left;

		CurrentAnimationRect = current_animation->GetCurrentFrame();

		SDL_RenderCopyEx(App->render->renderer, Textures, &CurrentAnimationRect, &playerrect, data.yvel*4, NULL, SDL_FLIP_NONE);

		//App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &playerrect, NULL, 0.0, 1, 1, 1.0);
	}
	if (jumping && (data.yvel < 0) && LastDirectionLeft && automatic_left) {  //JUMPING DOWN + LEFT
		current_animation = &run_left;

		CurrentAnimationRect = current_animation->GetCurrentFrame();

		SDL_RenderCopyEx(App->render->renderer, Textures, &CurrentAnimationRect, &playerrect, data.yvel * 4, NULL, SDL_FLIP_NONE);

		//App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &playerrect, NULL, 0.0, 1, 1, 1.0);
	}

	//MOVE LEFT 
	if (!jumping && movingleft) {  //NOT  JUMPING + MOVING LEFT
		current_animation = &run_left;
		
		CurrentAnimationRect = current_animation->GetCurrentFrame();

		SDL_RenderCopyEx(App->render->renderer, Textures, &CurrentAnimationRect, &playerrect, NULL, NULL, SDL_FLIP_NONE);

		//App->render->Blit(Textures, (int)data.xpos, (int)data.ypos,&playerrect, NULL, 0.0, 1, 1, 1.0);
	}


	//MOVE RIGHT 
	if (!jumping && movingright) {  //NOT  JUMPING + MOVING LEFT
		current_animation = &run_left;
		
		CurrentAnimationRect = current_animation->GetCurrentFrame();
		
		SDL_RenderCopyEx(App->render->renderer, Textures, &CurrentAnimationRect,&playerrect , NULL, NULL, SDL_FLIP_HORIZONTAL);

		//App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &playerrect, NULL, data.yvel*4.0, 1, 1, 1.0);
	}
	// JUMP RIGHT

	if (jumping && (data.yvel > 0) && LastDirectionRight && automatic_right) {  //JUMPING UP + LEFT
		current_animation = &run_left;

		CurrentAnimationRect = current_animation->GetCurrentFrame();

		SDL_RenderCopyEx(App->render->renderer, Textures, &CurrentAnimationRect, &playerrect, data.yvel * (-4), NULL, SDL_FLIP_HORIZONTAL);

		//App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &playerrect, NULL, 0.0, 1, 1, 1.0);
	}
	if (jumping && (data.yvel < 0) && LastDirectionRight && automatic_right) {  //JUMPING DOWN + LEFT
		current_animation = &run_left;

		CurrentAnimationRect = current_animation->GetCurrentFrame();

		SDL_RenderCopyEx(App->render->renderer, Textures, &CurrentAnimationRect, &playerrect, data.yvel * (-4), NULL, SDL_FLIP_HORIZONTAL);

		//App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &playerrect, NULL, 0.0, 1, 1, 1.0);
	}

	// IDEL LEFT
	if (LastDirectionLeft && !jumping && !movingleft) { 
		current_animation = &idle_left;

		CurrentAnimationRect = current_animation->GetCurrentFrame();

		SDL_RenderCopyEx(App->render->renderer, Textures, &CurrentAnimationRect, &playerrect, data.yvel * (-4), NULL, SDL_FLIP_NONE);

		//App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &playerrect, NULL, 0.0, 1, 1, 1.0);
	}

	//IDEL RIGHT
	if (LastDirectionRight && !jumping && !movingright) {   
		current_animation = &idle_left;

		CurrentAnimationRect = current_animation->GetCurrentFrame();

		SDL_RenderCopyEx(App->render->renderer, Textures, &CurrentAnimationRect, &playerrect, data.yvel * (-4), NULL, SDL_FLIP_HORIZONTAL);

		//App->render->Blit(Textures, (int)data.xpos, (int)data.ypos, &playerrect, NULL, 0.0, 1, 1, 1.0);
	}






	//App->render->DrawQuad(playerrect, 0, 255, 0, 100); //used for debugging player positions, DO NOT ERASE PLEASE!!!!!!!!!

	App->render->DrawQuad(StaminaRect, 0, 0, 255, 100);
}
void PlayerClass::LoadPushbacks(pugi::xml_node node, Animation &anim) {

	anim.loop = node.attribute("loop").as_bool();
	anim.speed = node.attribute("speed").as_float();
	SDL_Rect rect;
	for (node = node.child("PushBack"); node; node = node.next_sibling("PushBack")) {
		rect.x = node.attribute("x").as_int();
		rect.y = node.attribute("y").as_int();
		rect.w = node.attribute("w").as_int();
		rect.h = node.attribute("h").as_int();
		anim.PushBack({ rect });
	}
	
	
}
