#pragma once

#include <math.h>

namespace clm {

class Vec2 {
public:
	float x, y;
	
	// Constructors
	Vec2();
	Vec2(const Vec2 &a);
	Vec2(float nx, float ny);
	
	Vec2 &operator =(const Vec2 &a);
	bool operator ==(const Vec2 &a) const;
	bool operator !=(const Vec2 &a) const;
	void zero();
	Vec2 operator -() const;
	Vec2 operator +(const Vec2 &a) const;
	Vec2 operator -(const Vec2 &a) const;
	Vec2 operator *(float a) const;
	Vec2 operator /(float a) const;
	Vec2 &operator +=(const Vec2 &a);
	Vec2 &operator -=(const Vec2 &a);
	Vec2 &operator *=(float a);
	Vec2 &operator /=(float a);
	void normalize();
	float operator *(const Vec2 &a) const;
};

//Nonmember functions
/*float vectorMag(const mtVec2 &a);
float vectorMagP2(const mtVec2 &a);
mtVec2 operator *(float k, const mtVec2 &v);
float distance(const mtVec2 &a, const mtVec2 &b);*/

inline float vectorMag(const Vec2 &a) {
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
}
// maybe make it not inline
inline Vec2 vectorRot2D(const Vec2 &v, const Vec2 &rotCenter, float rotation) {
	float s = sin(rotation);
	float c = cos(rotation);
	Vec2 result;
	result.x = (v.x-rotCenter.x)*c - (v.y-rotCenter.y)*s;
	result.y = (v.x-rotCenter.x)*s + (v.y-rotCenter.y)*c;
	result += rotCenter;
	return result;
	//return mtVec2(v.x*c - v.y*s, v.x*s - v.y*c);
}

inline Vec2 vmin(const Vec2 &a, const Vec2& b) {
    Vec2 result;
    result.x = (a.x < b.x) ? a.x : b.x;
    result.y = (a.y < b.y) ? a.y : b.y;
    return result;
}

inline Vec2 vmax(const Vec2 &a, const Vec2& b) {
    Vec2 result;
    result.x = (a.x > b.x) ? a.x : b.x;
    result.y = (a.y > b.y) ? a.y : b.y;
    return result;
}

}
