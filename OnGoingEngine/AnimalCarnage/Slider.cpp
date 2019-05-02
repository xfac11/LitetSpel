#include "Slider.h"
#include "System.h"

bool Slider::texturesLoaded = false;
Texture Slider::sliderBG = Texture();
Texture Slider::slider = Texture();
Texture Slider::sliderButton = Texture();

Slider::Slider(int startValue, int maxValue, int stepSize, DirectX::SimpleMath::Vector2 position)
	: GuiElement(position), value(startValue), maxValue(maxValue), stepSize(stepSize)
{
	if (!Slider::texturesLoaded)
	{
		Slider::slider.setTexture("slider.tga");
		Slider::sliderButton.setTexture("sliderButton.tga");
		Slider::sliderBG.setTexture("sliderBG.tga");
		Slider::texturesLoaded = true;
	}
}

Slider::~Slider()
{

}

bool Slider::render(bool selected)
{
	System::getSpriteBatch()->Draw(Slider::sliderBG.getTexture(), this->position, nullptr, DirectX::Colors::White);
	System::getSpriteBatch()->Draw(Slider::slider.getTexture(), DirectX::SimpleMath::Rectangle(static_cast<long>(this->position.x + 5), static_cast<long>(this->position.y + 5), (this->value / static_cast<float>(this->maxValue)) * 600, 25), DirectX::Colors::White);
	System::getSpriteBatch()->Draw(Slider::sliderButton.getTexture(), this->position + DirectX::SimpleMath::Vector2(-20.0F + (this->value / static_cast<float>(this->maxValue)) * 600, -5.0F), nullptr, DirectX::Colors::White);

	return true;
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
