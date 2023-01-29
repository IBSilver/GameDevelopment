#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	// Initialize Player parameters
	// Get Player parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();


	return true;
}

bool Player::Start() {

	// Initialize scenes
	logo = app->tex->Load("Assets/Scenes/logo.png");
	title = app->tex->Load("Assets/Scenes/title.png");
	gameover = app->tex->Load("Assets/Scenes/gameover.png");

	dir = true;
	jumpTimer = 30;

	// Initialize animations
	idleR.totalFrames = 0;
	idleL.totalFrames = 0;
	left.totalFrames = 0;
	right.totalFrames = 0;
	jumpR.totalFrames = 0;
	jumpL.totalFrames = 0;
	jumpR2.totalFrames = 0;
	jumpL2.totalFrames = 0;
	death.totalFrames = 0;
	win.totalFrames = 0;

	// Initilize textures
	texture = app->tex->Load(texturePath);

	// idleR Anim
	for (int i = 0; i < 4; i++) {
		idleR.PushBack({ 0 + (i * 48), 336, 48, 48 });
	}
	idleR.loop = true;
	idleR.speed = 0.1f;

	// idleL Anim
	for (int i = 3; i >= 0; i--) {
		idleL.PushBack({ 576 + (i * 48), 336, 48, 48 });
	}
	idleL.loop = true;
	idleL.speed = 0.1f;

	// right Anim
	for (int i = 0; i < 6; i++) {
		right.PushBack({ 0 + (i * 48), 480, 48, 48 });
	}
	right.loop = true;
	right.speed = 0.2f;

	// left Anim
	for (int i = 5; i >= 0; i--) {
		left.PushBack({ 480 + (i * 48), 480, 48, 48 });
	}
	left.loop = true;
	left.speed = 0.2f;

	// jumpR Anim
	for (int i = 0; i < 5; i++) {
		jumpR.PushBack({ 0 + (i * 48), 240, 48, 48 });
	}
	jumpR.loop = false;
	jumpR.speed = 0.2f;

	// jumpL Anim
	for (int i = 5; i >= 0; i--) {
		jumpL.PushBack({ 480 + (i * 48), 240, 48, 48 });
	}
	jumpL.loop = false;
	jumpL.speed = 0.2f;

	// jumpR2 Anim
	for (int i = 0; i < 3; i++) {
		jumpR2.PushBack({ 0 + (i * 48), 384, 48, 48 });
	}
	jumpR2.loop = false;
	jumpR2.speed = 0.15f;
	
	// jumpL2 Anim
	for (int i = 3; i >= 0; i--) {
		jumpL2.PushBack({ 576 + (i * 48), 384, 48, 48 });
	}
	jumpL2.loop = false;
	jumpL2.speed = 0.15f; 
	
	// death Anim
	for (int i = 0; i < 5; i++) {
		death.PushBack({ 0 + (i * 48), 192, 48, 48 });
	}
	death.loop = false;
	death.speed = 0.2f;

	// win Anim
	for (int i = 0; i < 5; i++) {
		win.PushBack({ 0 + (i * 48), 576, 48, 48 });
	}
	win.loop = true;
	win.speed = 0.15f;

	currentAnimation = &idleR;

	// Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x+16, position.y+16, 16, bodyType::DYNAMIC);

	// Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	// Initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	jumpFx = app->audio->LoadFx("Assets/Audio/Fx/jump.wav");
	deathFx = app->audio->LoadFx("Assets/Audio/Fx/biker_hurt.wav");
	winFx = app->audio->LoadFx("Assets/Audio/Fx/win.wav");

	return true;
}

bool Player::Update()
{
	posInicialY = position.y;

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	currentAnimation->Update();

	// Render the texture
	app->render->DrawTexture(logo, 0, -2000, NULL);
	app->render->DrawTexture(title, 0, -1000, NULL);
	app->render->DrawTexture(gameover, 0, -3000, NULL);
	app->render->DrawTexture(texture, position.x, position.y - 16, &rect);

	// Add physics to the player - updated player position using physics
	int speed = 10; 
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y); 

	// Modify the position of the player using arrow keys
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		
	}
		
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !dead && !winner && !app->scene->Logo && !app->scene->Title) {
		vel = b2Vec2(-speed * 0.8, -GRAVITY_Y);
		currentAnimation = &left;
		dir = false;
	}
	else {
		left.Reset();
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !dead && !winner && !app->scene->Logo && !app->scene->Title) {
		vel = b2Vec2(speed * 0.8, -GRAVITY_Y);
		currentAnimation = &right;
		dir = true;
	}
	else {
		right.Reset();
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !dead && !winner && !app->scene->Logo && !app->scene->Title) {
		if (onair == false)
			app->audio->PlayFx(jumpFx);
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && !dead && !winner && !app->scene->Logo && !app->scene->Title) {
		if (jumpTimer > 0)
		{
			if (dir) {
				currentAnimation = &jumpR;
			}
			else {
				currentAnimation = &jumpL;
			}
			vel = b2Vec2(0, GRAVITY_Y/2);
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !dead && !winner) {
				vel = b2Vec2(speed * 0.8, GRAVITY_Y/1.5);
				dir = true;
				currentAnimation = &jumpR;
			}
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !dead && !winner) {
				vel = b2Vec2(-speed * 0.8, GRAVITY_Y/1.5);
				dir = false;
				currentAnimation = &jumpL;
			}
			jumpTimer--;

		}
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE)
	{
		if (onair == false) {
			jumpL.Reset();
			jumpR.Reset();
			if (!GodMode)
			{
				jumpTimer = 30;
			}
			else
			{
				jumpTimer = 999;
			}
		}
		else
			jumpTimer = 0;
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
		jumpTimer = 0;

	// Gameplay conditions
	if (app->scene->Title == true) {
		lives = 3;
	}

	if (dead && !GodMode) {
		currentAnimation = &death;
		dir = true;
	}
	else {
		death.Reset();
	}

	if (winner && !GodMode) {
		currentAnimation = &win;
		dir = true;
	}
	else {
		win.Reset();
	}

	// idleAnim condition
	if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_IDLE && dir && !dead && !winner)
		currentAnimation = &idleR;

	if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_IDLE && !dir && !dead && !winner)
		currentAnimation = &idleL;

	// Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);

	// Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	//SDL_Rect textSection = { 0,0,48,48 };

	if (position.y - posInicialY == 0)
		onair = false;
	else
		onair = true;

	return true;
}

bool Player::CleanUp()
{
	return true;
}


// Define OnCollision function for the player. Check the virtual function on Entity class
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	// Detect the type of collision
	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("PLAYER Collision ITEM");
		break;
	case ColliderType::PLATFORM:
		LOG("PLAYER Collision PLATFORM");
		break;
	case ColliderType::ENEMY:
		LOG("PLAYER Collision ENEMY");
		break;
	case ColliderType::ENEMYFLYING:
		LOG("PLAYER Collision FLYING ENEMY");
		break;
	case ColliderType::DEATH:
		LOG("PLAYER Collision DEATH");
		if (!GodMode) {
			dead = true;
			lives = lives - 1;
			app->audio->PlayFx(deathFx);
		}
		break;
	case ColliderType::WIN:
		LOG("PLAYER Collision WIN");
		if (!GodMode) {
			winner = true;
			app->audio->PlayFx(winFx);
		}
		break;
	case ColliderType::UNKNOWN:
		LOG("PLAYER Collision UNKNOWN");
		break;
	}
}
