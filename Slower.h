// COMP710 JESSE
#ifndef __SLOWER_H__
#define __SLOWER_H__
#include "EnemyBase.h"
// Forward declarations:
class Renderer;
class AnimatedSprite;
class ParticleEmitter;
// Class declaration:
class Slower : public EnemyBase
{
	// Member methods:
public:
	Slower();
	~Slower() override;

	bool Initialise(Renderer& renderer, b2BodyId playerAddress, b2WorldId WorldID, b2Vec2 position) override;
	void PickNewSpot();
	void Process(float deltaTime);
	void Draw(Renderer& renderer);
	void ProcessDamageCollision(b2BodyId collidingwith) override;
	void ProcessCollision(b2BodyId collidingwith) override;
protected:

private:
	Slower(const Slower& Slower);
	Slower& operator=(const Slower& Slower);

	// Member data:
public:
	bool needsmine;
	bool playingsound;
protected:
	float cooldown;//will wait before ddetonating again
	float detonation;//will slowly explode into an area of slow
	Vector2 target;
	bool travelling;//go to a specific point and when in range drop a mine
	Sprite* m_pSprite;
	Sprite* m_pCircle;
	AnimatedSprite* m_pASprite;
	ParticleEmitter* Radiation;
private:


};

#endif // __SLOWER_H__
