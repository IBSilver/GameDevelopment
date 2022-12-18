#include "EntityManager.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyFlying.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr; 

	// Instantiate entity according to the type and add the new entoty it to the list of Entities

	switch (type)
	{

	case EntityType::PLAYER:
		entity = new Player();
		break;

	case EntityType::ITEM:
		entity = new Item();
		break;

	case EntityType::ENEMY:
		entity = new Enemy();
		break;

	case EntityType::ENEMYFLYING:
		entity = new EnemyFlying();
		break;

	default: break;
	}

	// Created entities are added to the list
	AddEntity(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.Del(item);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Update();
	}

	return ret;
}

// Implement a method to load the state
// for now load camera's x and y
bool EntityManager::LoadState(pugi::xml_node& data)
{
	int x = 16+data.child("player").attribute("x").as_int();
	int y = 16+data.child("player").attribute("y").as_int();

	app->scene->player->pbody->body->SetTransform({ PIXEL_TO_METERS(x), PIXEL_TO_METERS(y) }, 0);

	if (!app->scene->enemy->destroyed) {
		int xE = 16 + data.child("enemy").attribute("x").as_int();
		int yE = 16 + data.child("enemy").attribute("y").as_int();

		app->scene->player->pbody->body->SetTransform({ PIXEL_TO_METERS(xE), PIXEL_TO_METERS(yE) }, 0);
	}

	return true;
}

// Create a method to save the state of the renderer
// using append_child and append_attribute
bool EntityManager::SaveState(pugi::xml_node& data)
{
	pugi::xml_node pl = data.append_child("player");

	pl.append_attribute("x") = app->scene->player->position.x;
	pl.append_attribute("y") = app->scene->player->position.y;

	if (!app->scene->enemy->destroyed) {
		app->scene->enemy->destroyed = false;
		pugi::xml_node we = data.append_child("walkingEnemy");

		we.append_attribute("x") = app->scene->enemy->position.x;
		we.append_attribute("y") = app->scene->enemy->position.y;
	}
	return true;
}