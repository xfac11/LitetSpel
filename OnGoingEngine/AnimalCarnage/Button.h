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

	std::string text;
	Vector2 size;

public:
	Button(std::string text, Vector2 position = Vector2::Zero, Vector2 size = Vector2(100, 100));
	~Button();

	//bool update(float deltaTime);
	//bool render(DirectX::SpriteBatch* spriteBatch);

	Vector2 getSize() const;
	std::string getText() const;
	DirectX::SimpleMath::Rectangle getRect() const;

	static ID3D11ShaderResourceView* getTexture();
};

#endif // !BUTTON_H
