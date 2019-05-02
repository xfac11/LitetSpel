#include "Geometry.h"


bool Intersects(const Sphere s1, const Sphere s2)
{
	float sum = s1.radius + s2.radius;
	float distance = sqrtf(
		std::pow(s1.position.x - s2.position.x, 2) +
		std::pow(s1.position.y - s2.position.y, 2) +
		std::pow(s1.position.y - s2.position.y, 2));
	return distance < sum;
}

bool Intersects(const AABB a, XMFLOAT3 posA, const AABB b, XMFLOAT3 posB)
{
	
	DirectX::XMFLOAT3 minA;
	DirectX::XMFLOAT3 maxA;
	DirectX::XMFLOAT3 minB;
	DirectX::XMFLOAT3 maxB;

	minA = XMFLOAT3(posA.x - a.width, posA.y - a.height, posA.z - a.depth);
	maxA = XMFLOAT3(posA.x + a.width, posA.y + a.height, posA.z + a.depth);

	minB = XMFLOAT3(posB.x - b.width, posB.y - b.height, posB.z - b.depth);
	maxB = XMFLOAT3(posB.x + b.width, posB.y + b.height, posB.z + b.depth);
	//Check if AABB1's max is greater than AABB2's min and AABB1's min is less than AABB2's max

	return(maxA.x > minB.x &&
		minA.x < maxB.x &&
		maxA.y >minB.y &&
		minA.y < maxB.y &&
		maxA.z > minB.z &&
		minA.z < maxB.z);
}

bool Intersects(DirectX::XMFLOAT3 aabbmin, DirectX::XMFLOAT3 aabbMax,const AABB b, XMFLOAT3 posB)
{

	DirectX::XMFLOAT3 minA;
	DirectX::XMFLOAT3 maxA;
	DirectX::XMFLOAT3 minB;
	DirectX::XMFLOAT3 maxB;

	minA = aabbmin;
	maxA = aabbMax;

	minB = XMFLOAT3(posB.x - b.width, posB.y - b.height, posB.z - b.depth);
	maxB = XMFLOAT3(posB.x + b.width, posB.y + b.height, posB.z + b.depth);
	//Check if AABB1's max is greater than AABB2's min and AABB1's min is less than AABB2's max

	return(maxA.x > minB.x &&
		minA.x < maxB.x &&
		maxA.y >minB.y &&
		minA.y < maxB.y &&
		maxA.z > minB.z &&
		minA.z < maxB.z);
}

ostream& operator<<(ostream & stream, const Sphere & shape) {
	stream << "\nPosition:(" << shape.position.x << ", " << shape.position.y << ", " << shape.position.z << "), " <<
		"Radius: " << shape.radius;
	return stream;
}
ostream& operator<<(ostream & stream, const AABB & shape) {
	//stream << "\nPosition:(" << shape.position.x << ", " << shape.position.y << ", " << shape.position.z << "), " <<
	//	"Size:(" << shape.size.x << ", " << shape.size.y << ", " << shape.size.z << "), ";
	return stream;
}
