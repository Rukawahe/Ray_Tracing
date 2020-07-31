#ifndef MATERIAL_H
#define MATERIAL_H

#include"Integration.h"
#include"hittable.h"


class material {
public:
	//r_inΪ�������, scatteredΪɢ�����, attenuation ��˼Ϊ˥������ʵ��Ϊ��ͨ���ķ�����
	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)const = 0;

};
//lambert���ʣ����⻬�ı����������ɢ����γɵĲ�������
class lambertian :public material {
public :
	color albedo;//������
public:
	lambertian(const color& r) :albedo(r){}
	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)const override{
		vec3 scatter_direction = rec.normal + random_unit_vector();
		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}
};
//���з��书�ܵĽ���,���ǿ����÷��������΢ƫ��һ�㣬��ʵ�ֽ������ƹ�Ч��
class metal : public material {
public:
	color albedo;
	double fuzz;//ģ����
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

