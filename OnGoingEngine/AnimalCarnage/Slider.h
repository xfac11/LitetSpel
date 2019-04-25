#ifndef SLIDER_H
#define SLIDER_H

#include "GuiElement.h"
#include "Texture.h"

class Slider : public GuiElement
{
private:
	static bool texturesLoaded;
	static Texture sliderBG;
	static Texture slider;

	int value;
	int maxValue;
	int stepSize;

public:
	Slider(int startValue, int maxValue, int stepSize, DirectX::SimpleMath::Vector2 position = DirectX::SimpleMath::Vector2::Zero);
	virtual ~Slider();

	void changeValueWithStep(bool remove);
	void setValue(int value);

	int getValue() const;
	int getMaxValue() const;

	static ID3D11ShaderResourceView* getTexture();
	static ID3D11ShaderResourceView* getTextureBG();
};

#endif // !SLIDER_H
