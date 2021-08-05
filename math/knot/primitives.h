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
	Vector vector_product(const Vector &v) const {
		return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
	double square() const { return x * x + y * y + z * z; }
	double length() const { return sqrt(x * x + y * y + z * z); }

	void add(const Vector &v, double coef = 1.0) {
		this->x += v.x * coef;
		this->y += v.y * coef;
		this->z += v.z * coef;
	}

	void swap(Vector &v) {
		{ double swap = this->x; this->x = v.x; v.x = swap; }
		{ double swap = this->y; this->y = v.y; v.y = swap; }
		{ double swap = this->z; this->z = v.z; v.z = swap; }
	}

	void normalize() {
		double len = this->length();
		this->x /= len;
		this->y /= len;
		this->z /= len;
	}

	static Vector linear(const Vector &v0, double coef0, const Vector &v1, double coef1) {
		return Vector(
			coef0 * v0.x + coef1 * v1.x,
			coef0 * v0.y + coef1 * v1.y,
			coef0 * v0.z + coef1 * v1.z
		);
	}
};

inline void Point::move(const Vector &v, double coef) {
	this->x += v.x * coef;
	this->y += v.y * coef;
	this->z += v.z * coef;
}

}}

#endif /* __POINT_H__ */
