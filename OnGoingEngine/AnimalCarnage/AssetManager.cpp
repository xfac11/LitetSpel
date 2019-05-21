#include "AssetManager.h"
#include "System.h"
AssetManager::~AssetManager()
{


	_model.clear();
	_textures.clear();

}
void AssetManager::LoadTexture(std::string name, std::string fileName)
{

	//there are already one with that texture
	if (_textures.count(name))
		return;

	shared_ptr<Texture>  texture(new Texture());
	texture->setTexture(fileName);
	this->_textures[name] = texture;

}

shared_ptr<Texture>  AssetManager::GetTexture(std::string name)
{
	return this->_textures.at(name);
}

void AssetManager::LoadModel(std::string name,
	shared_ptr<Model> m)
{
	if (_model.count(name))
		return;
	this->_model[name] = m;
}

shared_ptr<Model> AssetManager::GetModel(std::string name)
{
	if (!_model.count(name))
	{
		return NULL;
	}
	else
		return  this->_model.at(name);
}

bool AssetManager::CheckIfExist(std::string& name)
{
	/*for (int i = 0; i < models.size(); i++)
	{
		if (models[i]->filePath == name)
		{
			return true;
		}
	}*/
	return false;
}
