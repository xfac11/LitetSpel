#ifndef LOOSER_VIEW_H
#define LOOSER_VIEW_H

#include "GuiElement.h"
#include "Texture.h"

class LooserView : public GuiElement
{
private:
	static bool texturesLoaded;
	static Texture texture;

public:
	LooserView(DirectX::SimpleMath::Vector2 position = DirectX::SimpleMath::Vector2());
	virtual ~LooserView();

	bool render(bool selected);
};

#endif // !LOOSER_VIEW_H
