#include "Vec2.h"

namespace clm {

// Constructors
Vec2::Vec2() : x(0), y(0) {}
Vec2::Vec2(const Vec2 &a) : x(a.x), y(a.y) {}
Vec2::Vec2(float nx, float ny) : x(nx), y(ny) {}

Vec2 &Vec2::operator =(const Vec2 &a) {
	x = a.x; y = a.y;
	return *this;
}
bool Vec2::operator ==(const Vec2 &a) const {
	return x == a.x && y == a.y;
}
bool Vec2::operator !=(const Vec2 &a) const {
	return x != a.x || y != a.y;
}
void Vec2::zero() { x = y = 0.0f; }
Vec2 Vec2::operator -() const { return Vec2(-x,-y); }
Vec2 Vec2::operator +(const Vec2 &a) const {
	return Vec2(x + a.x, y + a.y);
}
Vec2 Vec2::operator -(const Vec2 &a) const {
	return Vec2(x - a.x, y - a.y);
}
Vec2 Vec2::operator *(float a) const {
	return Vec2(x*a, y*a);
}
Vec2 Vec2::operator /(float a) const {
	float oneOverA = 1.0f / a;
	return Vec2(x*oneOverA, y*oneOverA);
}
Vec2 &Vec2::operator +=(const Vec2 &a) {
	x += a.x; y += a.y;
	return *this;
}
Vec2 &Vec2::operator -=(const Vec2 &a) {
	x-= a.x; y -= a.y;
	return *this;
}
Vec2 &Vec2::operator *=(float a) {
	x *= a; y *= a;
	return *this;
}
Vec2 &Vec2::operator /=(float a) {
	float oneOverA = 1.0f / a;
	x *= oneOverA; y *= oneOverA;
	return *this;
}
void Vec2::normalize() {
	float magSq = x*x + y*y;
	if(magSq > 0.0f) {
	float oneOverMag = 1.0f / sqrt(magSq);
	x *= oneOverMag;
	y *= oneOverMag;
	}
}
float Vec2::operator *(const Vec2 &a) const {
	return x*a.x + y*a.y;
}

//Nonmember functions
/*inline float vectorMag(const Vec2 &a) {
    return sqrt(a.x*a.x + a.y*a.y);
}
inline float vectorMagP2(const Vec2 &a) {
    return (a.x*a.x + a.y*a.y);
}
inline Vec2 operator *(float k, const Vec2 &v) {
    return Vec2(k*v.x, k*v.y);
}
inline float distance(const Vec2 &a, const Vec2 &b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return sqrt(dx*dx + dy*dy);
}*/

}
