#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");

	pugi::xml_parse_result result=ui_data.load_file("UiConfig.xml");
	
	pugi::xml_node icon_data= ui_data.child("properties").child("icons");

	settings = { icon_data.child("settings").attribute("x").as_int(),
				icon_data.child("settings").attribute("y").as_int(),
				icon_data.child("settings").attribute("w").as_int(),
				icon_data.child("settings").attribute("h").as_int() };

	exit = { icon_data.child("exit").attribute("x").as_int(),
				icon_data.child("exit").attribute("y").as_int(),
				icon_data.child("exit").attribute("w").as_int(),
				icon_data.child("exit").attribute("h").as_int() };

	gem = { icon_data.child("gem").attribute("x").as_int(),
				icon_data.child("gem").attribute("y").as_int(),
				icon_data.child("gem").attribute("w").as_int(),
				icon_data.child("gem").attribute("h").as_int() };



}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

bool j1Gui::Update(float dt)
{
	p2List_item<UiDragBar*>* ui_dragbar;
	if (ui_bars.count() > 0)
	{
		for (ui_dragbar = ui_bars.start; ui_dragbar != nullptr; ui_dragbar = ui_dragbar->next)
		{
			//Can't find an enum for the Mouse buttons, so I use numbers instead: 1 -> Left click, 2 -> Roll click , 3 -> Right click
			if (ui_dragbar->data->active &&ui_dragbar->data->MouseOnTop() && App->input->GetMouseButtonDown(1) == KEY_REPEAT)
				ui_dragbar->data->Act();
		}
	}

	p2List_item<UiActiveElement*>* active_element;

	if (active_elements.count() > 0)
	{
		for (active_element = active_elements.start; active_element != nullptr; active_element = active_element->next)
		{
			if (active_element->data->active) {

				active_element->data->Update();

				//Can't find an enum for the Mouse buttons, so I use numbers instead: 1 -> Left click, 2 -> Roll click , 3 -> Right click
				if (active_element->data->MouseOnTop() && App->input->GetMouseButtonDown(1) == KEY_UP)
					active_element->data->Act();

			}
		}
	}


	//DRAW
	p2List_item<UiElement*>* ui_element;

	if (ui_elements.count() > 0)
	{
		for (ui_element = ui_elements.start; ui_element != nullptr; ui_element = ui_element->next)
		{
			ui_element->data->position.x = ui_element->data->draw_position.x - App->render->camera.x;
			ui_element->data->position.y = ui_element->data->draw_position.y - App->render->camera.y;

			if (ui_element->data->active) {

				//If two elements are nested, the parent will take care of the Draw() function
				if (ui_element->data->parent == nullptr)
					ui_element->data->Draw();
			}
		}
	}

	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{

	

	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------
UiImage* j1Gui::AddImage(iPoint position, SDL_Rect section)
{
	UiImage* item = new UiImage(position, section);

	ui_elements.add(item);

	return item;
}

UiLabel* j1Gui::AddLabel(iPoint position, char* label, _TTF_Font* font)
{
	UiLabel* item = new UiLabel(position, label, font);;

	ui_elements.add(item);

	return item;
}

UiButton* j1Gui::AddButton(iPoint position, ButtonSize size, j1Module* callback, ButtonFunction function)
{
	UiButton* item = new UiButton(position, size, callback, function);
	item->Start();

	ui_elements.add(item);
	active_elements.add(item);

	return item;
}

UiCheckBox* j1Gui::AddCheckBox(iPoint position, bool* boolean, char* label)
{
	UiCheckBox* item = new UiCheckBox(position, *boolean, label);
	item->Start();

	ui_elements.add(item);
	active_elements.add(item);

	return item;
}

UiWindow* j1Gui::AddWindow(iPoint position)
{
	UiWindow* item = new UiWindow(position);
	
	ui_elements.add(item);

	return item;
}

UiDragBar* j1Gui::AddDragBar(iPoint position)
{
	UiDragBar* item = new UiDragBar(position);

	ui_bars.add(item);
	ui_elements.add(item);

	return item;
}

void j1Gui::DrawUi() const
{

	p2List_item<UiElement*>* item;

	if (ui_elements.count() > 0)
	{
		for (item = ui_elements.start; item != nullptr; item = item->next)
		{
			//If two elements are nested, the parent will take care of the Draw() function
			if (item->data->parent == nullptr && item->data->active)
				item->data->Draw();
		}
	}
}

//UI Element
UiElement::UiElement(iPoint position) :position(position), draw_position(position)
{
	active = true;
}


//=====================================================================================================================
//UI Image
UiImage::UiImage(iPoint position, SDL_Rect section) : UiElement(position), section(section)
{
	atlas = App->gui->GetAtlas();
	type = UiType::IMAGE;
}

void UiImage::Draw()
{
	App->render->Blit(atlas, position.x, position.y, &section);
}

void UiImage::CleanUp()
{
	atlas = nullptr;
}


//=====================================================================================================================
//UI label
UiLabel::UiLabel(iPoint position, char* label, _TTF_Font* font) :UiElement(position), font(font)
{
	text = label;

	type = UiType::LABEL;

}

void UiLabel::CleanUp()
{
	font = nullptr;
}

void UiLabel::Draw()
{
	//White
	SDL_Color color = { 150,100,220,0 };

	SDL_Texture* texture = App->fonts->Print(text.GetString(), color, font);

	App->render->Blit(texture, position.x, position.y);

	App->tex->UnLoad(texture);
	texture = nullptr;
}

//=====================================================================================================================
//Ui active element
UiActiveElement::UiActiveElement(iPoint position) :UiElement(position)
{
	atlas = App->gui->GetAtlas();
}

bool UiActiveElement::MouseOnTop() const
{
	bool ret = false;

	iPoint mouse_position;
	App->input->GetMousePosition(mouse_position.x, mouse_position.y);

	if (mouse_position.x > action_area.x &&
		mouse_position.x < action_area.x + action_area.w &&
		mouse_position.y > action_area.y &&
		mouse_position.y < action_area.y + action_area.h)
		ret = true;


	return ret;
}


//=====================================================
//UI button
UiButton::UiButton(iPoint position, ButtonSize size, j1Module* callback, ButtonFunction function) : UiActiveElement(position), callback(callback), function(function)
{
	//position of the action area
	action_area.x = position.x;
	action_area.y = position.y;


	pugi::xml_node button_data;

	switch (size)
	{
	case ButtonSize::BIG:
		button_data = App->gui->ui_data.child("properties").child("button_properties").child("big_button");
		break;

	case ButtonSize::SMALL:
		button_data = App->gui->ui_data.child("properties").child("button_properties").child("small_button");
		break;

	case ButtonSize::EXTRA:
		button_data = App->gui->ui_data.child("properties").child("button_properties").child("large_button");
		break;
	case ButtonSize::MICRO:
		button_data = App->gui->ui_data.child("properties").child("button_properties").child("micro_button");
		break;

	}


	section_idle = {	button_data.child("idle_section").attribute("x").as_int(),
						button_data.child("idle_section").attribute("y").as_int(),
						button_data.child("idle_section").attribute("w").as_int(),
						button_data.child("idle_section").attribute("h").as_int() };

	section_selected = { button_data.child("active_section").attribute("x").as_int(),
						button_data.child("active_section").attribute("y").as_int(),
						button_data.child("active_section").attribute("w").as_int(),
						button_data.child("active_section").attribute("h").as_int() };

	
	action_area.w = section_idle.w;
	action_area.h = section_idle.h;

	type = UiType::BUTTON;
}

void UiButton::Start()
{
	current_section = &section_idle;
}

void UiButton::Update()
{
	if (MouseOnTop())
	{
		current_section = &section_selected;
	}
	else
		current_section = &section_idle;

}

void UiButton::Draw()
{

	App->render->Blit(atlas, position.x, position.y, current_section);

	if (son_element != nullptr)
		son_element->Draw();

}

void UiButton::Act()
{
	if (callback!=nullptr)
		callback->ButtonAction(this);
}

UiLabel* UiButton::NestLabel(iPoint label_position, char* text)
{
	assert(son_element == nullptr, "Can't nest more than ONE UiElement to a button!");

	UiLabel* item = App->gui->AddLabel({ label_position.x + position.x, label_position.y + position.y }, text);
	item->parent = this;

	son_element = item;

	return item;
}

UiImage* UiButton::NestImage(iPoint image_position, SDL_Rect section)
{
	assert(son_element == nullptr, "Can't nest more than ONE UiElement to a button!");

	UiImage* item = App->gui->AddImage({ image_position.x + position.x, image_position.y + position.y }, section);
	item->parent = this;

	son_element = item;

	return item;

}

//==========================================================================================
//Ui Checkbox

UiCheckBox::UiCheckBox(iPoint position, bool &boolean, char* label) : UiActiveElement(position), bool_ptr(boolean), label_text(label)
{
	section_idle = { 992,847,20,20 };
	section_selected = { 992, 814, 20, 20 };

	action_area = { position.x, position.y, 20,20 };

	type = UiType::CHECKBOX;

}

void UiCheckBox::Start()
{
	current_section = &section_idle;

	label = App->gui->AddLabel({ position.x + 30, position.y }, label_text);
	label->parent = this;

	tick = App->gui->AddImage({ position.x + 0,position.y + 0 }, { 935,402,28,28 });
	tick->parent = this;
}

void UiCheckBox::Draw()
{
	App->render->Blit(atlas, position.x, position.y, current_section);

	if (bool_ptr == true)
		tick->Draw();

	label->Draw();
}

void UiCheckBox::Update()
{

	if (MouseOnTop())
		current_section = &section_selected;
	else
		current_section = &section_idle;


}

void UiCheckBox::Act()
{

	bool_ptr = !bool_ptr;

}

//=================================================
//Window
UiWindow::UiWindow(iPoint position) : UiElement(position)
{
	atlas = App->gui->GetAtlas();

	pugi::xml_node window_config= App->gui->ui_data.child("properties").child("window_properties").child("window_section");

	section.x = window_config.attribute("x").as_int();
	section.y = window_config.attribute("y").as_int();
	section.w = window_config.attribute("w").as_int();
	section.h = window_config.attribute("h").as_int();
}

void UiWindow::CleanUp()
{
	p2List_item<UiElement*>* item;

	for (item = element_list.start; item != nullptr; item = item->next)
	{
		item->data->CleanUp();
		RELEASE(item->data);
	}
	element_list.clear();
}

void UiWindow::Draw()
{
	App->render->Blit(atlas, position.x, position.y, &section);

	p2List_item<UiElement*>* item;
	for (item = element_list.start; item != nullptr; item = item->next)
	{
		item->data->Draw();
	}
}

UiImage* UiWindow::NestImage(iPoint image_position, SDL_Rect section)
{
	UiImage* item = App->gui->AddImage({ image_position.x + position.x, image_position.y + position.y }, section);
	item->parent = this;

	element_list.add(item);

	return item;
}

UiLabel* UiWindow::NestLabel(iPoint label_position, char*text, _TTF_Font* font)
{
	UiLabel* item = App->gui->AddLabel({ label_position.x + position.x, label_position.y + position.y }, text, font);
	item->parent = this;

	element_list.add(item);

	return item;
}

UiButton* UiWindow::NestButton(iPoint button_position, ButtonSize size, j1Module* callback, ButtonFunction function)
{
	UiButton* item =App->gui->AddButton({ button_position.x + position.x, button_position.y + position.y }, size, callback, function);
	item->parent = this;

	element_list.add(item);

	return item;
}

UiDragBar* UiWindow::NestBar(iPoint bar_position)
{
	UiDragBar* item = App->gui->AddDragBar({ position.x + bar_position.x, position.y + bar_position.y });
	
	element_list.add(item);
	item->parent = this;

	return item;
}

void UiWindow::ChangeState()
{
	active = !active;

	p2List_item<UiElement*>* item;
	
	if (element_list.count()>0)
		for (item = element_list.start; item != nullptr; item = item->next)
			item->data->active = !item->data->active;
}


//==================================================================================================================
//Dragable bar

UiDragBar::UiDragBar(iPoint position) :UiElement(position)
{
	pugi::xml_node uiproperties = App->gui->ui_data.child("properties").child("dragable_bar").child("bar_section");

	section = { uiproperties.attribute("x").as_int(),
					uiproperties.attribute("y").as_int(),
					uiproperties.attribute("w").as_int() ,
					uiproperties.attribute("h").as_int() };

	action_area.x = position.x;
	action_area.y = position.y;
	action_area.w = section.w;
	action_area.h = section.h;

	bar_length = section.w;

	button = App->gui->AddButton({ position.x+int(bar_length)/2,position.y }, ButtonSize::MICRO);
	button->parent = this;
	value = 0.5f;

	atlas = App->gui->GetAtlas();
}

void UiDragBar::CleanUp()
{
	atlas = nullptr;
}

bool UiDragBar::MouseOnTop() const
{
	bool ret = false;

	iPoint mouse_position;
	App->input->GetMousePosition(mouse_position.x, mouse_position.y);

	if (mouse_position.x > action_area.x &&
		mouse_position.x < action_area.x + action_area.w &&
		mouse_position.y > action_area.y &&
		mouse_position.y < action_area.y + action_area.h)
		ret = true;


	return ret;
}

void UiDragBar::Draw()
{
	App->render->Blit(atlas, position.x, position.y, &section);

	button->Draw();
}

void UiDragBar::Act()
{
	iPoint mouse_position;
	App->input->GetMousePosition(mouse_position.x, mouse_position.y);

	value = float(float(button->position.x -position.x)/bar_length);

	button->draw_position.x = mouse_position.x;
}