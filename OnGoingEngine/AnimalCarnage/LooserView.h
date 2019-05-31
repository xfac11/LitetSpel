#ifndef LOOSER_VIEW_H
#define LOOSER_VIEW_H

#include "GuiElement.h"
#include "Texture.h"
#include "Player.h"

class LooserView : public GuiElement
{
protected:
	static bool texturesLoaded;
	static Texture texture;
	static Texture players[4];
	static Texture backGroundLayer;

	PlayerStats stats;
public:
	LooserView(DirectX::SimpleMath::Vector2 position = DirectX::SimpleMath::Vector2());
	virtual ~LooserView();

	virtual bool render(bool selected);

	void setStats(PlayerStats stats);
};

#endif // !LOOSER_VIEW_H
