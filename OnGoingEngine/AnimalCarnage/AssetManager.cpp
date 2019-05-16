#include "AssetManager.h"

void AssetManager::LoadTexture(std::string name, std::string fileName)
{

	//there are already one with that texture
	if (_textures.count(name))
		return;

	Texture * texture;
	texture->setTexture(fileName);
	this->_textures[name] = texture;

}

Texture * AssetManager::GetTexture(std::string name)
{
	return this->_textures.at(name);
}
