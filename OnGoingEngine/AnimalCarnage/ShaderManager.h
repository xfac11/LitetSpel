#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#include"ForwardShader.h"
class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();
	void initialize();
	//void addShader(ForwardShader *& shader);
	ForwardShader*& getForwardShader();
private:
	ForwardShader* fShader;

	//Shader* *shaders;
	//int cap;
	//int nrOfShaders;

	//ForwardShader* fShader;
	//first
	//DeferredShader *dShader;
	//second
	//LightShader *lShader;
};
#endif // !SHADERMANAGER_H
