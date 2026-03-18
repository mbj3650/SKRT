// COMP710 JESSE
#include "vector2.h"
#ifndef __ALIENINVADER_H__
#define __ALIENINVADER_H__

// Forward declarations:
class Renderer;
class Sprite;
// Class declaration:
class Alieninvader
{
	// Member methods:
public:
	Alieninvader();
	~Alieninvader();

	void ArrayInit(Alieninvader* replacement[]);

	bool Initialise(Renderer& renderer);
	void Process(float deltaTime);
	void Draw(Renderer& renderer);
	float GetRadius();
	void Rowdown();
	void Kill();

	Vector2& Position();
	virtual void DebugDraw();

	bool isAlive();
	void Speedup();
	bool GetHitting();

protected:
	void ComputeBounds(int width, int height);
private:
	Alieninvader(const Alieninvader& Alieninvader);
	Alieninvader& operator=(const Alieninvader& Alieninvader);

	// Member data:
public:

protected:
	Vector2 m_position;
	Vector2 m_boundaryLow;
	Vector2 m_boundaryHigh;
	Sprite* m_pSprite;
	bool m_bAlive;
	static float direction;
	static bool Hittingboundary;
	static Alieninvader* List[55];
	static int madesofar;
	static float sm_fBoundaryWidth;
	static float sm_fBoundaryHeight;
	float radius;
private:

};

#endif // __ALIENINVADER_H__
