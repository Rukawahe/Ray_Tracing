#ifndef MATERIAL_H
#define MATERIAL_H

#include"Integration.h"
#include"hittable.h"


class material {
public:
	//r_in为入射光线, scattered为散射光线, attenuation 意思为衰减量，实际为各通道的反射率
	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)const = 0;

};
//lambert材质：不光滑的表面产生均匀散射而形成的材质类型
class lambertian :public material {
public :
	color albedo;//反射率
public:
	lambertian(const color& r) :albedo(r){}
	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)const override{
		vec3 scatter_direction = rec.normal + random_unit_vector();
		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}
};
//具有反射功能的金属,我们可以让反射光线稍微偏移一点，来实现金属的哑光效果
class metal : public material {
public:
	color albedo;
	double fuzz;//模糊量
public:
	metal(const color& a,double f) : albedo(a),fuzz(f<1?f:1) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected+fuzz*random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
};
#endif // !MATERIAL_H

