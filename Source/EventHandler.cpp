#include"EventHandler.h"
EventHandler::EventHandler()
{
	youDied = false;
}

EventHandler::~EventHandler()
{

}
//Add any events that need to be handled here
bool EventHandler::handleEvents(sf::Window & window, Player *player, SoundSystem * soundSystem)
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
		else if (windowEvent.type == sf::Event::MouseButtonPressed && windowEvent.key.code == sf::Mouse::Button::Left)
		{
			player->lightAttackPressed(window);
		}
		else if (windowEvent.type == sf::Event::MouseButtonReleased && windowEvent.key.code == sf::Mouse::Button::Left)
		{
			player->lightAttackReleased(window);
		}
		else if (windowEvent.type == sf::Event::MouseButtonPressed && windowEvent.key.code == sf::Mouse::Button::Right)
		{
			player->heavyAttackPressed(window);
		}
		else if (windowEvent.type == sf::Event::MouseButtonReleased && windowEvent.key.code == sf::Mouse::Button::Right)
		{
			player->heavyAttackReleased(window);
		}
		else if (windowEvent.type == sf::Event::MouseWheelScrolled && windowEvent.key.code == sf::Mouse::VerticalWheel)
		{
			player->lightAttackPressed(window);
			soundSystem->playSound("bowRelease");
		}
		else if (windowEvent.type == sf::Event::KeyReleased && windowEvent.key.code == sf::Keyboard::Space)
		{
			keyPressed[sf::Keyboard::Space] = false;
		}
		else if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Num1 && !keyPressed[sf::Keyboard::Num1])
		{
			keyPressed[sf::Keyboard::Num1] = true;
			player->swap(0);
		}
		else if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Num2 && !keyPressed[sf::Keyboard::Num2])
		{
			keyPressed[sf::Keyboard::Num2] = true;
			player->swap(1);
		}
		else if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Num3 && !keyPressed[sf::Keyboard::Num3])
		{
			keyPressed[sf::Keyboard::Num3] = true;
			player->swap(2);
		}
		else if (windowEvent.type == sf::Event::KeyReleased && windowEvent.key.code == sf::Keyboard::Num1)
		{
			keyPressed[sf::Keyboard::Num1] = false;
		}
		else if (windowEvent.type == sf::Event::KeyReleased && windowEvent.key.code == sf::Keyboard::Num2)
		{
			keyPressed[sf::Keyboard::Num2] = false;
		}
		else if (windowEvent.type == sf::Event::KeyReleased && windowEvent.key.code == sf::Keyboard::Num3)
		{
			keyPressed[sf::Keyboard::Num3] = false;
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
	if (player->getHealth() <= 0 && !youDied)
	{
		soundSystem->playSound("youDied");
		youDied = true;
	}
	return running;
}