#include"EventHandler.h"
EventHandler::EventHandler()
{
	youDied = false;
	running = 1;
}

EventHandler::~EventHandler()
{

}
//Add any events that need to be handled here
int EventHandler::handleEvents(sf::Window & window, Player *player, SoundSystem * soundSystem, Menu * menu)
{
	//If several controllers are plugged in, this decides which is used
	int controller = CONTROLLER0;
	//0 = quit, 1 = menu, 2 = game
	bool quit = false;
	//Handle events
	sf::Event windowEvent;
	while (window.pollEvent(windowEvent))
	{
		if (running == 1)
		{
			if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Up)
			{
				menu->MoveUp();
			}
			else if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Down)
			{
				menu->MoveDown();
			}
			else if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Return)
			{
				int action = menu->Select();
				switch (action)
				{
				case(0):
					running = 2;
					break;
				case(1):
					//options menu
					break;
				case(2):
					running = 0;
					break;
				}
			}
			//Menu/End program
			else if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Escape)
			{
				running = 0;
			}
			else if (windowEvent.type == sf::Event::JoystickButtonPressed && windowEvent.joystickButton.joystickId == controller && windowEvent.joystickButton.button == XBOXSTART)
			{
				running = 0;
			}
			else if (windowEvent.type == sf::Event::Closed)
			{
				//End the program
				running = 0;
			}
			else if (windowEvent.type == sf::Event::Resized)
			{
				//Adjust the viewport when the window is resized
				glViewport(0, 0, windowEvent.size.width, windowEvent.size.height);
			}
		}
		else if(running == 2)
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
				player->setCurrentKeyframe(1);
				player->setAnimationIndex(4);
				player->jump();
			}
			else if (windowEvent.type == sf::Event::MouseButtonPressed && windowEvent.key.code == sf::Mouse::Button::Left)
			{
				player->lightAttackPressed(window);
				player->setAnimationIndex(2);
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
				player->setCurrentKeyframe(1);
				player->swap(0);
			}
			else if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Num2 && !keyPressed[sf::Keyboard::Num2])
			{
				keyPressed[sf::Keyboard::Num2] = true;
				player->setCurrentKeyframe(1);
				player->swap(1);
			}
			else if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Num3 && !keyPressed[sf::Keyboard::Num3])
			{
				keyPressed[sf::Keyboard::Num3] = true;
				player->setCurrentKeyframe(1);
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
				running = 0;
			}
			else if (windowEvent.type == sf::Event::Resized)
			{
				//Adjust the viewport when the window is resized
				glViewport(0, 0, windowEvent.size.width, windowEvent.size.height);
			}
			//Menu/End program
			else if (windowEvent.type == sf::Event::KeyPressed && windowEvent.key.code == sf::Keyboard::Escape)
			{
				running = 1;
			}
			else if (windowEvent.type == sf::Event::JoystickButtonPressed && windowEvent.joystickButton.joystickId == controller && windowEvent.joystickButton.button == XBOXSTART)
			{
				running = 1;
			}
		}
	}
	if (player->getHealth() <= 0 && !youDied)
	{
		soundSystem->playSound("youDied");
		youDied = true;
	}
	return running;
}
