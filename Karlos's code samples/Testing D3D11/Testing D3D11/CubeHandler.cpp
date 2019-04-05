#include"CubeHandler.h"
CubeHandler::CubeHandler()
{
	this->cubeCap = 10;
	this->nrOfCubes = 0;
	this->cubes = new Cube[this->cubeCap];
}

CubeHandler::~CubeHandler()
{
	delete[] this->cubes;
}

void CubeHandler::addCube(DirectX::XMFLOAT3 pos, float width, float height, float depth)
{
	if (this->cubeCap == this->nrOfCubes)
	{
		this->expand();
	}
	this->cubes[this->nrOfCubes] = Cube(pos, width, height, depth);
	this->nrOfCubes++;
}

bool CubeHandler::insertVector(std::vector<Vertex3D> &model, int & vertexCount)
{
	int pos = 0;
	if (this->nrOfCubes * 6 * 6 == vertexCount)
	{
		for (int i = 0; i < this->nrOfCubes; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				for (int k = 0; k < 2; k++)
				{
					for (int l = 0; l < 3; l++)
					{
						model.at(pos) = this->cubes[i].getQuad(j).getTri(k).getPoint(l);
						pos++;
					}
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < this->nrOfCubes; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				for (int k = 0; k < 2; k++)
				{
					for (int l = 0; l < 3; l++)
					{
						model.push_back(this->cubes[i].getQuad(j).getTri(k).getPoint(l));
						vertexCount++;
					}
				}
			}
		}
	}
	return true;
}
void CubeHandler::moveCube(int id)
{
	this->cubes[id].move(DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f));
}
void CubeHandler::expand()
{
	this->cubeCap += 5;
	OutputDebugStringA("HALP");
	Cube* temp = new Cube[this->cubeCap];
	for (int i = 0; i < this->nrOfCubes; i++)
	{
		temp[i] = this->cubes[i];
	}
	delete[]this->cubes;
	this->cubes = temp;
}