#ifndef SLIDER_H
#define SLIDER_H

#include "GuiElement.h"
#include "Texture.h"

class Slider : public GuiElement
{
private:
	int value;
	int maxValue;
	int stepSize;
	Texture sliderBG;
	Texture slider;

public:
	Slider(std::string sliderBG, std::string slider, int startValue, int maxValue, int stepSize, DirectX::SimpleMath::Vector2 position = DirectX::SimpleMath::Vector2::Zero);
	virtual ~Slider();

	void changeValueWithStep(bool remove);
	void setValue(int value);

	int getValue() const;
	int getMaxValue() const;

	ID3D11ShaderResourceView* getTexture();
	ID3D11ShaderResourceView* getTextureBG();
};

#endif // !SLIDER_H
