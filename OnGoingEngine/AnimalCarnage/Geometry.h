#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <iostream>
#include<DirectXMath.h>
using namespace DirectX;
using namespace std;
struct Sphere
{
	XMFLOAT3 position;
	float radius;

	Sphere() : radius(1.f), position(1,1,1){ }
	Sphere(const XMFLOAT3& p, float r) :
		position(p), radius(r) { }
	friend bool Intersects(const Sphere s1, const Sphere s2);
}
typedef Sphere;
struct AABB {
	//XMFLOAT3 position;
	//XMFLOAT3 size; // HALF SIZE!
	XMFLOAT3 Max;
	XMFLOAT3 Min;
	XMFLOAT3 offset;
	float height;
	float width;
	float depth;

	AABB() : Max(1, 1, 1), Min(1, 1, 1) { }
	AABB(const XMFLOAT3& p, const XMFLOAT3& s) :
		Max(p), Min(s) { }
	friend bool Intersects(const AABB a, XMFLOAT3 posA, const AABB b, XMFLOAT3 posB);
	friend bool Intersects(DirectX::XMFLOAT3 aabbmin, DirectX::XMFLOAT3 aabbMax, const AABB b, XMFLOAT3 posB);
}
typedef AABB;

std::ostream& operator<<(std::ostream& os, const Sphere& shape);
std::ostream& operator<<(std::ostream& os, const AABB& shape);

#endif // !GEOMETRY_H