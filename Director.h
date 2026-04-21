// COMP710 JESSE
#ifndef __DIRECTOR_H__
#define __DIRECTOR_H__

// Forward declarations:
class Renderer;

// Class declaration:
class Director
{
	// Member methods:
public:
	Director();
	~Director();

	bool Initialise(Renderer& renderer);
	void Process(float deltaTime);
	void Draw(Renderer& renderer);

protected:

private:
	Director(const Director& Director);
	Director& operator=(const Director& Director);

	// Member data:
public:

protected:

private:

};

#endif // __DIRECTOR_H__
