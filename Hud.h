// COMP710 JESSE
#ifndef __HUD_H__
#define __HUD_H__

// Forward declarations:
class Renderer;

// Class declaration:
class Hud
{
	// Member methods:
public:
	Hud();
	~Hud();

	bool Initialise(Renderer& renderer);
	void Process(float deltaTime);
	void Draw(Renderer& renderer);

protected:

private:
	Hud(const Hud& Hud);
	Hud& operator=(const Hud& Hud);

	// Member data:
public:

protected:

private:

};

#endif // __HUD_H__
