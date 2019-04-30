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

	int health;
	int maxHealth;

public:
	HealthBar(int health, int maxHealth, DirectX::SimpleMath::Vector2 position = DirectX::SimpleMath::Vector2::Zero);
	virtual ~HealthBar();

	bool render(bool selected);

	void setHealth(int health);
	int getHealth() const;
	void setMaxHealth(int maxHealth);
	int getMaxHealth() const;
};

#endif // !HEALTH_BAR_H
