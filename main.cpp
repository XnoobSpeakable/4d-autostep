//#define DEBUG_CONSOLE // Uncomment this if you want a debug console to start. You can use the Console class to print. You can use Console::inStrings to get input.

#include <4dm.h>

using namespace fdm;

// Initialize the DLLMain
initDLL

glm::vec4 safeNormalize(const glm::vec4& v)
{
	float sqrLength = glm::dot(v, v);
	if (sqrLength <= 0.0001f * 0.0001f)
		sqrLength = 1.0f;
	float length = glm::sqrt(sqrLength);
	return v * (1.0f / length);
}

// ZiHEd and Tr1Ngle mostly made this lol, XnoobSpeakable just a little bit of it
$hook(void, Player, updatePos, World* world, double dt)
{
	// make sure it is the local player
	if (self != &fdm::StateGame::instanceObj.player) return original(self, world, dt);

	// weird length formula from updatePos
	float length = glm::length(self->vel)
		+ (glm::length(self->deltaVel) * 0.5f) * (dt * dt);

	// ray dir
	glm::vec4 dir = safeNormalize(self->vel + self->deltaVel);
	// ray origin
	glm::vec4 origin = self->pos + glm::vec4{ 0,0.05f,0,0 }; // add a tiny offset up

	// don't step if moving down or if not moving
	if (glm::dot(dir, { 0, 1, 0, 0 }) <= -0.01f || length <= 0.0001f)
		return original(self, world, dt);

	glm::ivec4 curBlock = glm::floor(origin);
	glm::vec4 endpoint = origin + dir * length * (float)dt;
	glm::ivec4 intersection = glm::floor(endpoint);
	// don't do anything if the intersection is on the same block as the origin
	if (curBlock == intersection) return original(self, world, dt);

	// raycast to check for a wall
	if (world->castRay(origin, curBlock, intersection, endpoint))
	{
		uint8_t intersectionBlock = world->getBlock(intersection);
		if (intersection.y == glm::floor(origin.y) &&
			BlockInfo::Blocks[intersectionBlock].solid &&
			!BlockInfo::Blocks[world->getBlock(intersection + glm::ivec4{0,1,0,0})].solid &&
			!BlockInfo::Blocks[world->getBlock(intersection + glm::ivec4{0,2,0,0})].solid)
		{
			// move the player onto the block
			self->pos += dir * length * (float)dt;
			self->pos.y = intersection.y + 1;
			// update currentBlock
			self->currentBlock = glm::round(self->pos);
			self->groundBlock = intersectionBlock;
			self->touchingGround = true;
		}
	}
 
	return original(self, world, dt);
}
//attempting to steal code to add settings, i will change this and do stuff to it
/*
using namespace fdm::gui;

Text autostepTitle;
CheckBox autostepCB;

$hook(void, StateSettings, init, StateManager& s)
{
    original(self, s);

    waypointsTitle = Text{};
    waypointsTitle.setText("Waypoints:");
    waypointsTitle.alignX(ALIGN_CENTER_X);
    waypointsTitle.offsetY(self->skinsCheckBox.yOffset + 75);
    waypointsTitle.size = 2;
    self->mainContentBox.addElement(&waypointsTitle);

    showWaypointsCB = CheckBox{};
    showWaypointsCB.text = "Display Waypoints";
    showWaypointsCB.alignX(ALIGN_CENTER_X);
    showWaypointsCB.offsetY(waypointsTitle.yOffset + 75);
    showWaypointsCB.checked = StateWaypointsMenu::showWaypoints;
    showWaypointsCB.callback = showWaypointsCBCallback;
    self->mainContentBox.addElement(&showWaypointsCB);
}

$hook(void, StateSettings, load, GLFWwindow* window)
{
    original(self, window);

    std::ifstream settingsFile("settings.json");
    if (!settingsFile.is_open()) return;
    nlohmann::json settings = nlohmann::json::parse(settingsFile);
    settingsFile.close();

    if (!settings.contains("showWaypoints") || !settings["showWaypoints"].is_boolean())
        settings["showWaypoints"] = true;
    StateWaypointsMenu::showWaypoints = settings["showWaypoints"];

    if (!settings.contains("showWaypointCoords") || !settings["showWaypointCoords"].is_boolean())
        settings["showWaypointCoords"] = true;
    StateWaypointsMenu::showCoords = settings["showWaypointCoords"];

    if (!settings.contains("maxScale") || !settings["maxScale"].is_number_float())
        settings["maxScale"] = 1.5f;
    StateWaypointsMenu::maxScale = settings["maxScale"];

    if (!settings.contains("maxScaleDistance") || !settings["maxScaleDistance"].is_number_float())
        settings["maxScaleDistance"] = 100.f;
    StateWaypointsMenu::maxScaleDistance = settings["maxScaleDistance"];
}
*/

$hook(void, StateIntro, init, StateManager& s)
{
	original(self, s);

	// initialize opengl stuff
	glewExperimental = true;
	glewInit();
	glfwInit();
}
