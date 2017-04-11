#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H
#include <SFML\Window.hpp>
#include "Player.h"
class EventHandler
{
private:
	enum { XBOXA, XBOXB, XBOXX, XBOXY };
	enum { CONTROLLER0, CONTROLLER1, CONTROLLER2, CONTROLLER3};
	//[Controller][Button]
	bool joystickPressed[8][4] = { false };
	//[Key]
	bool keyPressed[120] = { false };
public:
	EventHandler();
	~EventHandler();
	bool handleEvents(sf::Window &window, float dt, Player *player);
};
#endif