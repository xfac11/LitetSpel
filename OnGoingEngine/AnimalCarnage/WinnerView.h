#ifndef WINNER_VIEW_H
#define WINNER_VIEW_H

#include "GuiElement.h"
#include "Texture.h"

class WinnerView : public GuiElement
{
private:
	static bool texturesLoaded;
	static Texture texture;

public:
	WinnerView(DirectX::SimpleMath::Vector2 position = DirectX::SimpleMath::Vector2());
	virtual ~WinnerView();

	bool render(bool selected);
};

#endif // !WINNER_VIEW_H
