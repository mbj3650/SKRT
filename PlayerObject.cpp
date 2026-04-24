// COMP710 JESSE
// This include:
#include "PlayerObject.h"
#include "UpgradeList.h"
// Local includes:
#include "renderer.h"
#include "sprite.h"
#include "lib/imgui/imgui.h"
#include "inputsystem.h"
// Library includes:
#include <cassert>
#include <string>
#include <box2d.h>
#include <iostream>
float PlayerObject::sm_fBoundaryWidth = 0.0f;
float PlayerObject::sm_fBoundaryHeight = 0.0f;
PlayerObject* PlayerObject::sm_pInstance = 0;
PlayerObject::PlayerObject() :
	SpeedminBase(250),
	maxdistance (200),
	DamageBase(20),
	SpeedBase(1.2),
	reboundlossbase(0.75),
	experience(0),
	level(1),
	health(100)
{

};
PlayerObject::~PlayerObject()
{
b2DestroyBody(ID);
delete sm_pInstance;
sm_pInstance = 0;
};


PlayerObject& PlayerObject::GetInstance()
{
	static PlayerObject onlyInstance;
	return onlyInstance;
}

bool
PlayerObject::Initialise(Renderer& renderer, b2WorldId WorldId)
{
	Speedmin = SpeedminBase;
	Damage = DamageBase;
	speed = SpeedBase;
	reboundloss = reboundlossbase;
	exptolevel = (level / 2 * 100) + (2/level);
	//CREATE SPRITE TO FOLLOW SHAPE
	m_pSprite = renderer.CreateSprite("..\\assets\\cursor.png");
	m_bAlive = true;
	
	ratio = maxdistance / 100;
	m_pSprite->SetScale(0.075);

	//m_pSprite->SetAngle(180.0f);

	//ComputeBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
	radius = m_pSprite->GetWidth();
	m_pBoostPointer = renderer.CreateSprite("..\\assets\\booster.png");
	m_pBoostPointer->SetGreenTint(0.0f);
	m_pBoostPointer->SetScale(0.0);
	m_pBoostPointer->SetBlueTint(0.0f);


	const float MAX_SPEED = 250.0f;
	const int EDGE_LIMIT = m_pSprite->GetWidth();
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();
	sm_fBoundaryWidth = static_cast<float>(SCREEN_WIDTH);
	sm_fBoundaryHeight = static_cast<float>(SCREEN_HEIGHT);


	//CREATE BODY FOR THE WORLD TO USE AS SHAPE REFERENCE
	b2BodyDef WorldObj = b2DefaultBodyDef();
	WorldObj.position.x = (SCREEN_WIDTH / 2); ;
	WorldObj.position.y = SCREEN_HEIGHT / 2;;
	ID = b2CreateBody(WorldId, &WorldObj);
	b2Body_SetType(ID, b2_dynamicBody);
	b2Polygon Playerbox = b2MakeRoundedBox(10.0f, 10.0f,1.0f);
	b2Body_SetLinearDamping(ID, 0);
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 1000.0f;
	shapeDef.friction = 1.0f;


	shapeDef.filter.categoryBits = 0x0001;//i am
	shapeDef.filter.maskBits = 0x0008 | 0x0002;//i collide with


	b2Body_SetUserData(ID, this);
	shapeId = b2CreatePolygonShape(ID, &shapeDef, &Playerbox);


	b2Shape_EnableContactEvents(shapeId, true);
	m_position.x = b2Body_GetPosition(ID).x;
	m_position.y = b2Body_GetPosition(ID).y;
	angle = 0;
	b2Body_SetAwake(ID, true);

	return true;
};

void
PlayerObject::Process(float deltaTime, InputSystem& inputSystem)
{
	if (deltaTime != 0) {
		int result = inputSystem.GetMouseButtonState(SDL_BUTTON_LEFT);
		mouse_position.x = inputSystem.GetMousePosition().x;
		mouse_position.y = inputSystem.GetMousePosition().y;
		if (inputSystem.GetKeyState(SDL_SCANCODE_SPACE) != BS_HELD) {
			Drifting = false;
			driftpos.x = 0;
			driftpos.y = 0;
		}

		if (IFrames > 0) {
			IFrames -= deltaTime * 2;
			int Framelogic = IFrames * 10;
			if (Framelogic % 2 == 0) {
				m_pSprite->SetAlpha(1.0f);
			}
			else {
				m_pSprite->SetAlpha(0.0f);
			}
		}


		if (healthdelay > 0) {
			healthdelay -= deltaTime * 1;
		}


		if (inputSystem.GetKeyState(SDL_SCANCODE_SPACE) == BS_HELD || IsAiming) {//if holding space, brake quickly
			if (Drifting == false)//if player hasnt been drifting
			{//get position so we can set up drifting angle particles
				Drifting = true;
				driftpos.x = b2Body_GetPosition(ID).x;
				driftpos.y = b2Body_GetPosition(ID).y;
			}
			Drifting = false;
			if (b2Body_GetLinearVelocity(ID).x > 15 || b2Body_GetLinearVelocity(ID).x < -15) {
				Player_speed.x *= (0.996);
				Drifting = true;
			}
			else {
				Player_speed.x = 0;
			}


			if (b2Body_GetLinearVelocity(ID).y > 15 || b2Body_GetLinearVelocity(ID).y < -15) {
				Player_speed.y *= (0.996);
				Drifting = true;
			}
			else {
				Player_speed.y = 0;
			}
		}
		if (result == BS_PRESSED || IsAiming) {
			if (IsAiming == false) {//if first frame in which mouse is pressed
				//store mouse position to fling from
				clickpos.x = mouse_position.x; 
				clickpos.y = mouse_position.y;

			}
			distancebetween = sqrt((pow((mouse_position.x - clickpos.x), 2) + pow((mouse_position.y - clickpos.y), 2))) * 0.2;//get distance to mouse and original click position
			if (distancebetween > maxdistance) {
				distancebetween = maxdistance;
			}

			storedvelocity = distancebetween * speed;//distance * speed


			IsAiming = true;//aiming true
			angle = atan2(mouse_position.y - clickpos.y, mouse_position.x - clickpos.x);//set angle 

			angle = (180.0 / M_PI) * -angle;
			angle += 180;
			m_pBoostPointer->SetScale(0.0015 * (distancebetween / ratio));
			m_pBoostPointer->SetRedTint(50 / (distancebetween / ratio));
			m_pBoostPointer->SetGreenTint((distancebetween / ratio) / 50);

			/*	if (storedvelocity < 300) {
					storedvelocity += 100 * deltaTime;
				}*/
		}
		if (result == BS_RELEASED) {//if released dont draw booster
			Drifting = false;
			m_pBoostPointer->SetScale(0.0);
			Player_speed.y += storedvelocity * (sin((M_PI / 180) * -angle));
			Player_speed.x += storedvelocity * (cos((M_PI / 180) * -angle));
			IsAiming = false;
			storedvelocity = 0;
			distancebetween = 0;
		}

		//bounce off edges
		if (b2Body_GetPosition(ID).x + (radius / 2) > sm_fBoundaryWidth && Player_speed.x > 0)
		{
			Player_speed.x *= -1.0f;
			losemomentum();
		}
		else if (b2Body_GetPosition(ID).x - (radius / 2) < 0 && Player_speed.x < 0)
		{
			Player_speed.x *= -1.0f;
			losemomentum();

		}
		else if (b2Body_GetPosition(ID).y + (radius / 2) > sm_fBoundaryHeight && Player_speed.y > 0)
		{
			Player_speed.y *= -1.0f;
			losemomentum();
		}
		else if (b2Body_GetPosition(ID).y - (radius / 2) < 0 && Player_speed.y < 0)
		{
			Player_speed.y *= -1.0f;
			losemomentum();
		}



		b2Vec2 velocity = { Player_speed.x, Player_speed.y };
		b2Body_SetLinearVelocity(ID, velocity);//set velocity for obejct to move with

	}
	
	//set sprite positions
	m_pSprite->SetX(static_cast<int>(b2Body_GetPosition(ID).x));
	m_pSprite->SetY(static_cast<int>(b2Body_GetPosition(ID).y));
	m_pSprite->SetAngle(angle - 115);

	m_pBoostPointer->SetX((static_cast<int>(b2Body_GetPosition(ID).x)) + ((distancebetween / ratio) / 3 * (cos((M_PI / 180) * -angle))));
	m_pBoostPointer->SetY((static_cast<int>(b2Body_GetPosition(ID).y)) + ((distancebetween / ratio) / 3 * (sin((M_PI / 180) * -angle))));

	m_pBoostPointer->SetAngle(angle - 90);

	m_pSprite->SetX(b2Body_GetPosition(ID).x);
	m_pSprite->SetY(b2Body_GetPosition(ID).y);
};
void
PlayerObject::Draw(Renderer& renderer)
{
	if (IsAiming) {//dont draw this if player isnt aiming
		m_pBoostPointer->Draw(renderer);
	}
	m_pSprite->Draw(renderer);
};

bool PlayerObject::isDrifting() {
	if (sqrt(pow((b2Body_GetLinearVelocity(ID).x), 2) + pow((b2Body_GetLinearVelocity(ID).y), 2)) < 5) {
		return false;
	}
	return Drifting;
}

void PlayerObject::losemomentum() {
	if (reboundloss * (speed / SpeedBase) < 1) {//make sure the player doesnt get FASTER if their speed is fast enough
		Player_speed.x *= reboundloss * (speed / SpeedBase); //if player speed is 1.5 while the base is 1.2, that means that rebound loss upon hitting an enemy wont be as bad
		Player_speed.y *= reboundloss * (speed / SpeedBase);
	}
	b2Body_SetLinearVelocity(ID, { Player_speed.x, Player_speed.y });//set velocity for obejct to move with
}

Vector2 PlayerObject::getDriftAngle() {
	//float driftangle = atan2(b2Body_GetPosition(ID).y - driftpos.y, b2Body_GetPosition(ID).x - driftpos.x);//set angle 
	//driftangle = (180 * driftangle)/M_PI;
	return { 0, 0 };
}

//GET AND SET FUNCTIONS HERE
float PlayerObject::getSpeed() {
	return sqrt((pow((Player_speed.x), 2) + pow((Player_speed.y), 2)));
}

float PlayerObject::GetRadius() {
	return m_pSprite->GetWidth() / 2;
}

void PlayerObject::SetPosition(b2Vec2 position) {
	//m_pSprite->SetGreenTint(1.0f);
}

b2Vec2 PlayerObject::Position() {
	return b2Body_GetPosition(ID);
}

void PlayerObject::AddExp(float experienceamount) {
	experience += experienceamount;
	if (experience > exptolevel) {
		level += 1;
		exptolevel = (level / 5 * 100) * (level / 50) + 50;
		experience -= exptolevel;
		PlayerNeedsUpgrade = true;
	}
}

void PlayerObject::CheckLevel() {
	AddExp(0);//we can just check if the player can level up again by adding 0 exp lol
}

void PlayerObject::AddHealth(float healthtoadd, bool isdead) {
	health += healthtoadd;
	healthdelay = 0.2;//timer before health can be added
	if (isdead == true && IFrames <= 0) {//if enemy is dead from the hit and player can take damage
		losemomentum();//lose momentum
	}
}

bool PlayerObject::CanHeal() {
	if (healthdelay <= 0) {
		return true;
	}
	else {
		return false;
	}
}

void PlayerObject::takedamage(float damagetotake)
{
	health -= damagetotake;
	IFrames = 2;//player cant take damage again
}

float PlayerObject::GetShipAngle() {
	return angle;
}

//check if player is fast enough to damage
bool PlayerObject::CanDamage() {
	float playerspeed = sqrt(pow((b2Body_GetLinearVelocity(ID).x), 2) + pow((b2Body_GetLinearVelocity(ID).y), 2));
		return playerspeed > Speedmin;//compare speed to speed minimum to damage
}

bool PlayerObject::CanTakeDamage() {
	if (IFrames <= 0) {
		return true;
	}
	else {
		return false;
	}
}

float PlayerObject::getDamage() {
	return Damage;
}

std::vector<UpgradeList::Template> PlayerObject::GetUgprades() {
	return CurrentUpgrades;
}

bool PlayerObject::AddUpgrade(UpgradeList::Template upgrade) {
	for (int i = 0; i < CurrentUpgrades.size(); i++) {
		if (CurrentUpgrades.at(i).upgradesinto == upgrade.ID) {//if one of the player's upgrades turn into the new one,
			CurrentUpgrades.erase(CurrentUpgrades.begin()+ i);//replaces upgrades that turn into the new one, this can also work for 
			//upgrades that require multiple predecessors
		}
	}
	PlayerNeedsUpgrade = false;
	CurrentUpgrades.push_back(upgrade);//add upgrade
	UpdateStats();
	CheckLevel(); //check if we need to level up again
	return true;
}

bool PlayerObject::AddUpgrade(int upgrade)//skip
{
	PlayerNeedsUpgrade = false;
	UpdateStats();
	CheckLevel(); //check if we need to level up again
	return true;
}

void PlayerObject::UpdateStats() {
	float statchanges[3] = { SpeedBase, DamageBase, reboundlossbase };
	for (int i = 0; i < CurrentUpgrades.size(); i++) {
		if (CurrentUpgrades.at(i).stateffect != "-1") {
			std::string statcheck;
			float statmodifier;
			statcheck.push_back(CurrentUpgrades.at(i).stateffect.at(0));//1 is speed, 2 is damage, 3 is rebound speed

			//substr makes 1,012 into [1,] and [012] with 012 being the percentage increase
			statmodifier = 1 + (0.01 * stoi(CurrentUpgrades.at(i).stateffect.substr(3, 3)));

			switch (std::stoi(statcheck)) {//multiplies the values by each upgrade
				case 1:
					statchanges[0] *= statmodifier;
					break;
				case 2:
					statchanges[1] *= statmodifier;
					break;
				case 3:
					statchanges[2] *= statmodifier;
					break;
			}
			
		}
	}
	speed = statchanges[0];
	Damage = statchanges[1];
	reboundloss = statchanges[2];
}

void PlayerObject::DebugDraw(){
	ImGui::Text("SHIP INFORMATION:");
	ImGui::Text("Health: %d", health);
	ImGui::Text("Angle %f", atan2(b2Body_GetPosition(ID).y - driftpos.y, b2Body_GetPosition(ID).x - driftpos.x));
	ImGui::Text("momentum:%f", sqrt(pow((b2Body_GetLinearVelocity(ID).x), 2) + pow((b2Body_GetLinearVelocity(ID).y), 2)));
	ImGui::Text("dist:%f", distancebetween);
	ImGui::Text("Velocity x:%f y:%f", b2Body_GetLinearVelocity(ID).x, b2Body_GetLinearVelocity(ID).y);
	ImGui::Text("Mous Position x:%f y:%f", mouse_position.x, mouse_position.y);
	ImGui::Text("Position x:%f y:%f", Position().x, Position().y);
	ImGui::Text("Exp: %f/%d", experience, exptolevel);
}