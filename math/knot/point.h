#ifndef __POINT_H__
#define __POINT_H__

namespace KE { namespace ThreeD {

struct Point {
	double x, y, z;

	Point(double x, double y, double z) : x(x), y(y), z(z) {}
};

}}

#endif /* __POINT_H__ */
