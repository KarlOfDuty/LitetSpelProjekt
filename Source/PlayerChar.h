#ifndef PLAYERCHAR_H
#define PLAYERCHAR_H
#include "Model.h"
#include "Projectile.h"

class PlayerChar
{
private:
	int damage;
	Model* playerModel;
	bool inWater;

	//Animation animation;
public:
	PlayerChar();
	PlayerChar(Model* model, bool inWater);
	virtual ~PlayerChar();
	void setSwim(bool swiming);
	int getDamage() const;
	void update(float dt);
	void draw(Shader shader);
	virtual int getMaxJumps() = 0;
	virtual float getJumpHeight() = 0;
	Model* getModel();
};
#endif