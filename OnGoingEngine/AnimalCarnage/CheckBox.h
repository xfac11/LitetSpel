#ifndef CHECK_BOX_H
#define CHECK_BOX_H

#include "Texture.h"
#include <string>
#include "GuiElement.h"

class CheckBox : public GuiElement
{
private:
	bool checked;
	Texture texture;
	Texture checkTexture;

public:
	CheckBox(std::string texture, std::string checkTexture, bool checked, DirectX::SimpleMath::Vector2 position = DirectX::SimpleMath::Vector2::Zero);
	virtual ~CheckBox();

	void setChecked(bool isChecked);
	bool isChecked() const;

	ID3D11ShaderResourceView* getTexture();
	ID3D11ShaderResourceView* getCheckTexture();
};

#endif // !CHECK_BOX_H
