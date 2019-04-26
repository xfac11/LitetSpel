#ifndef PLAYER_SELECTOR_H
#define PLAYER_SELECTOR_H

#include "GuiElement.h"
#include "Texture.h"

enum Players
{
	WOLF
};

class PlayerSelector : public GuiElement
{
private:
	static bool texturesLoaded;
	static Texture selectorBG;
	static Texture playerCircle;
	static Texture arrowLeft;
	static Texture arrowRight;

	Players currentPlayer;

public:
	PlayerSelector(Players currentPlayer, DirectX::SimpleMath::Vector2 position = DirectX::SimpleMath::Vector2::Zero);
	virtual ~PlayerSelector();

	bool render(bool selected);
};

#endif // !PLAYER_SELECTOR_H