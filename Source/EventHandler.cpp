#include"EventHandler.h"
EventHandler::EventHandler()
{

}

EventHandler::~EventHandler()
{

}
//Add any events that need to be handled here
bool EventHandler::handleEvents(sf::Window & window, float dt, Player *player)
{
	//If several controllers are plugged in, this decides which is used
	int controller = CONTROLLER0;
	bool running = true;
	//Handle events
	sf::Event windowEvent;
	while (window.pollEvent(windowEvent))
	{
		//Jump
		if (windowEvent.type == sf::Event::JoystickButtonPressed && joystickPressed[controller][XBOXA] == false && windowEvent.joystickButton.joystickId == controller && windowEvent.joystickButton.button == XBOXA)
		{
			joystickPressed[controller][XBOXA] = true;
			player->jump(dt);
		}
		else if (windowEvent.type == sf::Event::JoystickButtonReleased && windowEvent.joystickButton.joystickId == controller && windowEvent.joystickButton.button == XBOXA)
		{
			joystickPressed[controller][XBOXA] = false;
		}
		else if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Space && !keyPressed[sf::Keyboard::Space])
		{
			keyPressed[sf::Keyboard::Space] = true;
			player->jump(dt);
		}
		else if (windowEvent.type == sf::Event::KeyReleased && windowEvent.key.code == sf::Keyboard::Space)
		{
			keyPressed[sf::Keyboard::Space] = false;
		}
		else if (windowEvent.type == sf::Event::Closed)
		{
			//End the program
			running = false;
		}
		else if (windowEvent.type == sf::Event::Resized)
		{
			//Adjust the viewport when the window is resized
			glViewport(0, 0, windowEvent.size.width, windowEvent.size.height);
		}
		else if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Escape)
		{
			running = false;
		}
	}
	return running;
}