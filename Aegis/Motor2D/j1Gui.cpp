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

// Called after all Updates
bool j1Gui::PostUpdate()
{

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
			if (ui_element->data->active) {

				//If two elements are nested, the parent will take care of the Draw() function
				if (ui_element->data->parent == nullptr)
					ui_element->data->Draw();
			}
		}
	}

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

UiButton* j1Gui::AddButton(iPoint position, ButtonSize size, j1Module* callback)
{
	UiButton* item = new UiButton(position, size, callback);
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
UiElement::UiElement(iPoint position) :position(position)
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
	App->render->Blit(atlas, position.x, position.y, &section, 0.0f);
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
	SDL_Color color = { 255,255,255,100 };

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
UiButton::UiButton(iPoint position, ButtonSize size, j1Module* callback) : UiActiveElement(position), callback(callback)
{
	switch (size)
	{
	case ButtonSize::BIG:
		section_idle = { 5,115,215,71 };
		section_selected = { 416,171,215,71 };

		action_area.x = position.x;
		action_area.y = position.y;
		action_area.w = 215;
		action_area.h = 71;
		break;

	case ButtonSize::SMALL:
		section_idle = { 813,77,38,38 };
		section_selected = { 755,527,38,38 };

		action_area.x = position.x;
		action_area.y = position.y;
		action_area.w = 38;
		action_area.h = 38;
		break;
	}

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
	//callback->ButtonAction(this);
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