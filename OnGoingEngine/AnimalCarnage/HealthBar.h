#ifndef HEALTH_BAR_H
#define HEALTH_BAR_H

#include "GuiElement.h"
#include "Texture.h"

class HealthBar : public GuiElement
{
private:
	static bool texturesLoaded;
	static Texture texture;
	static Texture textureOutline;
	static Texture textureBackground;
	int health;
	int maxHealth;
	DirectX::XMVECTOR color;
public:
	HealthBar(int health, int maxHealth, DirectX::SimpleMath::Vector2 position = DirectX::SimpleMath::Vector2::Zero);
	virtual ~HealthBar();
	const DirectX::XMVECTOR& getColor();
	bool render(bool selected);
	void setColor(float r, float g, float b);
	void setHealth(int health);
	int getHealth() const;
	void setMaxHealth(int maxHealth);
	int getMaxHealth() const;
};

#endif // !HEALTH_BAR_H
