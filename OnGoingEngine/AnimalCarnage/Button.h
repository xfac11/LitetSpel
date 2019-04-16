#ifndef BUTTON_H
#define BUTTON_H

#include "Texture.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

class Button
{
private:
	std::string text;
	Texture texture;
	Vector2 position;
	Vector2 size;

public:
	Button(std::string texture, std::string text, Vector2 position = Vector2::Zero, Vector2 size = Vector2(100, 100));
	~Button();

	ID3D11ShaderResourceView* getTexture();
	Vector2 getPosition() const;
	Vector2 getSize() const;
	std::string getText() const;
	DirectX::SimpleMath::Rectangle getRect() const;
};

#endif // !BUTTON_H
