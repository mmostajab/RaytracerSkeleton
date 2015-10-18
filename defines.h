#ifndef __DEFINES_H__
#define __DEFINES_H__

// The image width and height should be of the multiplier of 4 as there is padding in the bitmap saving function.
#define IMG_WIDTH	400
#define IMG_HEIGHT	400
#define PIXEL_SIZE  1.0

namespace RayTracer
{
	struct RGBColor {
		RGBColor(const float& _r, const float& _g, const float& _b, const float& _a): r(_r), g(_g), b(_b), a(_a) {}

		float r, g, b, a;	

		static RGBColor BLACK;
	};
}

#endif