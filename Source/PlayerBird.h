#include "PlayerChar.h"


class PlayerBird : public PlayerChar
{
private:
	Model birdModel;
public:
	PlayerBird(int HP);
	virtual ~PlayerBird();
	PlayerBird(const PlayerBird & originalObject);
	void operator=(const PlayerBird &originalObject);
	bool jumpAllowed();
	virtual Model getModel()const;
	virtual void jump();
	virtual void moveLeft();
	virtual void moveRight();
	virtual void attack();

};
