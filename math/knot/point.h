#ifndef __POINT_H__
#define __POINT_H__

struct point {
	double x;
	double y;
	double z;

	point(double x, double y, double z) : x(x), y(y), z(z) {}
};

#endif /* __POINT_H__ */
