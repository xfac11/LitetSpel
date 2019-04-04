#include "Scene.h"

Scene::Scene() {
	this->modelCount = 0;
	this->animatedModelCount = 0;
	this->blendMapModelCount = 0;
	this->lightCount = 0;
}


Scene::~Scene() {
	OutputDebugStringA("Destructor is run for scene\n");
	for (int i = 0; i < this->animatedModelCount; i++) {
		delete animatedModels[i];
	}
	animatedModels.clear();
	for (int i = 0; i < this->modelCount; i++) {
		delete models[i];
	}
	models.clear();
	for (int i = 0; i < this->blendMapModelCount; i++) {
		delete blendmapModels[i];
	}
	blendmapModels.clear();
}

void Scene::addModel(const char* path) {
	models.push_back(new RawModel (path));
	loader.loadOBJ(models[this->modelCount]); //Loads model from file
	loader.loadMTL(models[this->modelCount]); //Loads material from file
	models[this->modelCount]->prepareBuffers();
	this->modelCount++;
}

void Scene::addBlendmapModel(const char* path) {
	blendmapModels.push_back(new RawModel (path));
	loader.loadOBJ(blendmapModels[this->blendMapModelCount]); //Loads model from file
	loader.loadMTL(blendmapModels[this->blendMapModelCount]); //Loads material from file
	blendmapModels[this->blendMapModelCount]->prepareBuffers();
	this->blendMapModelCount++;
}

void Scene::addAnimatedModel(std::string path) {
	animatedModels.push_back(new AnimatedModel(path));
	animLoader.LoadModel(animatedModels[this->animatedModelCount]);//Load the model
	animatedModels[animatedModelCount]->prepareBuffers();
	this->animatedModelCount++;
}

int Scene::getModelCount() const {
	return this->modelCount;
}

unsigned int Scene::getAnimModelCount() const {
	return this->animatedModelCount;
}

int Scene::getBlendmapModelCount() const {
	return this->blendMapModelCount;
}

void Scene::addLight(glm::vec3 position, glm::vec3 color) {
	Light light(position, color); //Initialize with position and color
	lights.push_back(light);
	lightPositions.push_back(position);
	lightColors.push_back(color);
	this->lightCount++;
}

int Scene::getLightCount() const {
	return this->lightCount;
}
