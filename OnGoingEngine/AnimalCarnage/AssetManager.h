#ifndef ASSETMANGER_H
#define ASSETMANGER_H
#include <iostream>
#include <map>
#include <d3d11.h>
#include <wrl/client.h>
#include "Texture.h"
class  AssetManager
{
public:
	 AssetManager(){}
	 ~AssetManager() {}
	void LoadTexture(std::string name, std::string fileName);
	Texture* GetTexture(std::string name);

	//void LoadModel(std::string name, std::string fileName);
	//ID3D11ShaderResourceView* GetModel(std::string name);
private:
	//texture
	std::map<std::string, Texture*> _textures;
//	std::map<std::string, ModelLoader*> _model;
};

#endif // !Assetmanger
