#include "SoundSystem.h"
bool SoundSystem::playMusic(std::string path)
{
	if (music.openFromFile(path))
	{
		music.play();
		music.setLoop(true);
		return true;
	}
	return false;
}
//Loads a sound from file into soundbuffer
bool SoundSystem::loadSound(std::string path, std::string name)
{
	soundBuffer.push_back(new sf::SoundBuffer());
	if (soundBuffer[soundBuffer.size()-1]->loadFromFile(path))
	{

	}
	return false;
}
//Plays a sound from the sound buffer
bool SoundSystem::playSound(std::string name)
{
	for (int i = 0; i < names.size(); i++)
	{
		if (names[i] == name)
		{

		}
	}
	return false;
}
void SoundSystem::clearSounds()
{
	for (int i = 0; i < sounds.size(); i++)
	{
		if (!sounds[i]->getStatus())
		{
			i--;
			sounds.erase(sounds.begin() + i);
		}
	}
}
SoundSystem::SoundSystem()
{

}

SoundSystem::~SoundSystem()
{

}