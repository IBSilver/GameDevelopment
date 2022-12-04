#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Item::Item() : Entity(EntityType::ITEM)
{
	name.Create("item");
}

Item::~Item() {}

bool Item::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Item::Start() {

	money.totalFrames = 0;

	// Initilize textures
	texture = app->tex->Load(texturePath);

	// Money Anim
	for (int i = 0; i < 5; i++) {
		money.PushBack({ 0 + (i * 24), 0, 24, 24 });
	}
	money.loop = true;
	money.speed = 0.15f;

	currentAnimation = &money;
	
	// Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateRectangleSensor(position.x, position.y, 16, 16, bodyType::STATIC);

	// Assign item class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// Assign collider type
	pbody->ctype = ColliderType::ITEM;

	// Initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.wav");

	return true;
}

bool Item::Update()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	currentAnimation->Update();

	if (!isPicked) {
		app->render->DrawTexture(texture, position.x + 6, position.y + 6, &rect);
	}

	// Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	return true;
}

bool Item::CleanUp()
{
	return true;
}

void Item::OnCollision(PhysBody* physA, PhysBody* physB) {

	// Detect the collision with the player
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		physA->body->DestroyFixture(physA->body->GetFixtureList());
		isPicked = true;
		break;
	}
}