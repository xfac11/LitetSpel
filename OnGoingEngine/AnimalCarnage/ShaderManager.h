#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#include"ForwardShader.h"
class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();
private:
	ForwardShader *fShader;

	//first
	//DeferredShader *dShader;
	//second
	//LightShader *lShader;
};
#endif // !SHADERMANAGER_H
