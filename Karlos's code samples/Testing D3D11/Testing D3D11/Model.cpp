#include "Model.h"

Model::Model()
{
	this->vertexBuffer = nullptr;
	//this->indexBuffer = nullptr;
	//this->constantBuffer = nullptr;
	this->SamplerState = nullptr;
	//this->indexCount = 0;
	this->vertexCount = 0;

	this->rot = 0;
	this->moveM = 0;
	DirectX::XMVECTOR rotaxis = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMMATRIX rotTemp = DirectX::XMMatrixRotationAxis(rotaxis, 0);
	DirectX::XMMATRIX scaleTemp = DirectX::XMMatrixScaling(0.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX translTemp = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&this->Rotation, rotTemp);
	DirectX::XMStoreFloat4x4(&this->Scale, scaleTemp);
	DirectX::XMStoreFloat4x4(&this->Translation, translTemp);
	this->position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	scaleTemp = DirectX::XMLoadFloat4x4(&this->Scale);
	translTemp = DirectX::XMLoadFloat4x4(&this->Translation);

}

Model::~Model()
{

}

bool Model::addQuads(DirectX::XMFLOAT3 pos, float width, float height, float depth, int face)
{//Will change vertexbuffer and vertexcount
	this->vertexCount += 6;
	return quads.addQuad(pos, width, height, depth, face, this->vertexCount);
	
}

bool Model::addCube(DirectX::XMFLOAT3 pos, float width, float height, float depth)
{
	this->cubes.addCube(pos, width, height, depth);
	this->position = pos;
	return true;
}

bool Model::addTri(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 p3)
{
	return quads.addTri(p1, p2, p3, this->vertexCount);
}

void Model::shutdown()
{
	if (this->vertexBuffer != nullptr)
		this->vertexBuffer->Release();
	//if (this->indexBuffer != nullptr)
		//this->indexBuffer->Release();
	//if (this->constantBuffer != nullptr)
	//	this->constantBuffer->Release();
	if (this->SamplerState != nullptr)
		this->SamplerState->Release();
	this->texture.cleanUp();
	this->normal.cleanUp();
}

bool Model::insertCubesInVec()
{
	return this->cubes.insertVector(body, this->vertexCount);
}

void Model::moveCube(int id)
{
	//this->cubes.moveCube(id);
	//insertCubesInVec();
}

bool Model::createTheVertexBuffer(ID3D11Device *& gDevice)
{
	//return this->cubes.insertVertexBuffer(gDevice, this->vertexBuffer, this->vertexCount);
	unsigned int nrOfVertex = body.size();
	this->vertexCount = nrOfVertex;
	Vertex3D *temp = new Vertex3D[nrOfVertex];
	int vertices = 0;
	for (int i = 0; i < (int)nrOfVertex; i++)
	{
		temp[i] = body.at(i);
	}
	D3D11_SUBRESOURCE_DATA data;
	HRESULT hr;
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = nrOfVertex * sizeof(Vertex3D);
	data.pSysMem = temp;
	hr = gDevice->CreateBuffer(&bufferDesc, &data, &this->vertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	delete[] temp;

	this->calculateModelVectors();

	return true;
}

void Model::setVertexBandTexture(ID3D11DeviceContext *& gDeviceContext)
{
	UINT32 vertexSize = sizeof(Vertex3D);

	UINT32 offset = 0;
	gDeviceContext->PSSetShaderResources(0, 1, &this->texture.getTexture());
	gDeviceContext->PSSetShaderResources(1, 1, &this->normal.getTexture());
	gDeviceContext->IASetVertexBuffers(0, 1, &this->vertexBuffer, &vertexSize, &offset);
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gDeviceContext->PSSetSamplers(0, 1, &this->SamplerState);
	//gDeviceContext->GSSetShader(nullptr, nullptr, 0); //already initilized?+
	//this->calculateModelVectors();
}

int Model::getVertexCount() const
{
	return this->vertexCount;
}

void Model::setTheTexture(ID3D11Device *& gDevice, ID3D11DeviceContext *&gDeviceContext, std::string filename, std::string normalFileName)
{
	this->texture.setTexture(gDevice, gDeviceContext, filename);
	this->normal.setTexture(gDevice, gDeviceContext, normalFileName);
}

void Model::setSampler(ID3D11Device*& gDevice)
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = gDevice->CreateSamplerState(&desc, &this->SamplerState);
	if (FAILED(hr))
	{
		//MessageBox(hwnd, "Error compiling shader.  Check shader-error.txt for message.", "error", MB_OK);
		//deal with error. Log it maybe
		
	}
}
void Model::loadOBJ(char * file, char* normal, ID3D11Device * device, ID3D11DeviceContext * deviceContext) //directX::XMFLOAT3
{
	std::string txt = load.loadFile(file, body);
	if (txt == "")
		return;
	this->texture.setTexture(device, deviceContext, txt);
	//txt = load.loadFile(normal, body);
	//if (txt == "")
	//	return;
	this->normal.setTexture(device, deviceContext, normal);
	this->vertexCount += body.size();
}

//void Model::draw(ColorShader & shader, ID3D11DeviceContext * deviceContext) //forward
//{
//	this->setVertexBandTexture(deviceContext);
//	//Maybe shader.Render so the worldmatrix and all that updates everytime a new object gets drawn?
//	shader.RenderShader(deviceContext, this->vertexCount);
//}

void Model::draw(DeferedShader & shader, ID3D11DeviceContext * deviceContext)
{
	this->setVertexBandTexture(deviceContext);
	//Maybe shader.Render so the worldmatrix and all that updates everytime a new object gets drawn?
	shader.RenderShader(deviceContext, this->vertexCount);
}


DirectX::XMFLOAT4X4 Model::getId()
{
	return this->world;
}

void Model::setWorld(DirectX::XMMATRIX mtrx)
{
	DirectX::XMStoreFloat4x4(&this->world, mtrx);
	//this->world = mtrx;
}

void Model::setWorld()
{
	DirectX::XMMATRIX rotTemp;
	DirectX::XMMATRIX scaleTemp;
	DirectX::XMMATRIX translTemp;
	rotTemp = DirectX::XMLoadFloat4x4(&this->Rotation);
	scaleTemp = DirectX::XMLoadFloat4x4(&this->Scale);
	translTemp = DirectX::XMLoadFloat4x4(&this->Translation);
	DirectX::XMStoreFloat4x4(&this->world, (rotTemp*translTemp));

}

void Model::setPosition(float x, float y, float z)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
	DirectX::XMMATRIX tempTransl = DirectX::XMMatrixTranslation(x, y, z);
	DirectX::XMStoreFloat4x4(&this->Translation, tempTransl);
}

void Model::move(float x, float y, float z)
{
	this->position.x += x;
	this->position.y += y;
	this->position.z += z;
	DirectX::XMMATRIX tempTransl = DirectX::XMMatrixTranslation(this->position.x, this->position.y, this->position.z);
	DirectX::XMStoreFloat4x4(&this->Translation, tempTransl);
}

void Model::rotate(DirectX::XMVECTOR axis, float angle)
{
	DirectX::XMMATRIX tempRot = DirectX::XMMatrixRotationAxis(axis, angle);
	DirectX::XMStoreFloat4x4(&this->Rotation, tempRot);
}

void Model::billboard(DirectX::XMFLOAT3 camPos)
{
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX translateMatrix;

	double angle = atan2(position.x - camPos.x, position.z - camPos.z) * (180.0 / DirectX::XM_PI);
	double angle1 = atan2(position.x - camPos.x, position.z - camPos.z)* (180.0 / DirectX::XM_PI);
	//double angle2 = atan( position.z - camPos.z)* (180.0 / DirectX::XM_PI);
	float rotation = (float)angle * 0.0174532925f;
	float rotation1  = (float)angle1 * 0.0174532925f;
	// rotation2 = (float)angle2 * 0.0174532925f;
	

	worldMatrix = DirectX::XMLoadFloat4x4(&world);
	//worldMatrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationX(-rotation1), DirectX::XMMatrixRotationZ(-rotation2));
	worldMatrix =DirectX::XMMatrixRotationY(rotation);

	// Setup the translation matrix from the billboard model.
	translateMatrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// Finally combine the rotation and translation matrices to create the final world matrix for the billboard model.
	worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, translateMatrix);
	DirectX::XMStoreFloat4x4(&this->world, worldMatrix);
}

DirectX::XMFLOAT3 Model::getPosition() const
{
	return this->position;
}

void Model::setDistanceToCam(DirectX::XMFLOAT3 camPos)
{
	float x = camPos.x - this->position.x;
	float y = camPos.y - this->position.y;
	float z = camPos.z - this->position.z;
	distanceToCam = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	std::string string = "\n = \n";
	string += std::to_string(distanceToCam);
	OutputDebugStringA(string.c_str());
}

float Model::getDistanceToCam()
{
	return this->distanceToCam;
}

void Model::calculateModelVectors()
{
	int faceCount = vertexCount / 3;
	int index = 0;
	NM_Vertex vertex1;
	NM_Vertex vertex2;
	NM_Vertex vertex3;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT3 binormal;
	DirectX::XMFLOAT3 normal;

	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for (int i = 0; i < faceCount; i++)
	{
		// Get the three vertices for this face from the model.
		vertex1.x = body.at(index).x;
		vertex1.y = body.at(index).y;
		vertex1.z = body.at(index).z;
		vertex1.u = body.at(index).u;
		vertex1.v = body.at(index).v;
		vertex1.nx = body.at(index).nx;
		vertex1.ny = body.at(index).ny;
		vertex1.nz = body.at(index).nz;
		index++;

		vertex2.x = body.at(index).x;
		vertex2.y = body.at(index).y;
		vertex2.z = body.at(index).z;
		vertex2.u = body.at(index).u;
		vertex2.v = body.at(index).v;
		vertex2.nx = body.at(index).nx;
		vertex2.ny = body.at(index).ny;
		vertex2.nz = body.at(index).nz;
		index++;

		vertex3.x = body.at(index).x;
		vertex3.y = body.at(index).y;
		vertex3.z = body.at(index).z;
		vertex3.u = body.at(index).u;
		vertex3.v = body.at(index).v;
		vertex3.nx = body.at(index).nx;
		vertex3.ny = body.at(index).ny;
		vertex3.nz = body.at(index).nz;
		index++;
		// Calculate the tangent and binormal of that face.
		calculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal, normal);

		// Calculate the new normal using the tangent and binormal.
		calculateNormal(tangent, binormal, normal);

		// Store the normal, tangent, and binormal for this face back in the model structure.
		body.at(index - 1).nx = normal.x;
		body.at(index - 1).ny = normal.y;
		body.at(index - 1).nz = normal.z;
		body.at(index - 1).tx = tangent.x;
		body.at(index - 1).ty = tangent.y;
		body.at(index - 1).tz = tangent.z;
		body.at(index - 1).bx = binormal.x;
		body.at(index - 1).by = binormal.y;
		body.at(index - 1).bz = binormal.z;

		body.at(index - 2).nx = normal.x;
		body.at(index - 2).ny = normal.y;
		body.at(index - 2).nz = normal.z;
		body.at(index - 2).tx = tangent.x;
		body.at(index - 2).ty = tangent.y;
		body.at(index - 2).tz = tangent.z;
		body.at(index - 2).bx = binormal.x;
		body.at(index - 2).by = binormal.y;
		body.at(index - 2).bz = binormal.z;

		body.at(index - 3).nx = normal.x;
		body.at(index - 3).ny = normal.y;
		body.at(index - 3).nz = normal.z;
		body.at(index - 3).tx = tangent.x;
		body.at(index - 3).ty = tangent.y;
		body.at(index - 3).tz = tangent.z;
		body.at(index - 3).bx = binormal.x;
		body.at(index - 3).by = binormal.y;
		body.at(index - 3).bz = binormal.z;
	}
}

void Model::calculateTangentBinormal(NM_Vertex vertex1, NM_Vertex vertex2, NM_Vertex vertex3, DirectX::XMFLOAT3 & tangent, DirectX::XMFLOAT3 & binormal, DirectX::XMFLOAT3 & normal)
{

	// Calculate the two vectors for this face.
	float vector1[3];
	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;
	float vector2[3];
	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	// Calculate the tu and tv texture space vectors.
	float tuVector[2];
	float tvVector[2];
	tuVector[0] = vertex2.u - vertex1.u;
	tvVector[0] = vertex2.v - vertex1.v;
	tuVector[1] = vertex3.u - vertex1.u;
	tvVector[1] = vertex3.v - vertex1.v;

	// Calculate the denominator of the tangent/binormal equation.
	float den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	float length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// Normalize the normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// Normalize the normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;
}

void Model::calculateNormal(DirectX::XMFLOAT3 tangent, DirectX::XMFLOAT3 binormal, DirectX::XMFLOAT3 & normal)
{
	// Calculate the cross product of the tangent and binormal which will give the normal vector.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// Calculate the length of the normal.
	float length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// Normalize the normal.
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;
}
