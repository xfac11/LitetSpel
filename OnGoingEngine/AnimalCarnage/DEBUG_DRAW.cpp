#include "DEBUG_DRAW.h"
#include "System.h"
DEBUG_DRAW::DEBUG_DRAW()
{

	shader = new SimpleShader();
	if (shader->initialize())
		return;
}


DEBUG_DRAW::~DEBUG_DRAW()
{
	for (int i = 0; i < shapes.size(); i++)
	{
		Primitives* temp = this->shapes[i];
		delete temp;
	}
	shapes.clear();
	delete shader;
}

bool DEBUG_DRAW::addPrimitives(Primitives * shape)
{
	Primitives *temp;
	temp = shape;
	shapes.push_back(temp);
	return true;
}

void DEBUG_DRAW::Draw(XMMATRIX view,XMMATRIX proj)
{
	XMFLOAT4 p(0,0,0,0);
	this->shader->setCBuffers();
	this->shader->setShaders();
	this->shader->setViewProj(view, proj,p);
	for (int i = 0; i < shapes.size(); i++)
	{
		this->shapes[i]->draw(this->shader);
	}
}
