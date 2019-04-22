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

bool Intersects(const AABB a, const AABB b)
{
	// overlap
	return true;
}

ostream& operator<<(ostream & stream, const Sphere & shape) {
	stream << "\nPosition:(" << shape.position.x << ", " << shape.position.y << ", " << shape.position.z << "), " <<
		"Radius: " << shape.radius;
	return stream;
}
ostream& operator<<(ostream & stream, const AABB & shape) {
	stream << "\nPosition:(" << shape.position.x << ", " << shape.position.y << ", " << shape.position.z << "), " <<
		"Size:(" << shape.size.x << ", " << shape.size.y << ", " << shape.size.z << "), ";
	return stream;
}
