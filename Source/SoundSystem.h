#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H
#include <string>
#include <iostream>
#include <SFML/Audio.hpp>
class SoundSystem
{
private:
	sf::Music music;
	std::vector<std::string> names;
	std::vector<sf::SoundBuffer> soundBuffer;
	std::vector<sf::Sound*> sounds;
public:
	bool playMusic(std::string path);
	void stopMusic();
	bool loadSound(std::string path, std::string name);
	bool playSound(std::string name);
	SoundSystem();
	~SoundSystem();
};
#endif