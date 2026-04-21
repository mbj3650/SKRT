// COMP710 JESSE
// This include:
#include "Director.h"

// Local includes:
#include "renderer.h"

// Library includes:
#include <cassert>

Director::Director()
{

};
Director::~Director()
{

};

bool
Director::Initialise(Renderer& renderer)
{
	return false;
};

void
Director::Process(float deltaTime)
{
	assert(0);
};
void
Director::Draw(Renderer& renderer)
{
	assert(0);
};
