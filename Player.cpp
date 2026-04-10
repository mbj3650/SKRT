// COMP710 JESSE
// This include:
#include "Player.h"

// Local includes:
#include "renderer.h"
#include "inputsystem.h"
#include "sprite.h"
// Library includes:
#include <cassert>
#include "lib/imgui/imgui.h"
#include <iostream>
#include <box2d.h>
float Player::sm_fBoundaryWidth = 0.0f;
float Player::sm_fBoundaryHeight = 0.0f;
Player::Player() :
	Speedmin(50),
	reboundloss(0.75)
{

};
Player::~Player()
{
	m_pSprite->~Sprite();
	delete m_pSprite;
	m_pSprite = 0;
	delete m_pBoostPointer;
	m_pBoostPointer = 0;
	delete this;
};

bool
Player::Initialise(Renderer& renderer)
{
	m_pSprite = renderer.CreateSprite("..\\assets\\triangle.png");
	const float MAX_SPEED = 250.0f;
	const int EDGE_LIMIT = m_pSprite->GetWidth();
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();
	m_bAlive = true;
	m_pSprite->SetRedTint(0.0f);
	m_pSprite->SetScale(0.075);
	m_pSprite->SetBlueTint(0.0f);
	//m_pSprite->SetAngle(180.0f);
	sm_fBoundaryWidth = static_cast<float>(SCREEN_WIDTH);
	sm_fBoundaryHeight = static_cast<float>(SCREEN_HEIGHT);
	m_position.x = (SCREEN_WIDTH / 2);
	m_position.y = SCREEN_HEIGHT /2;
	angle = 0;
	//ComputeBounds(SCREEN_WIDTH, SCREEN_HEIGHT);
	radius = m_pSprite->GetWidth();
	m_pBoostPointer = renderer.CreateSprite("..\\assets\\booster.png");
	m_pBoostPointer->SetGreenTint(0.0f);
	m_pBoostPointer->SetScale(0.0);
	m_pBoostPointer->SetBlueTint(0.0f);
	return true;

	return true;
};

void
Player::Process(float deltaTime, InputSystem& inputSystem)
{
	
	int result = inputSystem.GetMouseButtonState(SDL_BUTTON_LEFT);
	mouse_position.x = inputSystem.GetMousePosition().x;
	mouse_position.y = inputSystem.GetMousePosition().y;
	if (inputSystem.GetKeyState(SDL_SCANCODE_SPACE)) {//if holding space, brake quickly

		if (Player_speed.x > 0.5f || Player_speed.x < -0.5f) {
			Player_speed.x *= (0.9985);
		}
		else {
			Player_speed.x = 0;
		}


		if (Player_speed.y > 0.5f || Player_speed.y < -0.5f) {
			Player_speed.y *= (0.9985);
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
		if (distancebetween > 100) {
			distancebetween = 100;
		}
		storedvelocity = distancebetween * 6;
		

		IsAiming = true;
		angle = atan2(mouse_position.y - clickpos.y, mouse_position.x - clickpos.x);

		angle = (180.0 / M_PI) * -angle;
		angle += 180;
		m_pBoostPointer->SetScale(0.0015 * distancebetween);
		m_pBoostPointer->SetRedTint(50 / distancebetween);
		m_pBoostPointer->SetGreenTint(distancebetween / 50);
		
		/*	if (storedvelocity < 300) {
				storedvelocity += 100 * deltaTime;
			}*/
	}
	if(result == BS_RELEASED){
		m_pBoostPointer->SetScale(0.0);
		Player_speed.y += storedvelocity * (sin((M_PI / 180) * -angle));
		Player_speed.x += storedvelocity * (cos((M_PI / 180) * -angle));
		IsAiming = false;
		storedvelocity = 0;
		distancebetween = 0;
	}
	if (m_position.x + (radius / 2) > sm_fBoundaryWidth)
	{
		Player_speed.x *= -1.0f;
		Player_speed.x *= reboundloss;
		m_position.x = sm_fBoundaryWidth- (radius / 2);
	}
	else if (m_position.x- (radius / 2) < 0)
	{
		Player_speed.x *= -1.0f;
		Player_speed.x *= reboundloss;
		m_position.x =  (radius / 2);
	}
	if (m_position.y+ (radius / 2) > sm_fBoundaryHeight)
	{
		Player_speed.y *= -1.0f;
		Player_speed.y *= reboundloss;
		m_position.y = sm_fBoundaryHeight - (radius / 2);
	}
	else if (m_position.y- (radius / 2) < 0)
	{
		Player_speed.y *= -1.0f;
		Player_speed.y *= reboundloss;
		m_position.y = (radius / 2);
	}
	m_position.x += Player_speed.x * deltaTime;
	m_position.y += Player_speed.y * deltaTime;
	m_pSprite->SetX(static_cast<int>(m_position.x));
	m_pSprite->SetY(static_cast<int>(m_position.y));
	m_pSprite->SetAngle(angle+90);


	m_pBoostPointer->SetX((static_cast<int>(m_position.x)) + (distancebetween/3 * (cos((M_PI / 180) * -angle)))		);
	m_pBoostPointer->SetY((static_cast<int>(m_position.y)) + (distancebetween / 3 * (sin((M_PI / 180) * -angle)))       );
	
	m_pBoostPointer->SetAngle(angle -90);
};
void
Player::Draw(Renderer& renderer)
{
	m_pBoostPointer->Draw(renderer);
		m_pSprite->Draw(renderer);
	
};

float Player::getSpeed() {
	return sqrt((pow((Player_speed.x), 2) + pow((Player_speed.y), 2)));
}

float Player::GetRadius() {
	return m_pSprite->GetWidth() / 2;
}

void Player::aiming() {
	if (IsAiming == false) {
		IsAiming = true;
	}
	
}



void Player::drift() {
	if (IsDrifting == false) {
		IsDrifting = true;
	}

}

void Player::Fling() {
	if (Fire == false) {
		Fire = true;
	}
	
}

void Player::SetPosition(Vector2 position) {
	m_position = position;
}
void Player::SetPosition(b2Vec2 position) {
	m_position.x = position.x;
	m_position.y = position.y;
}

Vector2 Player::Position() {
	return m_position;
}

float Player::GetShipAngle() {
	return m_pSprite->GetAngle();
}

bool Player::CanDamage() {
	if (sqrt((pow((Player_speed.x), 2) + pow((Player_speed.y), 2))) > Speedmin) {
		return true;
	}
}


void Player::DebugDraw() {
	ImGui::Text("SHIP INFORMATION:");
	ImGui::Text("Angle %f", GetShipAngle());
	ImGui::Text("momentum:%f", sqrt((pow((Player_speed.x), 2) + pow((Player_speed.y), 2))));
	ImGui::Text("dist:%f", distancebetween);
	ImGui::Text("Velocity x:%f y:%f", Player_speed.x, Player_speed.y);
	ImGui::Text("Mous Position x:%f y:%f", mouse_position.x, mouse_position.y);
	ImGui::Text("Position x:%f y:%f", Position().x, Position().y);
}