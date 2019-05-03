#ifndef TEXTURE_H
#define TEXTURE_H
#include <d3d11.h>
//#pragma comment(lib, "d3d11.lib")
#include <directxmath.h>
//#include "bth_image.h"
#include "textureload.h"
//#include "TextureData.h"
class Texture
{
public:
	Texture();
	~Texture();
	//void setTexture2(ID3D11Device*& gDevice, ID3D11DeviceContext *& gDeviceContext);
	void setTexture(std::string fileName);
	//void changeTextureData(TextureData* obj);
	ID3D11ShaderResourceView*& getTexture();
	unsigned char* getTextureCharArray();
	int getWidth();
	int getHeight();
	bool isTransparent();
private:
	//ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* textureView;
	//ID3D11Resource* texture;
	//ID3D11CreateTextureFromFile* texureTest;
	//TextureData* targaData;
	//unsigned char* DATA;
	TextureLoad testTexture;
	void cleanUp();

};
#endif // !TEXTURE_H
