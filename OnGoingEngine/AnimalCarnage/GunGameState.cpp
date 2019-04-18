#include "GunGameState.h"

GunGameState::GunGameState()
{
	this->player = nullptr;
	this->nrOfPlayers = 0;
}

GunGameState::~GunGameState()
{
	shutDown();
}

bool GunGameState::initailize()
{
	player = new Player * [nrOfPlayers];
	return true;
}

bool GunGameState::render()
{

	return true;
}

bool GunGameState::update(float deltaTime)
{
	for (int i = 0; i < nrOfPlayers; i++)
	{
		player[i]->update();
		//swap characters
		//player[i]->SelectCharacter();
		for (int j = 0; j < nrOfPlayers; j++)
		{
			//collision detection
			if (Intersects(player[i]->GetBody(), player[j]->GetBody()))
			{
				//logic
			}
		}

	}

	return true;
}

void GunGameState::shutDown()
{
	for (int i = 0; i < nrOfPlayers; i++)
	{
		delete player[i];
	}
	delete[] player;
}
