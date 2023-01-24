#include "Enemy.h"
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

	// Initialize animations
	idleR.totalFrames = 0;
	idleL.totalFrames = 0;
	left.totalFrames = 0;
	right.totalFrames = 0;
	death.totalFrames = 0;

	// Initilize textures
	texture = app->tex->Load(texturePath);

	// idleR Anim
	for (int i = 0; i < 4; i++) {
		idleR.PushBack({ 0 + (i * 48), 0, 48, 48 });
	}
	idleR.loop = true;
	idleR.speed = 0.1f;

	// idleL Anim
	for (int i = 0; i < 4; i++) {
		idleL.PushBack({ 192 + (i * 48), 144, 48, 48 });
	}
	idleL.loop = true;
	idleL.speed = 0.1f;

	// right Anim
	for (int i = 0; i < 4; i++) {
		right.PushBack({ 0 + (i * 48), 192, 48, 48 });
	}
	right.loop = true;
	right.speed = 0.1f;

	// left Anim
	for (int i = 0; i < 4; i++) {
		left.PushBack({ 192 + (i * 48), 192, 48, 48 });
	}
	left.loop = true;
	left.speed = 0.1f;

	// death Anim
	for (int i = 0; i < 2; i++) {
		death.PushBack({ 0 + (i * 48), 96, 48, 48 });
	}
	for (int i = 0; i < 4; i++) {
		death.PushBack({ 0 + (i * 48), 48, 48, 48 });
	}
	death.loop = false;
	death.speed = 0.075f;

	currentAnimation = &idleL;

	// Add physics to the enemy - initialize physics body
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 12, bodyType::DYNAMIC);

	// Assign enemy class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	// Initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	deathFx = app->audio->LoadFx("Assets/Audio/Fx/robot_death.wav");

	return true;
}

bool Enemy::Update()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	currentAnimation->Update();

	// Render the texture
	app->render->DrawTexture(texture, position.x, position.y - 16, &rect);
	
	// Add physics to the enemy - updated enemy position using physics
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);

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
	int enemyhead = app->scene->player->position.y + 10; //Variable que hace que el jugador deba estar encima un numero de pixeles minimo para matar al enemigo
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("ENEMY Collision PLAYER");
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
		}
		break;
	case ColliderType::PLATFORM:
		LOG("ENEMY Collision PLATFORM");
		break;
	}

}

void Enemy::moveLeft() {
	LOG("LEFT");
	currentAnimation = &left;
	dir = false;
	pbody->body->SetLinearVelocity(b2Vec2(-2, -GRAVITY_Y));
}

void Enemy::moveRight() {
	LOG("RIGHT");
	currentAnimation = &right;
	dir = true;
	pbody->body->SetLinearVelocity(b2Vec2(2, -GRAVITY_Y));
}