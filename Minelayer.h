// COMP710 JESSE
#ifndef __MINELAYER_H__
#define __MINELAYER_H__
#include "EnemyBase.h"
// Forward declarations:
class Renderer;
class b2BodyId;
class AnimatedSprite;
// Class declaration:
class Minelayer : public EnemyBase
{
	// Member methods:
public:
	Minelayer();
	~Minelayer();

	bool Initialise(Renderer& renderer, b2BodyId playerAddress, b2WorldId WorldID, b2Vec2 position) override;
	void PickNewSpot();
	void MinePlaced();
	void Process(float deltaTime);
	void Draw(Renderer& renderer);
	void ProcessDamageCollision(b2BodyId collidingwith) override;
	void ProcessCollision(b2BodyId collidingwith) override;
protected:

private:
	Minelayer(const Minelayer& Minelayer);
	Minelayer& operator=(const Minelayer& Minelayer);

	// Member data:
public:
	bool needsmine;
protected:
	
	float minetimer;//wait till leaving the spot aka "minelaying" phase
	Vector2 target;
	bool travelling;//go to a specific point and when in range drop a mine
	Sprite* m_pSprite;
private:

};

#endif // __MINELAYER_H__
