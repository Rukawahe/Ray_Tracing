#ifndef HITTTABLE_LIST
#define HITTTABLE_LIST

#include "hittable.h"

#include<memory>
#include<vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list:public hittable
{
public:
	std::vector<shared_ptr<hittable>>objects;
public:
	hittable_list(){}
	hittable_list(shared_ptr<hittable>object) { add(object); }

	void clear() { objects.clear(); }
	void add(shared_ptr<hittable>object) { objects.push_back(object); }

	virtual bool hit(const ray&r, double t_min, double t_max, hit_record& rec)const override;

};
//作用是遍历数组中的所有物体，记录目前当前射线命中的最近的球，然后返回是否命中。
bool hittable_list::hit(const ray&r, double t_min, double t_max, hit_record& rec)const {
	hit_record temp_rec;
	bool hit_anything = false;
	auto closest_so_far = t_max;  //记录目前最近的物体
	
	for (const auto& object : objects) {
		if (object->hit(r, t_min, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;  //信息保存在rec中
		}
	}
	return hit_anything;
}

#endif // !HITTTABLE_LIST
