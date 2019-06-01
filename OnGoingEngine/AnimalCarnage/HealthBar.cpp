#include "HealthBar.h"
#include "System.h"

bool HealthBar::texturesLoaded = false;
Texture* HealthBar::texture = nullptr;
Texture* HealthBar::textureOutline = nullptr;
Texture* HealthBar::textureBackground = nullptr;
Texture* HealthBar::textureFox = nullptr;
Texture* HealthBar::textureRabbit = nullptr;
Texture* HealthBar::textureMoose = nullptr;
Texture* HealthBar::textureBear = nullptr;
Texture* HealthBar::textureCrown = nullptr;
Texture* HealthBar::foxPortait = nullptr;
Texture* HealthBar::bearPortrait = nullptr;
Texture* HealthBar::rabbitPortrait = nullptr;
Texture* HealthBar::moosePortrait = nullptr;

HealthBar::HealthBar(int health, int maxHealth, DirectX::SimpleMath::Vector2 position) : GuiElement(position), health(health), maxHealth(maxHealth)
{
	if (!HealthBar::texturesLoaded)
	{
		HealthBar::texture = new Texture;
		HealthBar::textureOutline = new Texture;
		HealthBar::textureBackground = new Texture;
		HealthBar::textureFox = new Texture;
		HealthBar::textureRabbit = new Texture;
		HealthBar::textureCrown = new Texture;
		HealthBar::textureMoose = new Texture;
		HealthBar::textureBear = new Texture;
		HealthBar::bearPortrait = new Texture;
		HealthBar::foxPortait = new Texture;
		HealthBar::moosePortrait = new Texture;
		HealthBar::rabbitPortrait = new Texture;
		HealthBar::texture->setTexture("healthBar2.tga");
		HealthBar::textureOutline->setTexture("healthBarOutline2.tga");
		HealthBar::textureBackground->setTexture("healthBarBackground.tga");
		HealthBar::textureFox->setTexture("paw_fox.tga");
		HealthBar::textureRabbit->setTexture("paw_rabbit.tga");
		HealthBar::textureBear->setTexture("paw_bear.tga");
		HealthBar::textureMoose->setTexture("paw_moose.tga");
		HealthBar::textureCrown->setTexture("paw_crown.tga");
		HealthBar::bearPortrait->setTexture("paw_bear.tga");
		HealthBar::foxPortait->setTexture("playerFox.tga");
		HealthBar::moosePortrait->setTexture("paw_moose.tga");
		HealthBar::rabbitPortrait->setTexture("playerRabbit.tga");
		HealthBar::texturesLoaded = true;
	}
	this->color = DirectX::XMVectorSet(0, 1, 0, 1);
}

HealthBar::~HealthBar()
{
	if (HealthBar::texturesLoaded)
	{
		delete HealthBar::texture;
		delete HealthBar::textureOutline;
		delete HealthBar::textureBackground;
		delete HealthBar::textureFox;
		delete HealthBar::textureRabbit;
		delete HealthBar::textureMoose;
		delete HealthBar::textureBear;
		delete HealthBar::textureCrown;
		delete HealthBar::bearPortrait;
		delete HealthBar::foxPortait;
		delete HealthBar::moosePortrait;
		delete HealthBar::rabbitPortrait;
		HealthBar::texturesLoaded = false;
	}
}

const DirectX::XMVECTOR & HealthBar::getColor()
{
	return this->color;
}

bool HealthBar::render(bool selected)
{
	RECT source; source.top = 0; source.left = 0; source.bottom = HealthBar::textureOutline->getHeight();
	float colorValue = this->health / (this->maxHealth / 2.0F);
	if (colorValue > 1.0F) colorValue = 1.0F;

	source.right = static_cast<long>((this->health / static_cast<float>(this->maxHealth)) * HealthBar::textureOutline->getWidth());
	DirectX::SimpleMath::Color color(DirectX::SimpleMath::Color::Lerp(DirectX::SimpleMath::Color(1, 0, 0),DirectX::SimpleMath::Color(0, 1, 0), colorValue));
	//color.w = 0.9f;
	float scale = 1.5f;
	float scalePaw = 0.3f;
	ID3D11ShaderResourceView* test = nullptr;
	System::getSpriteBatch()->Draw(HealthBar::textureBackground->getTexture(), this->position+ DirectX::SimpleMath::Vector2(5, 0), nullptr, this->color, 0, SimpleMath::Vector2(0, 0), SimpleMath::Vector2(scale, scale));
	System::getSpriteBatch()->Draw(this->currentTexture->getTexture(), this->position + DirectX::SimpleMath::Vector2(0, 0), nullptr,Colors::White, 0, SimpleMath::Vector2(0, 0), SimpleMath::Vector2(1, 1));

	System::getSpriteBatch()->Draw(HealthBar::textureOutline->getTexture(), this->position + DirectX::SimpleMath::Vector2(5, HealthBar::textureBackground->getHeight()*scale), &source, color, 0, SimpleMath::Vector2(0, 0), SimpleMath::Vector2(scale, scale));
	System::getSpriteBatch()->Draw(HealthBar::texture->getTexture(), this->position+DirectX::SimpleMath::Vector2(5, HealthBar::textureBackground->getHeight()*scale),nullptr, Colors::White, 0, SimpleMath::Vector2(0, 0), SimpleMath::Vector2(scale, scale));
	System::getSpriteBatch()->Draw(this->nextTexture->getTexture(), this->position + DirectX::SimpleMath::Vector2(5/*HealthBar::textureOutline->getWidth()-30*/,5),nullptr,Colors::White,0,SimpleMath::Vector2(0,0),SimpleMath::Vector2(scalePaw, scalePaw));

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

void HealthBar::setNextAnimal(int next)
{
	if (next == 0)
		this->nextTexture = this->textureFox;
	else if (next == 1)
		this->nextTexture = this->textureBear;
	else if (next == 2)
		this->nextTexture = this->textureRabbit;
	else if (next == 3)
		this->nextTexture = this->textureMoose;
	else if (next == -1)
		this->nextTexture = this->textureCrown;

}

void HealthBar::setCurrentAnimal(int current)
{
	if (current == 0)
		this->currentTexture = this->foxPortait;
	else if (current == 1)
		this->currentTexture = this->bearPortrait;
	else if (current == 2)
		this->currentTexture = this->rabbitPortrait;
	else if (current == 3)
		this->currentTexture = this->moosePortrait;
}


void HealthBar::setMaxHealth(int maxHealth)
{
	this->maxHealth = maxHealth;
}

int HealthBar::getMaxHealth() const
{
	return this->maxHealth;
}
