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
	//idleR.totalFrames = 0;
	//idleL.totalFrames = 0;
	//left.totalFrames = 0;
	//right.totalFrames = 0;
	//death.totalFrames = 0;

	// Initilize textures
	texture = app->tex->Load(texturePath);

	// idleR Anim
	//for (int i = 0; i < 3; i++) {
	//	idleR.PushBack({ 0 + (i * 48), 336, 48, 48 });
	//}
	//idleR.loop = true;
	//idleR.speed = 0.1f;

	// idleL Anim
	//for (int i = 3; i >= 0; i--) {
	//	idleL.PushBack({ 576 + (i * 48), 336, 48, 48 });
	//}
	//idleL.loop = true;
	//idleL.speed = 0.1f;

	// right Anim
	//for (int i = 0; i < 5; i++) {
	//	right.PushBack({ 0 + (i * 48), 480, 48, 48 });
	//}
	//right.loop = true;
	//right.speed = 0.2f;

	// left Anim
	//for (int i = 5; i >= 0; i--) {
	//	left.PushBack({ 480 + (i * 48), 480, 48, 48 });
	//}
	//left.loop = true;
	//left.speed = 0.2f;


	// death Anim
	//for (int i = 0; i < 5; i++) {
	//	death.PushBack({ 0 + (i * 48), 192, 48, 48 });
	//}
	//death.loop = false;
	//death.speed = 0.2f;

	//currentAnimation = &idleR;





	// Add physics to the enemy - initialize physics body
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC);

	// Assign enemy class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	// initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	//deathFx = app->audio->LoadFx("Assets/Audio/Fx/biker_hurt.wav");

	return true;
}

bool Enemy::Update()
{
	//posInicialY = position.y;

	//SDL_Rect rect = currentAnimation->GetCurrentFrame();
	//currentAnimation->Update();

	// Render the texture
	app->render->DrawTexture(texture, position.x, position.y - 16, NULL/*&rect*/);

	// Add physics to the enemy - updated enemy position using physics
	//int speed = 10;
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);

	// Modify the position of the player using arrow keys

	//if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !dead && !winner && !app->scene->Logo && !app->scene->Title) {
	//	vel = b2Vec2(-speed * 0.8, -GRAVITY_Y);
	//	currentAnimation = &left;
	//	dir = false;
	//}
	//else {
	//	left.Reset();
	//}

	//if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !dead && !winner && !app->scene->Logo && !app->scene->Title) {
	//	vel = b2Vec2(speed * 0.8, -GRAVITY_Y);
	//	currentAnimation = &right;
	//	dir = true;
	//}
	//else {
	//	right.Reset();
	//}

	
	// Gameplay conditions
	//if (dead && !GodMode) {
	//	currentAnimation = &death;
	//	dir = true;
	//}
	//else {
	//	death.Reset();
	//}

	//if (winner && !GodMode) {
	//	currentAnimation = &win;
	//	dir = true;
	//}
	//else {
	//	win.Reset();
	//}

	// idleAnim condition
	//currentAnimation = &idleR;


	// Set the velocity of the pbody of the player
	//pbody->body->SetLinearVelocity(vel);

	// Update enemy position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	SDL_Rect textSection = { 0,0,48,48 };


	return true;
}

bool Enemy::CleanUp()
{
	return true;
}


// Define OnCollision function for the player. Check the virtual function on Entity class
void Enemy::OnCollision(PhysBody* physA, PhysBody* physB) {

	// Detect the type of collision
	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}
