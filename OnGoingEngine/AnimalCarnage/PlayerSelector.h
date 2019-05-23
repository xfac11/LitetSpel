#ifndef PLAYER_SELECTOR_H
#define PLAYER_SELECTOR_H

#include "GuiElement.h"
#include "Texture.h"
#include "Player.h"

class PlayerSelector : public GuiElement
{
private:
	static bool texturesLoaded;
	static Texture selectorBG;
	static Texture playerCircle;
	static Texture arrowLeft;
	static Texture arrowRight;
	static Texture players[4];
	static Texture backGroundLayer;
	AnimalType animalType;
	PlayerColor color;
public:
	PlayerSelector(AnimalType animalType = FOX, PlayerColor color = RED, DirectX::SimpleMath::Vector2 position = DirectX::SimpleMath::Vector2::Zero);
	virtual ~PlayerSelector();

	bool render(bool selected);

	AnimalType getAnimalType() const;
	PlayerColor getPlayerColor() const;
	void changeAnimalType(bool dir);
	void changePlayerColor(bool dir);
};

#endif // !PLAYER_SELECTOR_H
