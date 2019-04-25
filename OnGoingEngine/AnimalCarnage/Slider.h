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
	static Texture sliderButton;

	int value;
	int maxValue;
	int stepSize;

public:
	Slider(int startValue, int maxValue, int stepSize, DirectX::SimpleMath::Vector2 position = DirectX::SimpleMath::Vector2::Zero);
	virtual ~Slider();

	bool render(bool selected);

	void changeValueWithStep(bool remove);
	void setValue(int value);

	int getValue() const;
	int getMaxValue() const;
};

#endif // !SLIDER_H
