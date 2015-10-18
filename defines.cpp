#include "defines.h"

using namespace RayTracer;

RGBColor RGBColor::BLACK(0, 0, 0, 1);

float RayTracer::dot(const float vec0[3], const float vec1[3]) {
	return vec0[0] * vec1[0] + vec0[1] * vec1[1] + vec0[2] * vec1[2];
}