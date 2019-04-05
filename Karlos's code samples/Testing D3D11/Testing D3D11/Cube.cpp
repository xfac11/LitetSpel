#include"Cube.h"

void Cube::setPosition(DirectX::XMFLOAT3 newPos)
{

}

void Cube::move(DirectX::XMFLOAT3 dir)
{
	for (int i = 0; i < 6; i++)
	{
		this->cube[i].move(dir.x, dir.y, dir.z);
	}
}

void Cube::rotateX(float degree)
{
	for (int i = 0; i < 6; i++)
	{
		this->cube[i].setRotationX(degree);
	}
}

void Cube::rotateY(float degree)
{
	for (int i = 0; i < 6; i++)
	{
		this->cube[i].setRotationY(degree);
	}
}

void Cube::rotateZ(float degree)
{
	for (int i = 0; i < 6; i++)
	{
		this->cube[i].setRotationZ(degree);
	}
}

QuadVertex Cube::getQuad(int id)
{
	return this->cube[id];
}

Cube::Cube()
{
	//this->cube[1] = nullptr;
}
Cube::Cube(DirectX::XMFLOAT3 pos, float width, float height, float depth)
{
	this->cube[0] = QuadVertex(pos, width, 0, depth, 1);
	this->cube[1] = QuadVertex(pos, 0, height, depth, 1);
	this->cube[2] = QuadVertex(pos, width, height, 0, 0);


	DirectX::XMFLOAT3 temp = pos;

	temp.y += height;
	this->cube[3] = QuadVertex(temp, width, 0, depth, 0);
	temp.y -= height;
	temp.x += width;
	this->cube[4] = QuadVertex(temp, 0, height, depth, 0);
	temp.x -= width;
	temp.z += depth;
	this->cube[5] = QuadVertex(temp, width, height, 0, 1);
}

Cube::~Cube()
{
}