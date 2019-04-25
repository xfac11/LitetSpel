#include "Slider.h"

bool Slider::texturesLoaded = false;
Texture Slider::sliderBG = Texture();
Texture Slider::slider = Texture();

Slider::Slider(int startValue, int maxValue, int stepSize, DirectX::SimpleMath::Vector2 position)
	: GuiElement(position), value(startValue), maxValue(maxValue), stepSize(stepSize)
{
	if (!Slider::texturesLoaded)
	{
		Slider::slider.setTexture("slider.tga");
		Slider::sliderBG.setTexture("sliderBG.tga");
		Slider::texturesLoaded = true;
	}
}

Slider::~Slider()
{
}

void Slider::changeValueWithStep(bool remove)
{
	this->value += remove ? -stepSize : stepSize;

	if (this->value < 0)
	{
		this->value = 0;
	}
	else if (this->value > this->maxValue)
	{
		this->value = this->maxValue;
	}
}

void Slider::setValue(int value)
{
	this->value = value;

	if (this->value < 0)
	{
		this->value = 0;
	}
	else if (this->value > this->maxValue)
	{
		this->value = this->maxValue;
	}
}

int Slider::getValue() const
{
	return this->value;
}

int Slider::getMaxValue() const
{
	return this->maxValue;
}

ID3D11ShaderResourceView * Slider::getTexture()
{
	return Slider::slider.getTexture();
}

ID3D11ShaderResourceView * Slider::getTextureBG()
{
	return Slider::sliderBG.getTexture();
}
