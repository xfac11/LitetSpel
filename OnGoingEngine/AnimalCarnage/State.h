#ifndef STATE_H
#define STATE_H
#include"GameObject.h"
class State
{
public:
	State();
	virtual ~State();
	virtual bool initailize() = 0;
	virtual bool render() = 0;
	virtual bool update() = 0;
	virtual void shutDown() = 0;
protected:
	//UI * ui;
	//vector<Gameobject> objects;
	//vector<Gameobject> transObjects;

};
#endif // !STATE_H
