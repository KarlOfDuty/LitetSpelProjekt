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
	bool musicEnabled;
	bool soundEnabled;
	bool sfxEnabled;
public:
	bool playMusic(std::string path);
	void stopMusic();
	void stopSound();
	void stopSfx();
	void startMusic();
	void startSound();
	void startSfx();
	bool loadSound(std::string path, std::string name);
	bool playSound(std::string name);
	bool getMusicEnabled();
	void setMusicEnabled(bool state);
	bool getSoundEnabled();
	void setSoundEnabled(bool state);
	bool getSfxEnabled();
	void setSfxEnabled(bool state);
	SoundSystem();
	~SoundSystem();
};
#endif