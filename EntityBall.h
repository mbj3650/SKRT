// COMP710 GP Framework 2022
#ifndef ENTITYBALL_H
#define ENTITYBALL_H
// Local includes:
#include "vector2.h"
#include "ball.h"
// Forward declarations:
class Renderer;
class Sprite;
// Class declaration:
class EntityBall : public Ball
{
public:
	EntityBall();
	~EntityBall();
	bool Initialise(Renderer& renderer);

	float GetRadius();

	float GetGB();

	void SetGood();
	void SetBad();
	void SetNeutral();
	void RandomiseEntitySize();
	void UpdateScale(float scale);

protected:
	int Goodbad;//1 is good 2 is bad
};

#endif // ENTITYBALL_H
