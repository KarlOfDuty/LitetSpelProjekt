#ifndef PLAYERBIRD_H
#define PLAYERBIRD_H


class PlayerBird: public PlayerChar
{
private:

public:
	PlayerBird();
	~PlayerBird();
	bool jumpAllowed();
	void jump();
	void moveLeft();
	void moveRight();
	void attack();

};
#endif
