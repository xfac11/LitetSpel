#ifndef MODELLOADER_H
#define MODELLOADER_H
#include <fstream>
#include "Model.h"
#include "Luna.h"
#include "Reader.h"
class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();

	void loadModel(Model &model, const char* filePath);

private:

};

#endif // !MODELLOADER_H