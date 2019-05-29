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
	DirectX::XMVECTOR readyColor;
	bool ready;
public:
	PlayerSelector(AnimalType animalType = FOX, PlayerColor color = RED, DirectX::SimpleMath::Vector2 position = DirectX::SimpleMath::Vector2::Zero);
	virtual ~PlayerSelector();

	bool render(bool selected, DirectX::XMVECTOR color = DirectX::XMVectorSet(1, 0, 0, 1));
	bool getReady();
	AnimalType getAnimalType() const;
	PlayerColor getPlayerColor() const;
	void setReady(bool arg);
	void changeAnimalType(bool dir);
	void changePlayerColor(bool dir);
};

#endif // !PLAYER_SELECTOR_H
