#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <iostream>
#include <DirectXMath.h>
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
	XMFLOAT3 position;
	XMFLOAT3 size; // HALF SIZE!

	AABB() : size(1, 1, 1), position(1, 1, 1) { }
	AABB(const XMFLOAT3& p, const XMFLOAT3& s) :
		position(p), size(s) { }
	friend bool Intersects(const AABB b1, const AABB b2);
}
typedef AABB;

std::ostream& operator<<(std::ostream& os, const Sphere& shape);
std::ostream& operator<<(std::ostream& os, const AABB& shape);

#endif // !GEOMETRY_H