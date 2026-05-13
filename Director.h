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
		MINELAYER,
		SLOWER,
		BOSS
	};
	bool Initialise(Renderer& renderer, std::vector<EnemyBase*> &EnemyArray, PlayerObject* m_pPlayerChar, b2WorldId WorldP);
	void SpawnTrees();
	void Process(float deltaTime);
	void Draw(Renderer& renderer);
	void AddCredits(int exp);
	void CreateEnemy(Enemies input);
	void CreateFriends();
	void DebugDraw();
protected:

private:
	Director(const Director& Director);
	Director& operator=(const Director& Director);

	// Member data:
public:
	float EnemyPrice[4];
	float EnemyPriceMultiplier[4];
	std::vector<EnemyBase*>* m_pDirectorArray;
	PlayerObject* m_pPlayer;
	Renderer* storage;
	b2WorldId Worldref;
	int TotalEntities;
	float Credits = 0;
	float Timetospawn = 1;
	float Timepassed = 0;
	int Arraysize;
	int totaltrees;
	bool Spawnboss;
	float bosstimer;
	bool spawnedtrees;
protected:

private:

};

#endif // __DIRECTOR_H__
