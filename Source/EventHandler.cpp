#include"EventHandler.h"
EventHandler::EventHandler()
{

}

EventHandler::~EventHandler()
{

}
//Add any events that need to be handled here
bool EventHandler::handleEvents(sf::Window & window, Player *player)
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
			player->jump();
		}
		else if (windowEvent.type == sf::Event::JoystickButtonReleased && windowEvent.joystickButton.joystickId == controller && windowEvent.joystickButton.button == XBOXA)
		{
			joystickPressed[controller][XBOXA] = false;
		}
		else if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Space && !keyPressed[sf::Keyboard::Space])
		{
			keyPressed[sf::Keyboard::Space] = true;
			player->jump();
		}
		else if (windowEvent.type == sf::Event::MouseButtonReleased && windowEvent.key.code == sf::Mouse::Button::Left && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
		{
			player->useLightAttack(window);
		}
		else if (windowEvent.type == sf::Event::MouseButtonReleased && windowEvent.key.code == sf::Mouse::Button::Right)
		{
			player->useLightAttack(window);
		}
		else if (windowEvent.type == sf::Event::MouseWheelScrolled && windowEvent.key.code == sf::Mouse::VerticalWheel)
		{
			player->useLightAttack(window);
		}
		else if (windowEvent.type == sf::Event::KeyReleased && windowEvent.key.code == sf::Keyboard::Space)
		{
			keyPressed[sf::Keyboard::Space] = false;
		}
		else if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Z && !keyPressed[sf::Keyboard::Z])
		{
			keyPressed[sf::Keyboard::Z] = true;
			player->swap(0);
		}
		else if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::X && !keyPressed[sf::Keyboard::X])
		{
			keyPressed[sf::Keyboard::X] = true;
			player->swap(1);
		}
		else if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::C && !keyPressed[sf::Keyboard::C])
		{
			keyPressed[sf::Keyboard::C] = true;
			player->swap(2);
		}
		else if (windowEvent.type == sf::Event::KeyReleased && windowEvent.key.code == sf::Keyboard::X)
		{
			keyPressed[sf::Keyboard::X] = false;
		}
		else if (windowEvent.type == sf::Event::KeyReleased && windowEvent.key.code == sf::Keyboard::Z)
		{
			keyPressed[sf::Keyboard::Z] = false;
		}
		else if (windowEvent.type == sf::Event::KeyReleased && windowEvent.key.code == sf::Keyboard::C)
		{
			keyPressed[sf::Keyboard::C] = false;
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
		//Menu/End program
		else if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Escape)
		{
			running = false;
		}
		else if (windowEvent.type == sf::Event::JoystickButtonPressed && windowEvent.joystickButton.joystickId == controller && windowEvent.joystickButton.button == XBOXSTART)
		{
			running = false;
		}
	}
	return running;
}