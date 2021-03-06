#ifndef GUI_BASE_H
#define GUI_BASE_H

class State;

class GuiBase
{
protected:
	State* myState;
	float keyboardDelay;

public:
	GuiBase(State* myState);
	virtual ~GuiBase();

	virtual bool initialize() = 0;
	virtual void shutDown() = 0;
	virtual bool update(float deltaTime) = 0;
	virtual bool render() = 0;

	void activateDelay();
};

#endif // !GUI_H
