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
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		-8.0, 3.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		-8.0, -2.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		-3.0, -2.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		2.0, -2.0, 0.0, 1.0
	}));
	Model* slope = new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		8.0, 0.5, 0.0, 1.0
	});
	slope->setRotationMatrix(glm::rotate(glm::mat4(), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	slope->rotate();
	staticModels.push_back(slope);
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		14.0, -2.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		19.0, -2.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		19.0, 3.0, 0.0, 1.0
	}));

	std::srand(time(0));
	//Loads 1000 spheres randomly
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