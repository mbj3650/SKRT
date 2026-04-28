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

	bool Initialise(Renderer& renderer);
	void Process(float deltaTime);
	void Draw(Renderer& renderer);

protected:

private:
	Mine(const Mine& Mine);
	Mine& operator=(const Mine& Mine);

	// Member data:
public:

protected:

private:

};

#endif // __MINE_H__
