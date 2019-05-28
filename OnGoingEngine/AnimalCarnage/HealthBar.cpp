#include "HealthBar.h"
#include "System.h"

bool HealthBar::texturesLoaded = false;
Texture HealthBar::texture = Texture();
Texture HealthBar::textureOutline = Texture();
Texture HealthBar::textureBackground = Texture();
HealthBar::HealthBar(int health, int maxHealth, DirectX::SimpleMath::Vector2 position) : GuiElement(position), health(health), maxHealth(maxHealth)
{
	if (!HealthBar::texturesLoaded)
	{
		HealthBar::texture.setTexture("healthBar2.tga");
		HealthBar::textureOutline.setTexture("healthBarOutline2.tga");
		HealthBar::textureBackground.setTexture("healthBarBackground.tga");
		this->color = DirectX::XMVectorSet(0, 1, 0, 1);
		HealthBar::texturesLoaded = true;
	}
}

HealthBar::~HealthBar()
{
}

bool HealthBar::render(bool selected)
{
	RECT source; source.top = 0; source.left = 0; source.bottom = HealthBar::textureOutline.getHeight();
	float colorValue = this->health / (this->maxHealth / 2.0F);
	if (colorValue > 1.0F) colorValue = 1.0F;

	source.right = static_cast<long>((this->health / static_cast<float>(this->maxHealth)) * HealthBar::textureOutline.getWidth());
	DirectX::SimpleMath::Color color(DirectX::SimpleMath::Color::Lerp(DirectX::SimpleMath::Color(1, 0, 0),DirectX::SimpleMath::Color(0, 1, 0), colorValue));
	//color.w = 0.9f;
	ID3D11ShaderResourceView* test = nullptr;
	System::getSpriteBatch()->Draw(HealthBar::textureBackground.getTexture(), this->position+ DirectX::SimpleMath::Vector2(5, 0), this->color);
	System::getSpriteBatch()->Draw(HealthBar::textureOutline.getTexture(), this->position + DirectX::SimpleMath::Vector2(5, 0), &source, color);
	System::getSpriteBatch()->Draw(HealthBar::texture.getTexture(), this->position+DirectX::SimpleMath::Vector2(5, 0));
	
	
	return true;
}

void HealthBar::setColor(float r, float g, float b)
{
	this->color = DirectX::XMVectorSet(r, g, b, 1);
}

void HealthBar::setHealth(int health)
{
	this->health = health;

	if (this->health < 0)
	{
		this->health = 0;
	}
	else if (this->health > this->maxHealth)
	{
		this->health = this->maxHealth;
	}
}

int HealthBar::getHealth() const
{
	return this->health;
}

void HealthBar::setMaxHealth(int maxHealth)
{
	this->maxHealth = maxHealth;
}

int HealthBar::getMaxHealth() const
{
	return this->maxHealth;
}
