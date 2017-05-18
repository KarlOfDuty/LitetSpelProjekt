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

	music.stop();

}
void SoundSystem::stopSound()
{
	stopMusic();
	stopSfx();
}
void SoundSystem::stopSfx()
{
	//sfx.stop();
}
void SoundSystem::startMusic()
{
	if (soundEnabled)
	{
		music.play();
	}
}
void SoundSystem::startSound()
{
	startMusic();
	startSfx();
}
void SoundSystem::startSfx()
{
	if (/*sfxEnabled*/true)
	{
		//sfx.start();
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
bool SoundSystem::getMusicEnabled()
{
	return this->musicEnabled;
}
void SoundSystem::setMusicEnabled(bool state)
{
	this->musicEnabled = state;
}
bool SoundSystem::getSoundEnabled()
{
	return this->soundEnabled;
}
void SoundSystem::setSoundEnabled(bool state)
{
	this->soundEnabled = state;
}
bool SoundSystem::getSfxEnabled()
{
	return this->sfxEnabled;
}
void SoundSystem::setSfxEnabled(bool state)
{
	this->sfxEnabled = state;
}
SoundSystem::SoundSystem()
{
	musicEnabled = true;
	soundEnabled = true;
	sfxEnabled = true;
}

SoundSystem::~SoundSystem()
{

}