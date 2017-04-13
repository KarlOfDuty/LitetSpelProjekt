#include "Level.h"
std::vector<Model*> Level::getStaticModels()
{
	return staticModels;
}
void Level::loadModels()
{
	//Loads all models
	for (int i = 0; i < modelFilePaths.size(); i++)
	{
		modelLibrary.push_back(new Model(modelFilePaths[i]));
	}
}
void Level::setupModels()
{
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		10.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 2.0, 0.0,
		0.0, -1.0, 0.0, 1.0
	}));
	std::srand(time(0));
	//Loads spheres in random positions
	for (int i = 0; i < 0; i++)
	{
		staticModels.push_back(new Model(modelLibrary.at(1), {
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			(rand() % 100) - 50, (rand() % 100) - 50, (rand() % 100) - 100, 1.0 }));
	}
}
Level::Level()
{

}

Level::~Level()
{

}