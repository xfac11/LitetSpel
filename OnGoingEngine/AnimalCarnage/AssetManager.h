#ifndef ASSETMANGER_H
#define ASSETMANGER_H
#include <iostream>
#include <map>
#include <d3d11.h>
#include <wrl/client.h>
#include "Texture.h"
#include "GameObject.h"
#include <memory>
#include <iostream>
using namespace std;
class  AssetManager
{
public:
	AssetManager() {}
	~AssetManager();
	void LoadTexture(std::string name, std::string fileName);
	shared_ptr<Texture>  GetTexture(std::string name);

	void LoadModel(std::string name, shared_ptr<Model>  m);

	shared_ptr<Model> GetModel(std::string name);
private:
	//
	bool CheckIfExist(std::string& name);

	//texture
	std::map<std::string, shared_ptr<Texture>> _textures;
	std::map<std::string, shared_ptr<Model>> _model;

};

#endif // !Assetmanger
