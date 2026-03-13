// COMP710 GP Framework 2022
#ifndef SCENESPLASHAUT_H
#define SCENESPLASHAUT_H
// Local includes:
#include "scene.h"
// Forward declarations:
class Renderer;
class Sprite;
// Class declaration:
class SceneSplashAUT : public Scene
{
	// Member methods:
public:
	SceneSplashAUT();
	virtual ~SceneSplashAUT();
	virtual bool Initialise(Renderer& renderer);
	virtual void Process(float deltaTime);
	virtual void Draw(Renderer& renderer);
protected:
private:
	SceneSplashAUT(const SceneSplashAUT& SplashScreen);
	SceneSplashAUT& operator=(const SceneSplashAUT& SplashScreen);
	// Member data:
public:
protected:
	Sprite* m_pCentre;
	float Fade;
	int AlphaDir;

private:
};
#endif // SCENESPLASHAUT_H