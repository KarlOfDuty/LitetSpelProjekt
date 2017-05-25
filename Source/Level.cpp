#include "Level.h"

//Loads a single copy of all models into memory
void Level::loadModels()
{
	for (int i = 0; i < modelFilePaths.size(); i++)
	{
		modelLibrary.push_back(new Model(modelFilePaths[i]));
	}
}
//Places the loaded models in the world, keeping pointers to the original 
//meshes to avoid copying large amounts of data in memory
void Level::setupModels()
{
	//Boss stuff
	//remove these
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		4.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		60.0, 13.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		35.0, 3.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		40.0, 3.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		45.0, 3.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		50.0, 3.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		55.0, 3.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		25.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		45.0, 8.0, 0.0, 1.0
	}));


	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		25.0, -2.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		25.0, 3.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		25.0, 7.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		30.0, -2.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		30.0, 3.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		30.0, 8.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		30.0, 11.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		30.0, 21.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		1.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		32.0, 26.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		30.0, 31.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		25.0, 21.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		35.0, -2.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		40.0, -2.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		40.0, 8.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		45.0, -2.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		45.0, -2.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		50.0, -2.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		55.0, -2.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		60.0, -2.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		60.0, 3.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		60.0, 8.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		60.0, 10.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		60.0, 18.0, 0.0, 1.0
	}));
	//--------------------

	staticModels.push_back(new Model(modelLibrary[0],
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		-13.0, -2.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		-13.0, -2.0, 0.0, 1.0
	}));
	staticModels.push_back(new Model(*(modelLibrary.at(2)),
	{
		5.0, 0.0, 0.0, 0.0,
		0.0, 5.0, 0.0, 0.0,
		0.0, 0.0, 5.0, 0.0,
		-18.0, -2.0, 0.0, 1.0
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
		19.0, 3.0, 0.0, 1.0
	}));

	std::srand((int)time(0));
	//Loads spheres in random positions
	for (int i = 0; i < 0; i++)
	{
		staticModels.push_back(new Model(modelLibrary[1], {
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			(rand() % 100) - 50, (rand() % 100) - 50, (rand() % 100) - 50, 1.0 }));
		//std::cout << "Loaded." << std::endl;
	}
}
//Delete all models from memory
void Level::unloadModels()
{
	for (int i = 0; i < modelLibrary.size(); i++)
	{
		//Only the model library deletes it's meshes as the others only have pointers to these meshes
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
//Sets the triggerboxes for this level
void Level::setupTriggers(Player* player)
{
	std::vector<glm::vec2> corners = { glm::vec2(3,2), glm::vec2(6,2), glm::vec2(3,0), glm::vec2(6,0) };
	TriggerSettings settings;
	settings.onEnter = true;
	settings.onExit = true;
	triggerBoxes.push_back(new Trigger(corners, settings, player, player, "endLevel"));

	//water land
	std::vector<glm::vec2> corners2 = { glm::vec2(9,5), glm::vec2(15,5), glm::vec2(9,0), glm::vec2(15,0) };
	TriggerSettings settings2;
	settings2.onEnter = true;
	settings2.onExit = true;
	triggerBoxes.push_back(new Trigger(corners2, settings2, player, player, "hellogais"));

	//health pickup
	glm::mat4 mat({
		0.02, 0, 0, 0,
		0, 0.02, 0, 0,
		0, 0, 0.02, 0,
		-4, 5, 0, 1
	});
	Model* heart = new Model(modelLibrary[4], mat);
	glm::vec3 min, max;
	heart->getMinMaxBouding(min, max);
	min += glm::vec3(mat[3]);
	max += glm::vec3(mat[3]);
	heart->setRotationMatrix(glm::rotate(glm::mat4(), glm::radians(-6.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	dynamicModels.push_back(heart);

	std::vector<glm::vec2> corners3 = { glm::vec2(min), glm::vec2(min.x,max.y), glm::vec2(max), glm::vec2(max.x,min.y) };
	TriggerSettings settings3;
	settings3.onEnter = true;
	std::vector<GameObject*> shitVec;
	shitVec.push_back(player);
	shitVec.push_back(heart);
	Trigger* fuck = new Trigger(heart->getPoints(), settings3, player, shitVec, "healthPickup");
	triggerBoxes.push_back(fuck);
}
void Level::updateTriggers(float dt)
{
	for (int i = 0; i < triggerBoxes.size(); i++)
	{
		triggerBoxes[i]->update(dt);
	}
	for (int i = 0; i < dynamicModels.size(); i++)
	{
		dynamicModels[i]->rotate();
	}
}
void Level::deleteTriggers()
{
	for (int i = 0; i < triggerBoxes.size(); i++)
	{
		delete triggerBoxes[i];
	}
	triggerBoxes.clear();
}
void Level::playMusic(SoundSystem* soundSystem)
{

}
void Level::stopMusic(SoundSystem* soundSystem)
{

}
//void Level::playMusic(SoundSystem *soundSystem)
//{
//	soundSystem->playMusic("audio/music/never.flac");
//}
//void Level::stopMusic(SoundSystem *soundSystem)
//{
//	soundSystem->stopMusic();
//}
//Getters
std::vector<Model*>& Level::getStaticModels()
{
	return staticModels;
}
std::vector<Model*> Level::getDynamicModels()
{
	return dynamicModels;
}
std::vector<Trigger*> Level::getTriggers()
{
	return std::vector<Trigger*>();
}
glm::vec3 Level::getPlayerPos()
{
	return playerPos;
}
//Constructors
Level::Level()
{
	modelFilePaths =
	{
		"models/cube/cube.obj"
		,"models/sphere/sphere.obj"
		,"models/cube/cubeGreen.obj"
		,"models/Characters/Bird/BirdTest1.obj"
		,"models/heart/HeartContainer.obj"
	};
	playerPos = glm::vec3(14,4,0);
}
Level::Level(std::string filepath)
{
	//Imports this level from file
}
//Destructor
Level::~Level()
{

}