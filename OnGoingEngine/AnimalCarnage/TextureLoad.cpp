#include "TextureLoad.h"

TextureLoad::TextureLoad()
{
}

TextureLoad::TextureLoad(const TextureLoad &)
{
}

TextureLoad::~TextureLoad()
{
}

bool TextureLoad::Initialize(const char *)
{
	return false;
}

void TextureLoad::Shutdown()
{
}

ID3D11ShaderResourceView * TextureLoad::GetTexture()
{
	return nullptr;
}

unsigned char * TextureLoad::getTextureCharArray()
{
	return nullptr;
}

unsigned short TextureLoad::getWidth()
{
	return 0;
}

unsigned short TextureLoad::getHeight()
{
	return 0;
}
