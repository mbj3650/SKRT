// COMP710 JESSE
// This include:
#include "PlayerObject.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
#include "lib/imgui/imgui.h"
#include "inputsystem.h"
// Library includes:
#include <cassert>

#include <box2d.h>
float PlayerObject::sm_fBoundaryWidth = 0.0f;
float PlayerObject::sm_fBoundaryHeight = 0.0f;
PlayerObject::PlayerObject():
Speedmin(100),
Damage(20),
speedboost(2),
reboundloss(0.75)
{

};
PlayerObject::~PlayerObject()
{

};

bool
PlayerObject::Initialise(Renderer& renderer, b2WorldId WorldId)
{
	//CREATE SPRITE TO FOLLOW SHAPE
	m_pSprite = renderer.CreateSprite("..\\assets\\triangle.png");
	m_bAlive = true;
	maxdistance = 200;
	ratio = maxdistance / 100;
	m_pSprite->SetRedTint(0.0f);
	m_pSprite->SetScale(0.075);
	m_pSprite->SetBlueTint(0.0f);
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
	b2Polygon box = b2MakeBox(12.0f, 12.0f);

	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 100.0f;
	shapeDef.friction = 1.0f;
	b2Body_SetUserData(ID, this);
	b2ShapeId shapeId = b2CreatePolygonShape(ID, &shapeDef, &box);
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
	int result = inputSystem.GetMouseButtonState(SDL_BUTTON_LEFT);
	mouse_position.x = inputSystem.GetMousePosition().x;
	mouse_position.y = inputSystem.GetMousePosition().y;
	if (inputSystem.GetKeyState(SDL_SCANCODE_SPACE)) {//if holding space, brake quickly

		if (Player_speed.x > 0.5f || Player_speed.x < -0.5f) {
			Player_speed.x *= (0.997);
		}
		else {
			Player_speed.x = 0;
		}


		if (Player_speed.y > 0.5f || Player_speed.y < -0.5f) {
			Player_speed.y *= (0.997);
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
		distancebetween = sqrt((pow((mouse_position.x - clickpos.x), 2) + pow((mouse_position.y - clickpos.y), 2))) * 0.2;
		if (distancebetween > maxdistance) {
			distancebetween = maxdistance;
		}

		storedvelocity = distancebetween*speedboost;


		IsAiming = true;
		angle = atan2(mouse_position.y - clickpos.y, mouse_position.x - clickpos.x);

		angle = (180.0 / M_PI) * -angle;
		angle += 180;
		m_pBoostPointer->SetScale(0.0015 * (distancebetween/ ratio));
		m_pBoostPointer->SetRedTint(50 / (distancebetween/ ratio));
		m_pBoostPointer->SetGreenTint((distancebetween/ ratio) / 50);

		/*	if (storedvelocity < 300) {
				storedvelocity += 100 * deltaTime;
			}*/
	}
	if (result == BS_RELEASED) {
		m_pBoostPointer->SetScale(0.0);
		Player_speed.y += storedvelocity * (sin((M_PI / 180) * -angle));
		Player_speed.x += storedvelocity * (cos((M_PI / 180) * -angle));
		IsAiming = false;
		storedvelocity = 0;
		distancebetween = 0;
	}
	if (b2Body_GetPosition(ID).x + (radius/2) > sm_fBoundaryWidth && Player_speed.x > 0)
	{
		Player_speed.x *= -1.0f;
	}
	else if (b2Body_GetPosition(ID).x - (radius / 2) < 0 && Player_speed.x < 0)
	{
		Player_speed.x *= -1.0f;

	}
	else if (b2Body_GetPosition(ID).y + (radius / 2) > sm_fBoundaryHeight && Player_speed.y > 0)
	{
		Player_speed.y *= -1.0f;

	}
	else if (b2Body_GetPosition(ID).y - (radius / 2) < 0 && Player_speed.y < 0)
	{
		Player_speed.y *= -1.0f;
	}
	b2Vec2 velocity = { Player_speed.x, Player_speed.y };
	b2Body_SetLinearVelocity(ID, velocity);
	m_pSprite->SetX(static_cast<int>(b2Body_GetPosition(ID).x));
	m_pSprite->SetY(static_cast<int>(b2Body_GetPosition(ID).y));
	m_pSprite->SetAngle(angle + 90);

	m_pBoostPointer->SetX((static_cast<int>(b2Body_GetPosition(ID).x)) + ((distancebetween / ratio) / 3 * (cos((M_PI / 180) * -angle))));
	m_pBoostPointer->SetY((static_cast<int>(b2Body_GetPosition(ID).y)) + ((distancebetween / ratio) / 3 * (sin((M_PI / 180) * -angle))));

	m_pBoostPointer->SetAngle(angle - 90);

	m_pSprite->SetX(b2Body_GetPosition(ID).x);
	m_pSprite->SetY(b2Body_GetPosition(ID).y);
};
void
PlayerObject::Draw(Renderer& renderer)
{
	m_pBoostPointer->Draw(renderer);
	m_pSprite->Draw(renderer);
};


float PlayerObject::getSpeed() {
	return sqrt((pow((Player_speed.x), 2) + pow((Player_speed.y), 2)));
}

float PlayerObject::GetRadius() {
	return m_pSprite->GetWidth() / 2;
}

void PlayerObject::SetPosition(b2Vec2 position) {
	m_pSprite->SetGreenTint(1.0f);
}

b2Vec2 PlayerObject::Position() {
	return b2Body_GetPosition(ID);
}

float PlayerObject::GetShipAngle() {
	return angle;
}

bool PlayerObject::CanDamage() {
	if (sqrt((pow((Player_speed.x), 2) + pow((Player_speed.y), 2))) > Speedmin) {
		return true;
	}
}

float PlayerObject::getDamage() {
	return Damage;
}

void PlayerObject::DebugDraw(){
	ImGui::Text("SHIP INFORMATION:");
	ImGui::Text("Angle %f", angle);
	ImGui::Text("momentum:%f", sqrt((pow((Player_speed.x), 2) + pow((Player_speed.y), 2))));
	ImGui::Text("dist:%f", distancebetween);
	ImGui::Text("Velocity x:%f y:%f", b2Body_GetLinearVelocity(ID).x, b2Body_GetLinearVelocity(ID).y);
	ImGui::Text("Mous Position x:%f y:%f", mouse_position.x, mouse_position.y);
	ImGui::Text("Position x:%f y:%f", Position().x, Position().y);
}