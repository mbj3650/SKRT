// COMP710 JESSE
#ifndef __BIGBOSS_H__
#define __BIGBOSS_H__
#include "EnemyBase.h"
// Forward declarations:
class Renderer;
class Sprite;
class Vector2;
class b2Vec2;
// Class declaration:
class Bigboss : public EnemyBase
{
	// Member methods:
public:
	Bigboss();
	~Bigboss() override;


	bool Initialise(Renderer& renderer, b2BodyId playerAddress, b2WorldId WorldID, b2Vec2 position) override;
	void ProcessDamageCollision(b2BodyId collidingwith) override;
	void Process(float deltaTime);
	void Draw(Renderer& renderer);

protected:

private:
	Bigboss(const Bigboss& Bigboss);
	Bigboss& operator=(const Bigboss& Bigboss);

	// Member data:
public:
	bool summonfriends;
protected:
	float IFrames;
	float cooldown;
	float chargeup;
	float chargingtimer;
	bool inbounds;
	int attack;
	Sprite* m_pBooster;
	Sprite* m_pSaws;
	Vector2 target;
	b2Vec2 velocityVec;
private:

};

#endif // __BIGBOSS_H__
