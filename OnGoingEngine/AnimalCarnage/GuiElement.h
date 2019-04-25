#ifndef GUI_ELEMENT_H
#define GUI_ELEMENT_H

#include "SpriteBatch.h"
#include <d3d11.h>
#include "SimpleMath.h"
#include <functional>

class GuiElement
{
protected:
	DirectX::SimpleMath::Vector2 position;
	GuiElement* left;
	GuiElement* right;
	GuiElement* up;
	GuiElement* down;

public:
	GuiElement(DirectX::SimpleMath::Vector2 position);
	virtual ~GuiElement();

	virtual bool render(bool selected);

	void setConnectedElements(GuiElement* left, GuiElement* right, GuiElement* up, GuiElement* down);

	DirectX::SimpleMath::Vector2 getPosition() const;
	GuiElement* getLeft() const;
	GuiElement* getRight() const;
	GuiElement* getUp() const;
	GuiElement* getDown() const;
};

#endif // !GUI_ELEMENT_H
