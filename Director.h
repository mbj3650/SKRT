// COMP710 JESSE
#ifndef __DIRECTOR_H__
#define __DIRECTOR_H__

// Forward declarations:
class Renderer;
class EnemyBase;
class PlayerObject;
class b2WorldId;
class b2BodyId;
#include "box2d.h"
#include <vector>
// Class declaration:
class Director
{
	// Member methods:
public:
	Director();
	~Director();
	enum Enemies
	{
		DEMON,
		RANGER,
		SLOWER,
	};
	bool Initialise(Renderer& renderer, std::vector<EnemyBase*>& EnemyArray, PlayerObject* m_pPlayerChar, b2WorldId WorldP);
	void Process(float deltaTime);
	void Draw(Renderer& renderer);
	void CreateEnemy(Enemies input);
protected:

private:
	Director(const Director& Director);
	Director& operator=(const Director& Director);

	// Member data:
public:
	int EnemyPrice[3];
	std::vector<EnemyBase*>* m_pDirectorArray;
	PlayerObject* m_pPlayer;
	Renderer* storage;
	b2WorldId Worldref;
	int TotalEntities;
	int Credits;
	float Timetospawn = 1;
	float Timepassed = 0;
	
protected:

private:

};

#endif // __DIRECTOR_H__
