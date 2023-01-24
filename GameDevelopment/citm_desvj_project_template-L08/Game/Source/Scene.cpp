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

	enemy = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
	enemy->parameters = config.child("enemy");

	enemyFlying = (EnemyFlying*)app->entityManager->CreateEntity(EntityType::ENEMYFLYING);
	enemyFlying->parameters = config.child("enemyFlying");

	// Instantiate the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");


	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	app->audio->PlayMusic("Assets/Audio/Music/Level1.ogg");

	// Texture to highligh mouse position 
	mouseTileTex = app->tex->Load("Assets/Maps/path_square.png");

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
		app->scene->player->dir = true;
	}

	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		app->LoadCurrentLevelRequest();
		app->scene->player->dead = false;
		app->scene->player->winner = false;
		app->scene->player->dir = true;
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

	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
		if (showPath == false) {
			showPath = true;
		}
		else {
			showPath = false;
		}

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
	
	int pathDestinyX, pathDestinyY;
	pathDestinyX = player->position.x+8;
	pathDestinyY = player->position.y+8;

	iPoint destinyTile = iPoint(0, 0);

	destinyTile = app->map->WorldToMap(pathDestinyX/* - app->render->camera.x*/,
		pathDestinyY/* - app->render->camera.y*/);

	//Convert again the tile coordinates to world coordinates to render the texture of the tile
	iPoint highlightedTileWorld = app->map->MapToWorld(destinyTile.x, destinyTile.y);
	if(showPath)
		app->render->DrawTexture(mouseTileTex, highlightedTileWorld.x, highlightedTileWorld.y);

	int originX, originY;
	originX = enemy->position.x+8;
	originY = enemy->position.y+8;

	int originFX, originFY;
	originFX = enemyFlying->position.x;
	originFY = enemyFlying->position.y;

	origin = app->map->WorldToMap(originX, originY);

	originF = app->map->WorldToMap(originFX, originFY);
	
	app->pathfinding->ClearLastPath();
	if(!enemy->destroyed )
		app->pathfinding->CreatePath(origin, destinyTile);

	// Get the latest calculated path and draw
	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{
		pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		if(showPath==true)
			app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
	}

	if (!enemy->destroyed)
		app->pathfinding->CreatePath(originF, destinyTile);

	// Get the latest calculated path and draw
	const DynArray<iPoint>* pathF = app->pathfinding->GetLastPath();
	for (uint i = 0; i < pathF->Count(); ++i)
	{
		pos = app->map->MapToWorld(pathF->At(i)->x, pathF->At(i)->y);
		if (showPath == true)
			app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
	}

	if (enemy->destroyed == false && app->pathfinding->CreatePath(origin, destinyTile)>1) {
		LOG("%d, %d", enemy->position.x, app->map->MapToWorld(path->At(1)->x, path->At(1)->y).x);
		if (enemy->position.x+8 >= app->map->MapToWorld(path->At(0)->x, path->At(0)->y).x)
			enemy->moveLeft();
		if (enemy->position.x+8 < app->map->MapToWorld(path->At(1)->x, path->At(1)->y).x)
			enemy->moveRight();
	}

	if (enemyFlying->destroyed == false && app->pathfinding->CreatePath(originF, destinyTile) > 1) {
		LOG("%d, %d", enemyFlying->position.x, app->map->MapToWorld(path->At(1)->x, path->At(1)->y).x);
		if (enemyFlying->position.x + 6 >= app->scene->player->position.x)
			enemyFlying->moveLeft();
		if (enemyFlying->position.x + 6 < app->scene->player->position.x)
			enemyFlying->moveRight();
		if (enemyFlying->position.y + 6 < app->map->MapToWorld(path->At(1)->x, path->At(1)->y).y)
			enemyFlying->moveUp();
		if (enemyFlying->position.y + 6 < app->map->MapToWorld(path->At(1)->x, path->At(1)->y).y)
			enemyFlying->moveDown();
	}

	// Debug pathfinding
	iPoint originScreen = app->map->MapToWorld(origin.x, origin.y);
	app->render->DrawTexture(originTex, originScreen.x, originScreen.y);

	iPoint originScreenF = app->map->MapToWorld(originF.x, originF.y);
	app->render->DrawTexture(originTex, originScreenF.x, originScreenF.y);

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
