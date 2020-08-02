#include "Integration.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include"material.h"


//发射一条射线，判断是否命中球体
double hit_sphere(const point3& center, double radius, const ray& r) {
	vec3 oc = r.origin() - center;
	auto a = r.direction().length_squared();
	auto half_b = dot(r.direction(), oc);
	auto c = oc.length_squared() - radius * radius;
	auto discriminant = half_b* half_b -  a*c;//根的判断式
	if (discriminant < 0.0)
		return -1.0;
	else
		return (-half_b - sqrt(discriminant)) / a;


}
//发射一条射线，并采样该射线最终输出到屏幕的颜色值
color ray_color(const ray& r,const hittable& world,int depth) {
	hit_record rec;
	//如果我们超过光线反弹限制，则不再收集光线
	if (depth <= 0)
		return color(0, 0, 0);
	if (world.hit(r, 0.001, infinity, rec)) {
		/*
		//将得到的随机点转换为场景所在的世界坐标
		point3 target = rec.p + random_in_hemisphere(rec.normal);//半球散射
		return 0.5*ray_color(ray(rec.p,target-rec.p),world,depth-1);//假设每次反射会被吸收一半的光线能量，则每次的反射采样的返回值要减半
		//如果光线一直反射，则上述递归会一直进行下去，直到射线没有命中物体，则采样天空的颜色并返回
		*/
		ray scattered;
		color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);
		return color(0, 0, 0);

	}
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5*(unit_direction.y() + 1.0);
	return (1.0 - t)*color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);//对白色和浅蓝色插值
}

int main() {
	//Image屏幕位置
	const auto aspect_ratio = 16 / 9;//纵横比
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 100;
	const int max_depth = 50;


	//world,存放场景中所有的球
	auto R = cos(pi / 4);
	hittable_list world;

	auto material_left = make_shared<lambertian>(color(0, 0, 1));
	auto material_right = make_shared<lambertian>(color(1, 0, 0));

	world.add(make_shared<sphere>(point3(-R, 0, -1), R, material_left));
	world.add(make_shared<sphere>(point3(R, 0, -1), R, material_right));
	/*hittable_list world;
	auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
	auto material_left = make_shared<dielectric>(1.5);
	auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

	world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.4, material_left));
	world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));*/

	//camera
	camera cam(90.0, aspect_ratio);

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			color pixel_color(0, 0, 0);
			//在一个像素里面随机使用多条光线进行采样，最后求平均，可以实现抗锯齿
			for (int s = 0; s < samples_per_pixel; ++s) {
				auto u = (i + random_double()) / (image_width - 1);
				auto v = (j + random_double()) / (image_height - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world,max_depth);
			}
			write_color(std::cout, pixel_color, samples_per_pixel);
		}
	}
	std::cerr << "\nDone.\n";	
}