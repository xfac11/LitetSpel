#ifndef GUN_GAME_STATE_H
#define GUN_GAME_STATE_H

#include "State.h"

class GunGameState : public State
{
public:
	GunGameState();
	~GunGameState();

	bool initailize();
	bool render();
	bool update(float deltaTime);
	void shutDown();
};

#endif // !GUN_GAME_STATE_H
