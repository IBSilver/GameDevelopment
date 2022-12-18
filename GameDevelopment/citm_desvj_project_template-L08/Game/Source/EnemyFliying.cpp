#include "EnemyFlying.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Pathfinding.h"
#include "Map.h"

EnemyFlying::EnemyFlying() : Entity(EntityType::ENEMYFLYING)
{
	name.Create("EnemyFlying");
}

EnemyFlying::~EnemyFlying() {

}

bool EnemyFlying::Awake() {

	// Initialize Enemy parameters
	// Get Enemy parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool EnemyFlying::Start() {

	// Initialize animations
	idle.totalFrames = 0;
	move.totalFrames = 0;
	death.totalFrames = 0;

	// Initilize textures
	texture = app->tex->Load(texturePath);
	
	// idle Anim
	for (int i = 0; i < 4; i++) {
		idle.PushBack({ 0 + (i * 48), 144, 48, 48 });
	}
	idle.loop = true;
	idle.speed = 0.1f;

	// move Anim
	for (int i = 0; i < 4; i++) {
		move.PushBack({ 0 + (i * 48), 192, 48, 48 });
	}
	move.loop = true;
	move.speed = 0.1f;

	// death Anim
	for (int i = 0; i < 2; i++) {
		death.PushBack({ 0 + (i * 48), 96, 48, 48 });
	}
	for (int i = 0; i < 4; i++) {
		death.PushBack({ 0 + (i * 48), 54, 48, 48 });
	}
	death.loop = false;
	death.speed = 0.075f;

	currentAnimation = &idle;

	// Add physics to the enemy - initialize physics body
	pbody = app->physics->CreateCircle(position.x + 12, position.y + 12, 12, bodyType::DYNAMIC);

	// Assign enemy class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// Assign collider type
	pbody->ctype = ColliderType::ENEMYFLYING;

	// Initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	deathFx = app->audio->LoadFx("Assets/Audio/Fx/robot_death.wav");

	return true;
}

bool EnemyFlying::Update()
{
	//posInicialY = position.y;

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	currentAnimation->Update();

	app->render->DrawTexture(texture, position.x, position.y - 12, &rect);

	// Add physics to the enemy - updated enemy position using physics
	b2Vec2 vel = b2Vec2(0, 0);

	// Update enemy position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	//SDL_Rect textSection = { 0,0,48,48 };

	return true;
}

bool EnemyFlying::CleanUp()
{
	return true;
}

// Define OnCollision function for the enemy. Check the virtual function on Entity class
void EnemyFlying::OnCollision(PhysBody* physA, PhysBody* physB) {

	// Detect the type of collision
	int enemyhead = app->scene->player->position.y + 10; //Variable que hace que el jugador deba estar encima un numero de pixeles minimo para matar al enemigo
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("ENEMYFLYING Collision PLAYER");
		if (!app->scene->player->GodMode && enemyhead >= position.y) {
			if (!destroyed) {
				if (!app->scene->player->dead) {
					app->audio->PlayFx(app->scene->player->deathFx);
				}
				app->scene->player->dead = true;
			}
		}
		else {
			currentAnimation = &death;
			if (!destroyed) {
				app->audio->PlayFx(deathFx);
			}
			destroyed = true;
			//b2Vec2(0, -GRAVITY_Y);
		}
		break;
	case ColliderType::PLATFORM:
		LOG("ENEMYFLYING Collision PLATFORM");
		break;
	}
}

void EnemyFlying::moveLeft() {
	LOG("LEFT");
	currentAnimation = &move;
	pbody->body->SetLinearVelocity(b2Vec2(-2, 0));
}

void EnemyFlying::moveRight() {
	LOG("RIGHT");
	currentAnimation = &move;
	pbody->body->SetLinearVelocity(b2Vec2(2, 0));
}

void EnemyFlying::moveUp() {
	LOG("UP");
	currentAnimation = &move;
	pbody->body->SetLinearVelocity(b2Vec2(0, -2));
}

void EnemyFlying::moveDown() {
	LOG("DOWN");
	currentAnimation = &move;
	pbody->body->SetLinearVelocity(b2Vec2(0, 2));
}