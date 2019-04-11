#include "Texture.h"
#include"System.h"
Texture::Texture()
{

}

Texture::~Texture()
{
}

void Texture::setTexture(std::string fileName)
{
	testTexture.Initialize(fileName.c_str());
}

ID3D11ShaderResourceView *& Texture::getTexture()
{
	return this->textureView;
	// TODO: insert return statement here
}

unsigned char * Texture::getTextureCharArray()
{
	return nullptr;
}

int Texture::getWidth()
{
	return 0;
}

int Texture::getHeight()
{
	return 0;
}

void Texture::cleanUp()
{
}
