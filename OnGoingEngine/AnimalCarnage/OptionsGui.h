#ifndef OPTIONS_GUI_H
#define OPTIONS_GUI_H

#include "GuiBase.h"

class OptionsGui : public GuiBase
{
public:
	OptionsGui();
	virtual ~OptionsGui();

	bool initialize();
	void shutDown();
	bool update(float deltaTime);
	bool render();
};

#endif // !OPTIONS_GUI_H
