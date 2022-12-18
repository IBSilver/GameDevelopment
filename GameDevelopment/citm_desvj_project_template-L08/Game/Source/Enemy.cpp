#include "Enemy.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Enemy::Enemy() : Entity(EntityType::ENEMY)
{
	name.Create("Enemy");
}

Enemy::~Enemy() {

}

bool Enemy::Awake() {

	// Initialize Enemy parameters
	// Get Enemy parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Enemy::Start() {

	//dir = true;

	// Initialize animations
	idleR.totalFrames = 0;
	idleL.totalFrames = 0;
	//left.totalFrames = 0;
	//right.totalFrames = 0;
	//death.totalFrames = 0;

	// Initilize textures
	texture = app->tex->Load(texturePath);

	// idleR Anim
	for (int i = 0; i < 4; i++) {
		idleR.PushBack({ 0 + (i * 48), 0, 48, 48 });
	}
	idleR.loop = true;
	idleR.speed = 0.1f;

	// idleL Anim
	for (int i = 3; i >= 0; i--) {
		idleL.PushBack({ 193 + (i * 48), 0, 48, 48 });
	}
	idleL.loop = true;
	idleL.speed = 0.1f;

	currentAnimation = &idleL;

	// Add physics to the enemy - initialize physics body
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC);

	// Assign enemy class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	// Initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	//deathFx = app->audio->LoadFx("Assets/Audio/Fx/biker_hurt.wav");

	return true;
}

bool Enemy::Update()
{
	//posInicialY = position.y;

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	currentAnimation->Update();

	// Render the texture
	if (!destroyed) {
		app->render->DrawTexture(texture, position.x, position.y - 16, &rect);
	}
	
	// Add physics to the enemy - updated enemy position using physics
	//int speed = 10;
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);

	// idleAnim condition
	currentAnimation = &idleL;

	// Set the velocity of the pbody of the player
	//pbody->body->SetLinearVelocity(vel);

	// Update enemy position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	//SDL_Rect textSection = { 0,0,48,48 };

	return true;
}

bool Enemy::CleanUp()
{
	return true;
}


// Define OnCollision function for the enemy. Check the virtual function on Entity class
void Enemy::OnCollision(PhysBody* physA, PhysBody* physB) {

	// Detect the type of collision
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("ENEMY Collision PLAYER");
		LOG("ENEMY Y: %d", position.y);
		LOG("PLAYER Y: %d", app->scene->player->position.y);
		if (!app->scene->player->GodMode && app->scene->player->position.y >= position.y) {
			app->scene->player->dead = true;
			//app->audio->PlayFx(deathFx);
		}
		else {
			physA->body->DestroyFixture(physA->body->GetFixtureList());
			destroyed = true;
		}
		break;
	}
}
