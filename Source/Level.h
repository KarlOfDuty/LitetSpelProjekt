#ifndef LEVEL_H
#define LEVEL_H
#include "Model.h"
#include <SFML/Window.hpp>
class Level
{
private:
	std::vector<Model*> modelLibrary;
	std::vector<Model*> staticModels;
	std::vector<Model*> dynamicModels;
	std::vector<std::string> modelFilePaths =
	{
		"models/cube/cube.obj"
		,"models/sphere/sphere.obj"
		,"models/cube/cubeGreen.obj"
		,"models/Characters/Bird/BirdTest1.obj"
	};
public:
	void loadModels();
	void setupModels();
	void unloadModels();
	std::vector<Model*> getStaticModels();
	Level();
	~Level();
};
#endif