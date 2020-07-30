#ifndef INTEGRATION_H
#define INTEGRATION_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

//usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

//constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

//生成[0,1)的随机数
inline double random_double() {
	return rand() / (RAND_MAX + 1.0);
}

//夹紧x函数
inline double clamp(double x, double min, double max) {
	if (x < min)	return min;
	if (x > max)	return max;
	return x;
}
#include "ray.h"
#include "vec3.h"

#endif // !INTEGRATION_H

