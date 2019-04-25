#include "Slider.h"

Slider::Slider(std::string sliderBG, std::string slider, int startValue, int maxValue, int stepSize, DirectX::SimpleMath::Vector2 position)
	: GuiElement(position), value(startValue), maxValue(maxValue), stepSize(stepSize)
{
	this->slider.setTexture(slider);
	this->sliderBG.setTexture(sliderBG);
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
	return this->slider.getTexture();
}

ID3D11ShaderResourceView * Slider::getTextureBG()
{
	return this->sliderBG.getTexture();
}
