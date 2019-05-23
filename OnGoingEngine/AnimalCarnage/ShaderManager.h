#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "ForwardShader.h"
#include "DeferredShader.h"
#include "LightShader.h"
#include "VerticalBlur.h"
#include "HorizontalBlur.h"
#include "ShadowMapping.h"
#include "ParticleShader.h"

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
	ParticleShader*& getParticleShader();
	ShadowMapping*& getShadowMapping();
	HorizontalBlur*& getHorBlur();
	VerticalBlur*& getVerBlur();

private:
	ForwardShader* fShader;
	DeferredShader* dShader;
	LightShader* lShader;
	ParticleShader* pShader;
	ShadowMapping* shadowMapping;
	HorizontalBlur* horBlur;
	VerticalBlur* verBlur;
};
#endif // !SHADERMANAGER_H
