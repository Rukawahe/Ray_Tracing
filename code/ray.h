#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
	public:
		point3 orig;//光线起点
		vec3 dir;//光线方向
	public:
		ray(){}
		ray(const point3 &origin,const vec3 &direction)
			:orig(origin),dir(direction)
		{}
		
		point3 origin() const { return orig; }
		vec3 direction() const { return dir; }	

		//P(t)=A+tb 向量函数
		point3 at(double t) const {
			return orig + t * dir;
		}
};

#endif // RAY_H
