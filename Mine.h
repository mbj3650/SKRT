// COMP710 JESSE
#ifndef __MINE_H__
#define __MINE_H__
#include "EnemyBase.h"
// Forward declarations:
class Renderer;

// Class declaration:
class Mine : public EnemyBase
{
	// Member methods:
public:
	Mine();
	~Mine();

	bool Initialise(Renderer& renderer, b2BodyId playerAddress, b2WorldId WorldID, b2Vec2 position);
	void Process(float deltaTime);
	void Draw(Renderer& renderer);
	void ProcessDamageCollision(b2BodyId collidingwith) override;
protected:

private:
	Mine(const Mine& Mine);
	Mine& operator=(const Mine& Mine);

	// Member data:
public:

protected:
	float lifetime = 5;
	float flickertimer;
private:

};

#endif // __MINE_H__
