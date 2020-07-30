#include "Integration.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"


//����һ�����ߣ��ж��Ƿ���������
double hit_sphere(const point3& center, double radius, const ray& r) {
	vec3 oc = r.origin() - center;
	auto a = r.direction().length_squared();
	auto half_b = dot(r.direction(), oc);
	auto c = oc.length_squared() - radius * radius;
	auto discriminant = half_b* half_b -  a*c;//�����ж�ʽ
	if (discriminant < 0.0)
		return -1.0;
	else
		return (-half_b - sqrt(discriminant)) / a;


}
//����һ�����ߣ������������������������Ļ����ɫֵ
color ray_color(const ray& r,const hittable& world) {
	hit_record rec;
	if (world.hit(r, 0, infinity, rec))
		return 0.5*(rec.normal + color(1, 1, 1));
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5*(unit_direction.y() + 1.0);
	return (1.0 - t)*color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);//�԰�ɫ��ǳ��ɫ��ֵ
}

int main() {
	//Image��Ļλ��
	const auto aspect_ratio = 16 / 9;//�ݺ��
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 100;



	//world,��ų��������е���
	hittable_list world;
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

	//camera
	camera cam;

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			color pixel_color(0, 0, 0);
			//��һ�������������ʹ�ö������߽��в����������ƽ��������ʵ�ֿ����
			for (int s = 0; s < samples_per_pixel; ++s) {
				auto u = (i + random_double()) / (image_width - 1);
				auto v = (j + random_double()) / (image_height - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world);
			}
			write_color(std::cout, pixel_color, samples_per_pixel);
		}
	}
	std::cerr << "\nDone.\n";	
}