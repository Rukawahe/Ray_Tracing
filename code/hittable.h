#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include"Integration.h"

class material;
struct hit_record{
	point3 p;//�����յ�����
	vec3 normal;//���е�ķ�����
	double t;//�������߳���
	shared_ptr<material>mat_ptr;//���еĲ���
	bool front_face;
	//������
	inline void set_face_normal(const ray&r, const vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};
class hittable
{
public:
	//���ڼ�¼���ߵ�������Ϣ���������Ƿ���С�t_min��t_max�����޶����߳��ȣ��ų������еĽ�Զ����
	virtual bool hit(const ray&r, double t_min, double t_max, hit_record& rec)const = 0;
};
#endif // !HITTABLE_H
