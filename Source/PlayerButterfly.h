#ifndef PLAYERBUTTERFLY_H
#define PLAYERBUTTERFLY_H


class PlayerButterfly : public PlayerChar
{
private:
	
public:
	PlayerButterfly();
	~PlayerButterfly();
	void teleport();
	void jump();
	void moveLeft();
	void moveRight();
	void attack();

};
#endif
