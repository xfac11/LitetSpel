#include"Loader.h"

Loader::Loader()
{
}

Loader::~Loader()
{
}

std::string Loader::loadFile(std::string file, std::vector<Vertex3D>& outVec) //directX::XMFLOAT3
{
	//"r" for reading
	std::FILE *infile;
	
	std::string fileTemp = "OBJ/" + file;
	errno_t err;
	err = fopen_s(&infile, fileTemp.c_str(), "r");
	char firstWord[128];
	char materialFile[256];
	char materialToUse[256];
	std::vector<int> vecIndices;
	std::vector<int> norIndices;
	std::vector<int> texIndices;
	std::vector<DirectX::XMFLOAT3> vectors;
	std::vector<DirectX::XMFLOAT3> normals;
	std::vector<DirectX::XMFLOAT2> texCoord;
	if (err != 0)
	{
		//Can't open the file
		fileTemp += "\n";
		OutputDebugStringA(fileTemp.c_str());
		return false;
	}
	//%s == string?
	int res = 0;
	res = fscanf_s(infile, "%s", firstWord, _countof(firstWord));
	while (res != EOF)
	{
		if (strcmp(firstWord, "v") == 0)
		{
			DirectX::XMFLOAT3 vec;
			//vertices 3 floats 
			fscanf_s(infile, "%f %f %f\n", &vec.x, &vec.y, &vec.z);
			//Convert from RH to LF
			vec.z = vec.z*-1.0f;
			vectors.push_back(vec);
		}
		else if (strcmp(firstWord, "vn") == 0)
		{
			DirectX::XMFLOAT3 vecNorm;
			fscanf_s(infile, "%f %f %f\n", &vecNorm.x, &vecNorm.y, &vecNorm.z);
			//Convert from RG to LF
			vecNorm.z = vecNorm.z*-1.0f;
			normals.push_back(vecNorm);
		}
		else if (strcmp(firstWord, "vt") == 0)
		{
			DirectX::XMFLOAT2 tex;
			fscanf_s(infile, "%f %f\n", &tex.x, &tex.y);
			//Convert
			tex.y = 1.0f - tex.y;
			texCoord.push_back(tex);
		}
		else if (strcmp(firstWord, "f") == 0)
		{
			int verIn[4];
			int norIn[4];
			int texIn[4];
			fscanf_s(infile, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &verIn[0], &texIn[0], &norIn[0],
				&verIn[1], &texIn[1], &norIn[1], &verIn[2], &texIn[2], &norIn[2],
				&verIn[3], &texIn[3], &norIn[3]);
			//Convert drawing order
			if (norIn[3] < -100000)
			{
				Vertex3D temp;
				for (int i = 2; i > -1; i--)
				{
					temp.x = vectors.at(verIn[i] - 1).x;
					temp.y = vectors.at(verIn[i] - 1).y;
					temp.z = vectors.at(verIn[i] - 1).z;
					temp.u = texCoord.at(texIn[i] - 1).x;
					temp.v = texCoord.at(texIn[i] - 1).y;
					temp.nx = normals.at(norIn[i] - 1).x;
					temp.ny = normals.at(norIn[i] - 1).y;
					temp.nz = normals.at(norIn[i] - 1).z;
					outVec.push_back(temp);
				}
			}
			else
			{
				Vertex3D temp;
				for (int i = 3; i > 0; i--)
				{
					temp.x = vectors.at(verIn[i] - 1).x;
					temp.y = vectors.at(verIn[i] - 1).y;
					temp.z = vectors.at(verIn[i] - 1).z;
					temp.u = texCoord.at(texIn[i] - 1).x;
					temp.v = texCoord.at(texIn[i] - 1).y;
					temp.nx = normals.at(norIn[i] - 1).x;
					temp.ny = normals.at(norIn[i] - 1).y;
					temp.nz = normals.at(norIn[i] - 1).z;
					outVec.push_back(temp);
				}
				temp.x = vectors.at(verIn[3] - 1).x;
				temp.y = vectors.at(verIn[3] - 1).y;
				temp.z = vectors.at(verIn[3] - 1).z;
				temp.u = texCoord.at(texIn[3] - 1).x;
				temp.v = texCoord.at(texIn[3] - 1).y;
				temp.nx = normals.at(norIn[3] - 1).x;
				temp.ny = normals.at(norIn[3] - 1).y;
				temp.nz = normals.at(norIn[3] - 1).z;
				outVec.push_back(temp);
				for (int i = 1; i > -1; i--)
				{
					temp.x = vectors.at(verIn[i] - 1).x;
					temp.y = vectors.at(verIn[i] - 1).y;
					temp.z = vectors.at(verIn[i] - 1).z;
					temp.u = texCoord.at(texIn[i] - 1).x;
					temp.v = texCoord.at(texIn[i] - 1).y;
					temp.nx = normals.at(norIn[i] - 1).x;
					temp.ny = normals.at(norIn[i] - 1).y;
					temp.nz = normals.at(norIn[i] - 1).z;
					outVec.push_back(temp);
				}
			}

		}
		else if (strcmp(firstWord, "mtllib") == 0)
		{
			//texture material
			fscanf_s(infile, "%s\n", materialFile, _countof(materialFile));

		}
		else if (strcmp(firstWord, "usemtl") == 0)
		{
			fscanf_s(infile, "%s\n", materialToUse, _countof(materialToUse));
		}
		res = fscanf_s(infile, "%s", firstWord, _countof(firstWord));
	}
	//insert the data into the outVec


	// For each vertex of each triangle


	std::FILE *mtlFile;
	std::string mtlFileTemp = materialFile;
	std::string objfile = "OBJ/" + mtlFileTemp;
	err = fopen_s(&mtlFile, objfile.c_str(), "r");	
	if (err != 0)
	{
		return std::string();
	}
	char materialTest[256];
	char materialTexture[256];
	bool foundTexture = false;
	res = fscanf_s(mtlFile, "%s %s", firstWord, _countof(firstWord), materialTest, _countof(materialTest));
	while (res != EOF && !foundTexture)
	{
		if (strcmp(firstWord, "newmtl") == 0)
		{
			if (strcmp(materialTest, materialToUse))
			{
				res = fscanf_s(mtlFile, "%s", firstWord, _countof(firstWord));
				while (res != EOF && !foundTexture)
				{
					if (strcmp(firstWord, "map_Kd") == 0)
					{
						fscanf_s(mtlFile, "%s\n", materialTexture, _countof(materialTexture));
						foundTexture = true;
					}
					res = fscanf_s(mtlFile, "%s", firstWord, _countof(firstWord));
				}
			}
		}
		res = fscanf_s(mtlFile, "%s %s", firstWord, _countof(firstWord), materialTest, _countof(materialTest));
	}
	//texture.setTexture(device, deviceContext, materialTexture);
	return materialTexture;
}