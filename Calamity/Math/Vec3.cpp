#include "Vec3.h"

namespace clm {

// Constructors
Vec3::Vec3() : x(0), y(0), z(0) {}
Vec3::Vec3(const Vec3 &a) : x(a.x), y(a.y), z(a.z) {}
Vec3::Vec3(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}

Vec3 &Vec3::operator =(const Vec3 &a) {
	x = a.x; y = a.y, z = a.z;
	return *this;
}
bool Vec3::operator ==(const Vec3 &a) const {
	return x == a.x && y == a.y && z == a.z;
}
bool Vec3::operator !=(const Vec3 &a) const {
	return x != a.x || y != a.y || z != a.z;
}
void Vec3::zero() { x = y = z = 0.0f; }
Vec3 Vec3::operator -() const { return Vec3(-x,-y, -z); }
Vec3 Vec3::operator +(const Vec3 &a) const {
	return Vec3(x + a.x, y + a.y, z + a.z);
}
Vec3 Vec3::operator -(const Vec3 &a) const {
	return Vec3(x - a.x, y - a.y, z - a.z);
}
Vec3 Vec3::operator *(float a) const {
	return Vec3(x*a, y*a, z*a);
}
Vec3 Vec3::operator /(float a) const {
	float oneOverA = 1.0f / a;
	return Vec3(x*oneOverA, y*oneOverA, z*oneOverA);
}
Vec3 &Vec3::operator +=(const Vec3 &a) {
	x += a.x; y += a.y; z += a.z;
	return *this;
}
Vec3 &Vec3::operator -=(const Vec3 &a) {
	x-= a.x; y -= a.y; z -= a.z;
	return *this;
}
Vec3 &Vec3::operator *=(float a) {
	x *= a; y *= a; z *= a;
	return *this;
}
Vec3 &Vec3::operator /=(float a) {
	float oneOverA = 1.0f / a;
	x *= oneOverA; y *= oneOverA; z *= oneOverA;
	return *this;
}
void Vec3::normalize() {
	float magSq = x*x + y*y + z*z;
	if(magSq > 0.0f) {
	float oneOverMag = 1.0f / sqrt(magSq);
	x *= oneOverMag;
	y *= oneOverMag;
	z *= oneOverMag;
	}
}
float Vec3::operator *(const Vec3 &a) const {
	return x*a.x + y*a.y + z*a.z;
}

}
