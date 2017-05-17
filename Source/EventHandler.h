#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H
#include <SFML\Window.hpp>
#include "Player.h"
#include "SoundSystem.h"
#include "Menu.h"
class EventHandler
{
private:
	enum { XBOXA, XBOXB, XBOXX, XBOXY, GFDGF, SDFDSF, DSFDSFDSFSDF, XBOXSTART };
	enum { CONTROLLER0, CONTROLLER1, CONTROLLER2, CONTROLLER3};
	//Bools used for toggleable keys
	//[Controller][Button]
	bool joystickPressed[8][30] = { false };
	//[Key]
	bool keyPressed[120] = { false };
	bool youDied;
	int running;
public:
	EventHandler();
	~EventHandler();
	int handleEvents(sf::Window &window, Player *player, SoundSystem *soundSystem, Menu * menu);
};
#endif
