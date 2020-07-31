#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();
	
	//将颜色除以样本数,将颜色除以 Gamma=2.0 的样本数和伽玛校正
	auto scale = 1.0 / samples_per_pixel;
	r = sqrt(scale*r);
	g = sqrt(scale*g);
	b = sqrt(scale*b);

	// Write the translated [0,255] value of each color component.
	out << static_cast<int>(256 * clamp(r,0.0,0.999) )<< ' '
		<< static_cast<int>(256 * clamp(g,0.0,0.999)) << ' '
		<< static_cast<int>(256*clamp(b,0.0,0.999) )<< '\n';
}

#endif