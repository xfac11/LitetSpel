#ifndef MODELLOADER_H
#define MODELLOADER_H
#include <fstream>
#include "Model.h"
#include "GameObject.h"
#include "Luna/Luna.h"
#include "Luna/Reader.h"
class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();

	void loadGO(GameObject*& object, const char* filePath);
	//void loadModel(Model **&model, const char* filePath);
	std::string getPath() const;

private:
	std::string path = "Resources/Models"; 
};

#endif // !MODELLOADER_H