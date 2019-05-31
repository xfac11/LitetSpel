#ifndef WINNER_VIEW_H
#define WINNER_VIEW_H

#include "LooserView.h"
#include "Texture.h"

class WinnerView : public LooserView
{
private:
	static bool texturesLoaded2;
	static Texture texture2;

public:
	WinnerView(DirectX::SimpleMath::Vector2 position = DirectX::SimpleMath::Vector2());
	virtual ~WinnerView();

	bool render(bool selected);
};

#endif // !WINNER_VIEW_H
