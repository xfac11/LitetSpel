#ifndef LOADER_H
#define LOADER_H
#include <iostream>
#include <vector>
#include <fstream>
#include <d3d11.h>
#include <directxmath.h>
#include <direct.h>
#include <string>
#include "Texture.h"
#include "Vertex3D.h"
class Loader
{
public:
	Loader();
	~Loader();
	std::string loadFile(std::string file, std::vector<Vertex3D> &outVec);

private:


};
#endif // !LOADER_H