#ifndef CHECK_BOX_H
#define CHECK_BOX_H

#include "Texture.h"
#include <string>
#include "GuiElement.h"

class CheckBox : public GuiElement
{
private:
	static bool texturesLoaded;
	static Texture texture;
	static Texture checkTexture;

	bool checked;

public:
	CheckBox(bool checked, DirectX::SimpleMath::Vector2 position = DirectX::SimpleMath::Vector2::Zero);
	virtual ~CheckBox();

	void setChecked(bool isChecked);
	bool isChecked() const;

	static ID3D11ShaderResourceView* getTexture();
	static ID3D11ShaderResourceView* getCheckTexture();
};

#endif // !CHECK_BOX_H
