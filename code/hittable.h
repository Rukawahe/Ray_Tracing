#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
struct hit_record{
	point3 p;//命中终点坐标
	vec3 normal;//命中点的法向量
	double t;//命中射线长度
	bool front_face;
	//求法向量
	inline void set_face_normal(const ray&r, const vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};
class hittable
{
public:
	//用于记录射线的命中信息，并返回是否击中。t_min和t_max用于限定射线长度，排除被命中的较远物体
	virtual bool hit(const ray&r, double t_min, double t_max, hit_record& rec)const = 0;
};
#endif // !HITTABLE_H
