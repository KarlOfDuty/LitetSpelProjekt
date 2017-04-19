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
	staticModels.push_back(new Model(modelLibrary[0],
	{
		10.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 2.0, 0.0,
		0.0, -1.0, 0.0, 1.0
	}));
	std::srand(time(0));
	//Loads spheres in random positions
	for (int i = 0; i < 10000; i++)
	{
		staticModels.push_back(new Model(modelLibrary[0], {
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			(rand() % 100) - 50, (rand() % 100) - 50, (rand() % 100) - 100, 1.0 }));
		std::cout << "Loaded." << std::endl;
	}
}
void Level::unloadModels()
{
	for (int i = 0; i < modelLibrary.size(); i++)
	{
		modelLibrary[i]->deleteMeshes();
		delete modelLibrary[i];
	}
	modelLibrary.clear();

	for (int i = 0; i < staticModels.size(); i++)
	{
		delete staticModels[i];
	}
	staticModels.clear();

	for (int i = 0; i < dynamicModels.size(); i++)
	{
		delete dynamicModels[i];
	}
	dynamicModels.clear();
}
Level::Level()
{

}

Level::~Level()
{

}