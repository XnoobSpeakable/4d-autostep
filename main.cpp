//#define DEBUG_CONSOLE // Uncomment this if you want a debug console to start. You can use the Console class to print. You can use Console::inStrings to get input.

#include <4dm.h>

using namespace fdm;

// Initialize the DLLMain
initDLL


$hook(void, Player, updatePos, World* world, double dt)
{
	if (self != &fdm::StateGame::instanceObj.player) return;
	if ((BlockInfo::TYPE)world->getBlock(self->pos + self->vel*(float)dt)!=BlockInfo::AIR  && ((BlockInfo::TYPE)world->getBlock(self->pos + self->vel*(float)dt + glm::vec4{0,1,0,0}))==BlockInfo::AIR) {
		self->pos.y += 1;

	}
	original(self, world, dt);
}



$hook(void, StateIntro, init, StateManager& s)
{
	original(self, s);

	// initialize opengl stuff
	glewExperimental = true;
	glewInit();
	glfwInit();
}
