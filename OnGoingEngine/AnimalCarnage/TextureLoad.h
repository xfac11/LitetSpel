#ifndef TEXTURELOAD_H
#define TEXTURELOAD_H
#include <d3d11.h>
#include <stdio.h>
#include <fstream>

class TextureLoad
{
private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

	bool LoadTarga(const char*, int&, int&);

	unsigned char* m_targaData;
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView;

	//TO STORE WIDTH & HEIGHT
	int width;
	int height;

	bool transparent;
public:
	TextureLoad();
	TextureLoad(const TextureLoad&);
	~TextureLoad();

	bool Initialize(const char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	bool isTransparent();

	unsigned char* getTextureCharArray();
	unsigned short getWidth();
	unsigned short getHeight();
};

#endif