#ifndef CUBEHANDLER_H
#define CUBEHANDLER_H
#include"Cube.h"
#include<vector>
class CubeHandler
{
public:
	CubeHandler();
	~CubeHandler();
	void addCube(DirectX::XMFLOAT3 pos, float width, float height, float depth);
	bool insertVector(std::vector<Vertex3D> &model, int & vertexCount);
	void moveCube(int id);
private:
	void expand();
	Cube *cubes;
	int cubeCap;
	int nrOfCubes;
};
#endif // !CUBEHANDLER_H