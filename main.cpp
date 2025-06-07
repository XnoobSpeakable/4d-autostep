//#define DEBUG_CONSOLE // Uncomment this if you want a debug console to start. You can use the Console class to print. You can use Console::inStrings to get input.

#include <4dm.h>

using namespace fdm;

// Initialize the DLLMain
initDLL

//ZiHEd made this lol
$hook(void, Player, updatePos, World* world, double dt)
{
    if (self != &fdm::StateGame::instanceObj.player) return;

    glm::vec4 nextPos = self->pos + self->vel*(float)dt + glm::vec4{0,0.001f,0,0}; // optional - add 0.001f just in case, floats can be funny
    glm::vec4 nextUpPos = nextPos + glm::vec4(0,1,0,0);

    bool isNextBlockSolid = BlockInfo::Blocks.at(world->getBlock(nextPos)).solid;
    bool isNextUpBlockSolid = BlockInfo::Blocks.at(world->getBlock(nextUpPos)).solid; 
    // also check if the upper block is solid, not air, so that you can step on blocks even if there are bushes on it

    if ( isNextBlockSolid && !isNextUpBlockSolid)
        self->pos.y += 1;
    
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