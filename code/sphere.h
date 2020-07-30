#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
class sphere:public hittable
{
public:
	point3 center;
	double radius;
public:
	sphere(){}
	sphere(point3 cen, double r) :center(cen), radius(r) {};

	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec)const override;

};
//如果命中，命中记录保存在rec中
bool sphere::hit(const ray& r, double tmin, double tmax, hit_record& rec)const {
	vec3 oc = r.origin() - center;
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;
	auto discriminant = half_b * half_b - a * c;
	if (discriminant > 0) {
		auto root = sqrt(discriminant);
		auto temp = (-half_b - root) / a;//小根
		if (temp<tmax && temp>tmin) {
			rec.t = temp;
			rec.p = r.at(rec.t);
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(r, outward_normal);
			return true;
		}

		temp = (-half_b + root) / a;//大根
		if (temp<tmax && temp>tmin) {
			rec.t = temp;
			rec.p = r.at(rec.t);
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_face_normal(r, outward_normal);
			return true;
		}
	}
	return false;
}
#endif // !SPHERE_H

