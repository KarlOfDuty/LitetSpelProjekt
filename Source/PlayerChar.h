#ifndef PLAYERCHAR_H
#define PLAYERCHAR_H
#include "Model.h"
#include "Projectile.h"

class PlayerChar
{
private:
	int health;
	int damage;
	Model playerModel;
	bool inWater;
	bool diving;
	//Animation animation;
public:
	PlayerChar();
	PlayerChar(int health, Model model, bool inWater);
	virtual ~PlayerChar();
	void applyDamage(int appliedDamage);
	void setHealth(int health);
	void setSwim(bool swiming);
	void setDiving(bool diving);
	int getDamage() const;
	int getHealth()const;
	int getDiving()const;
	virtual void lightAttack(std::vector<Projectile*> &allProjectiles, glm::vec2 position, glm::vec2 direction)const = 0;
	virtual void heavyAttack()const = 0;
	void draw(Shader shader);
	virtual int getMaxJumps() = 0;
	virtual float getJumpHeight() = 0;
	virtual void waterEffect() = 0;
	Model& getModel();
};
#endif