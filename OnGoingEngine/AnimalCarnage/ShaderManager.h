#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#include"ForwardShader.h"
#include"DeferredShader.h"
#include"LightShader.h"
#include"VerticalBlur.h"
#include"HorizontalBlur.h"
#include"ShadowMapping.h"
class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();
	void initialize(int height, int width, float nearPlane, float farPlane);
	//void addShader(ForwardShader *& shader);
	ForwardShader*& getForwardShader();
	DeferredShader*& getDefShader();
	LightShader*& getLightShader();
	ShadowMapping*& getShadowMapping();
	HorizontalBlur*& getHorBlur();
	VerticalBlur*& getVerBlur();
private:
	ForwardShader* fShader;
	DeferredShader* dShader;
	LightShader* lShader;
	ShadowMapping* shadowMapping;
	HorizontalBlur* horBlur;
	VerticalBlur* verBlur;
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
