#ifndef CUBE_H
#define CUBE_H
#include"QuadVertex.h"
class Cube
{
public:
	Cube();
	Cube(DirectX::XMFLOAT3 pos, float width, float height, float depth);
	~Cube();
	void setPosition(DirectX::XMFLOAT3 newPos);
	void move(DirectX::XMFLOAT3 dir);
	void rotateX(float degree);
	void rotateY(float degree);
	void rotateZ(float degree);
	QuadVertex getQuad(int id);
private:
	QuadVertex cube[6];

};
#endif // !CUBE_H