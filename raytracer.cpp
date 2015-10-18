#include "raytracer.h"

// STD
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <math.h>
#include <limits>

using namespace RayTracer;

std::random_device CRayTracer::rd;
std::mt19937 CRayTracer::RandomGenerator(CRayTracer::rd());

// ================================================================================
// == Example 1 code

bool intersectSphere(Ray& ray, const Sphere& sphere){
	  float b = dot(ray.origin, ray.dir);
	  float c = dot(ray.origin, ray.origin) - sphere.radius * sphere.radius;
	  float disc = b*b - c;

	  if (disc > 0.0f){
	    float sdisc = sqrtf(disc);
	    float root1 = (-b - sdisc);
	    if(root1 >= ray.t_min && root1 <= ray.t_max){
	      ray.t_max = root1;
	      return true;
	    }

	    float root2 = (-b + sdisc) + root1;
	    if(root2 >= ray.t_min && root2 <= ray.t_max){
	      ray.t_max = root2;
	      return true;
	    }
	}

	return false;
}

//

CRayTracer::CRayTracer(void) : m_ColorBuffer(3 * IMG_WIDTH * IMG_HEIGHT), m_BackgroundColor(RGBColor::BLACK) {
	sphere.radius = 150.f;
}


// Renders Scene from an orthographic view
void CRayTracer::renderScene(void) {

	for(int r = 0; r < IMG_HEIGHT; r++){
		for(int c = 0; c < IMG_WIDTH; c++){
			RGBColor pixelColor = m_BackgroundColor;

			Ray ray;

			std::uniform_real_distribution<float> dis(0.0f, 1.0f);

			// Setting the ray origin
			ray.origin[0] = r - IMG_WIDTH  / 2.0f;
			ray.origin[1] = c - IMG_HEIGHT / 2.0f;
			ray.origin[2] = -100.0f;

			// Specify the distance where the intersection can happen
			ray.t_min = 0.0f;
			ray.t_max = std::numeric_limits<float>::max();

			// Setting the ray direction -> in z-direction
			ray.dir[0] = 0.f;
			ray.dir[1] = 0.f;
			ray.dir[2] = 1.f;

			// trace rays to compute the pixel color
			if(intersectSphere(ray, sphere)){
				pixelColor = RGBColor(1, 0, 0, 1);
			}
			else
				pixelColor = RGBColor(dis(RandomGenerator), dis(RandomGenerator), dis(RandomGenerator), 1.0f);

			drawPixel(r, c, pixelColor);
		}
	}

}

void CRayTracer::drawPixel(const int& _row, const int& _col, const RGBColor& _pixelColor) {
	// Store the pixel color into color buffer
	m_ColorBuffer[3 * (_row * IMG_WIDTH + _col) + 0] = _pixelColor.r;
	m_ColorBuffer[3 * (_row * IMG_WIDTH + _col) + 1] = _pixelColor.g;
	m_ColorBuffer[3 * (_row * IMG_WIDTH + _col) + 2] = _pixelColor.b;
}

void CRayTracer::saveBitmapToFile(const std::string& _filename) const {
	// Save to ppm file
	std::ofstream ppmFile(_filename);
	if(!ppmFile){
		std::cerr << "Cannot open " << _filename << std::endl;
		return;
	}

	ppmFile << "P3" << std::endl;
	ppmFile << "# " << _filename << std::endl;
	ppmFile << IMG_WIDTH << " " << IMG_HEIGHT << std::endl;
	ppmFile << "255" << std::endl;

	for(int r = 0; r < IMG_HEIGHT; r++){
		for(int c = 0; c < IMG_WIDTH; c++){
			ppmFile 	<< static_cast<int>( m_ColorBuffer[3 * (r * IMG_WIDTH + c) + 0] * 255 ) << " "
						<< static_cast<int>( m_ColorBuffer[3 * (r * IMG_WIDTH + c) + 1] * 255 ) << " "
						<< static_cast<int>( m_ColorBuffer[3 * (r * IMG_WIDTH + c) + 2] * 255 ) << "\t";
		}
		ppmFile << std::endl;
	}
}

const float* CRayTracer::getColorBuffer() const {
	return m_ColorBuffer.data();
}

CRayTracer::~CRayTracer(void) {
}
