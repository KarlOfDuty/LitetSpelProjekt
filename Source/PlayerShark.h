#ifndef PLAYERSHARK_H
#define PLAYERSHARK_H


class PlayerShark : public PlayerChar
{
private:

public:
	PlayerShark();
	~PlayerShark();
	void dive();
	void jump();
	void moveLeft();
	void moveRight();
	void attack();

};
#endif
