#ifndef __POINT_H__
#define __POINT_H__

#include <cmath>

namespace KE { namespace ThreeD {

struct Vector;

struct Point {
	double x, y, z;

	Point(double x, double y, double z) : x(x), y(y), z(z) {}

	void move(const Vector &v, double coef = 1.0);
};

struct Vector {
	double x, y, z;
	Vector(double x, double y, double z) : x(x), y(y), z(z) {}
	Vector(const Point &start, const Point &end) : x(end.x - start.x), y(end.y - start.y), z(end.z - start.z) {}

	double scalar_product(const Vector &v) const { return x * v.x + y * v.y + z * v.z; }
	double square() const { return x * x + y * y + z * z; }
	double length() const { return sqrt(x * x + y * y + z * z); }

	void normalize() {
		double len = this->length();
		this->x /= len;
		this->y /= len;
		this->z /= len;
	}
};

inline void Point::move(const Vector &v, double coef) {
	this->x += v.x * coef;
	this->y += v.y * coef;
	this->z += v.z * coef;
}

}}

#endif /* __POINT_H__ */
