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
	//Check if AABB1's max is greater than AABB2's min and AABB1's min is less than AABB2's max
	return(a.Max.x > b.Min.x &&
		a.Min.x < b.Max.x &&
		a.Max.y >b.Min.y &&
		a.Min.y < b.Max.y &&
		a.Max.z > b.Min.z &&
		a.Min.z < b.Max.z);
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
