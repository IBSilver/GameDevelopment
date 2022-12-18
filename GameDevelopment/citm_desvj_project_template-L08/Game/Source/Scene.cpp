#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "Pathfinding.h"

#include "Defs.h"
#include "Log.h"
#include "DynArray.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	for (pugi::xml_node enemyNode = config.child("enemy"); enemyNode; enemyNode = enemyNode.next_sibling("enemy"))
	{
		Enemy* enemy = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		enemy->parameters = enemyNode;
	}
	// Instantiate the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");


	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//img = app->tex->Load("Assets/Textures/test.png");
	app->audio->PlayMusic("Assets/Audio/Music/Level1.ogg");

	// Texture to highligh mouse position 
	mouseTileTex = app->tex->Load("Assets/Maps/path_square.png");

	// Texture to show path origin 
	originTex = app->tex->Load("Assets/Maps/x_square.png");

	// Load map
	bool retLoad = app->map->Load();

	// L12 Create walkability map
	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}

	// Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	app->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	// Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		app->LoadLevel1Request();
		app->scene->player->dead = false;
		app->scene->player->winner = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		app->LoadCurrentLevelRequest();
		app->scene->player->dead = false;
		app->scene->player->winner = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		app->LoadGameRequest();
		app->scene->player->dead = false;
		app->scene->player->winner = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN) {
		app->scene->player->dead = true;
		app->scene->player->winner = false;
		app->audio->PlayFx(app->scene->player->deathFx);
	}

	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {
		app->scene->player->winner = true;
		app->scene->player->dead = false;
		app->audio->PlayFx(app->scene->player->winFx);
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		if (app->scene->player->GodMode == false) {
			app->scene->player->GodMode = true;
			app->scene->player->jumpTimer = 999;
		}
		else {
			app->scene->player->GodMode = false;
			app->scene->player->jumpTimer = 30;
		}

	// Move camera
	/*if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 50;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 50;*/

	/*if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 5;

	else if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 5;*/

	// Logo scene
	if (Logo == true) {
		app->render->camera.y = 1000;
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
			timer = 250;
		if (timer < 250)
		{
			timer++;
		}
		else {
			timer = 0;
			Title = true;
			Logo = false;
		}
	}

	// Title scene
	else if (Title == true) {
		app->render->camera.y = 2000;
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
			Title = false;
		}
	}

	// Dead scene
	else if (app->scene->player->dead == true)
	{
		if (timer < 50)
		{
			timer++;
		}
		else {
			app->render->camera.x = 0;
			app->render->camera.y = 3000;
			if (timer < 250)
			{
				timer++;
			}
			else {
				timer = 0;
				app->LoadCurrentLevelRequest();
				app->scene->player->dead = false;
			}
		}
	}

	else {
		app->render->camera.y = -000;
		if (player->position.x >= 250) {
			app->render->camera.x = -player->position.x + 250;
			if (player->position.x >= 2810) {
				app->render->camera.x = -2560;
			}
		}
		else {
			app->render->camera.x = -10;
		}
	}

	// Draw map
	app->map->Draw();

	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);

	iPoint mouseTile = iPoint(0, 0);

	mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x,
		mouseY - app->render->camera.y);
	

	//Convert again the tile coordinates to world coordinates to render the texture of the tile	******************
	iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x, mouseTile.y);
	app->render->DrawTexture(mouseTileTex, highlightedTileWorld.x, highlightedTileWorld.y);

	//Test compute path function
	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (originSelected == true)
		{
			app->pathfinding->CreatePath(origin, mouseTile);
			originSelected = false;
		}
		else
		{
			origin = mouseTile;
			originSelected = true;
			app->pathfinding->ClearLastPath();
		}
	}

	// L12: Get the latest calculated path and draw
	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
		LOG("pos.x = %d, pos.y = %d", pos.x, pos.y);
	}

	// L12: Debug pathfinding
	iPoint originScreen = app->map->MapToWorld(origin.x, origin.y);
	app->render->DrawTexture(originTex, originScreen.x, originScreen.y);
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
