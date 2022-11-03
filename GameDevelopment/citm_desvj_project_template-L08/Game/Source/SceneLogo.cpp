/*
include "SceneLogo.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
//#include "ModuleFadeToBlack.h"

SceneLogo::SceneLogo() : Module()
{

}

SceneLogo::~SceneLogo()
{

}

// Load assets
bool SceneLogo::Start()
{
	timer = 0;
	scape = false;

	bool ret = true;

	//bgTexture = app->tex->Load("Assets/Scenes/logo.png");


	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return ret;
}

bool SceneLogo::CleanUp() {
	app->tex->UnLoad(bgTexture);
	return true;
}

Update_Status SceneLogo::Update()
{
	//10s
	if (timer < 600)
	{
		timer++;
	}
	else if (timer == 600)
	{
		scape = true;
	}


	if (scape)
	{
		App->fade->FadeToBlack(this, (Module*)App->Scene, 30);
	}
	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		App->fade->FadeToBlack(this, (Module*)App->Scene, 30);
	}

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneLogo::PostUpdate()
{
	// Draw everything --------------------------------------
	//app->render->Blit(bgTexture, 0, 0, NULL);

	return Update_Status::UPDATE_CONTINUE;
}
*/