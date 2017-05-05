#include "SoundSystem.h"
bool SoundSystem::playMusic(std::string path)
{
	if (soundEnabled)
	{
		if (music.openFromFile(path))
		{
			music.play();
			music.setLoop(true);
			return true;
		}
	}
	return false;
}
void SoundSystem::stopMusic()
{
	if (soundEnabled)
	{
		music.stop();
	}
}
//Loads a sound from file into soundbuffer, name is used as an identifier to play the sound
bool SoundSystem::loadSound(std::string path, std::string name)
{
	if (soundEnabled)
	{
		soundBuffer.push_back(sf::SoundBuffer());
		names.push_back(name);
		if (soundBuffer[soundBuffer.size() - 1].loadFromFile(path))
		{
			return true;
		}
		std::cout << "Sound loading error." << std::endl;
	}
	return false;
}
//Plays a sound from the sound buffer
bool SoundSystem::playSound(std::string name)
{
	if (soundEnabled)
	{
		for (int i = 0; i < names.size(); i++)
		{
			if (names[i] == name)
			{
				sf::Sound* sound = new sf::Sound();
				sound->setBuffer(soundBuffer[i]);
				sound->setVolume(5);
				sound->play();
				sounds.push_back(sound);
			}
		}
	}
	return false;
}
SoundSystem::SoundSystem()
{

}

SoundSystem::~SoundSystem()
{

}