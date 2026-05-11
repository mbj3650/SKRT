// COMP710 JESSE
#ifndef __HUD_H__
#define __HUD_H__

// Forward declarations:
class Renderer;
class Sprite;
class PlayerObject;
#include <string>;
#include <vector>;
// Class declaration:
class Hud
{
	// Member methods:
public:
	Hud();
	~Hud();

	bool Initialise(Renderer& renderer, PlayerObject* player);
	void updatestats();
	void DisplayScore();
	void Process(float deltaTime);
	void GameWipe();
	void SetPlayer(PlayerObject* player);
	void Draw(Renderer& renderer);

protected:

private:
	Hud(const Hud& Hud);
	Hud& operator=(const Hud& Hud);

	// Member data:
public:

protected:
	Sprite* m_pSpriteHealth;
	Sprite* HealthText;
	std::string gameover;
	std::string restart;
	Renderer* storage;
	Sprite* text;
		Sprite* text2;
		std::vector <Sprite*> spritelist;
	PlayerObject* m_pPlayer;
private:

};

#endif // __HUD_H__
