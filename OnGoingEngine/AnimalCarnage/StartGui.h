#ifndef START_GUI_H
#define START_GUI_H

#include "GuiBase.h"
#include "Texture.h"

class StartGui : public GuiBase
{
private:
	std::shared_ptr<Texture> texture;
	float timer;

public:
	StartGui(State* myState);
	virtual ~StartGui();

	bool initialize();
	void shutDown();
	bool update(float deltaTime);
	bool render();
};

#endif // !START_GUI_H
