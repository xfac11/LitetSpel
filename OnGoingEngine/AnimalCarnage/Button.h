#ifndef BUTTON_H
#define BUTTON_H

#include "GuiElement.h"
#include "Texture.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

class Button : public GuiElement
{
private:
	static bool texturesLoaded;
	static Texture texture;
	static Texture textureSelected;

	std::string text;

public:
	Button(std::string text, Vector2 position = Vector2::Zero);
	~Button();

	bool render(bool selected);

	void setText(std::string text);
};

#endif // !BUTTON_H
