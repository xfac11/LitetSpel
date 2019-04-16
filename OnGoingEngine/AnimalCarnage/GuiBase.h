#ifndef GUI_BASE_H
#define GUI_BASE_H

class GuiBase
{
public:
	GuiBase();
	virtual ~GuiBase();

	virtual bool initialize() = 0;
	virtual void shutDown() = 0;
	virtual bool update(float deltaTime) = 0;
	virtual bool render() = 0;
};

#endif // !GUI_H
