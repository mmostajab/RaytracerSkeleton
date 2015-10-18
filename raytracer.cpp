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

	      ray.normal[0] = (ray.origin[0] + root1 * ray.dir[0]) / sphere.radius;
	      ray.normal[1] = (ray.origin[1] + root1 * ray.dir[1]) / sphere.radius;
	      ray.normal[2] = (ray.origin[2] + root1 * ray.dir[2]) / sphere.radius;

	      float normal_length = sqrtf(ray.normal[0] * ray.normal[0] + ray.normal[1] * ray.normal[1] + ray.normal[2] * ray.normal[2]);
			
		  ray.normal[0] /= normal_length;
	      ray.normal[1] /= normal_length;
	      ray.normal[2] /= normal_length;

	      return true;
	    }

	    float root2 = (-b + sdisc) + root1;
	    if(root2 >= ray.t_min && root2 <= ray.t_max){
	      ray.t_max = root2;

	      ray.normal[0] = (ray.origin[0] + root2 * ray.dir[0]) / sphere.radius;
	      ray.normal[1] = (ray.origin[1] + root2 * ray.dir[1]) / sphere.radius;
	      ray.normal[2] = (ray.origin[2] + root2 * ray.dir[2]) / sphere.radius;

	      float normal_length = sqrtf(ray.normal[0] * ray.normal[0] + ray.normal[1] * ray.normal[1] + ray.normal[2] * ray.normal[2]);
			
		  ray.normal[0] /= normal_length;
	      ray.normal[1] /= normal_length;
	      ray.normal[2] /= normal_length;
	      
	      return true;
	    }
	}

	return false;
}

//

CRayTracer::CRayTracer(void) : m_ColorBuffer(3 * IMG_WIDTH * IMG_HEIGHT), m_BackgroundColor(RGBColor::BLACK) {
	sphere.radius = 150.f;

	sphere.color.r = 0.6;
	sphere.color.g = 0.8;
	sphere.color.b = 0.5;
	sphere.color.a = 1.0;

	light_dir[0] = 0.58f;
	light_dir[1] = 0.58f;
	light_dir[2] = -0.58f;

	viewer_pos[0] = 0.f;
	viewer_pos[1] = 0.f;
	viewer_pos[2] = -200.f;
}


// Renders Scene from an orthographic view
void CRayTracer::renderScene(void) {

	for(int r = 0; r < IMG_HEIGHT; r++){
		for(int c = 0; c < IMG_WIDTH; c++){
			RGBColor pixelColor = m_BackgroundColor;

			Ray ray;

			std::uniform_real_distribution<float> dis(0.0f, 1.0f);

			// Setting the ray origin
			// viewer is in (0, 0, -200)
			ray.origin[0] = viewer_pos[0] + r - IMG_WIDTH  / 2.0f;
			ray.origin[1] = viewer_pos[1] + c - IMG_HEIGHT / 2.0f;
			ray.origin[2] = viewer_pos[2];

			// Specify the distance where the intersection can happen
			ray.t_min = 0.0f;
			ray.t_max = std::numeric_limits<float>::max();

			// Setting the ray direction -> in z-direction
			ray.dir[0] = 0.f;
			ray.dir[1] = 0.f;
			ray.dir[2] = 1.f;

			// trace rays to compute the pixel color
			if(intersectSphere(ray, sphere)){
				//pixelColor = RGBColor(1.f, 0.f, 0.f, 1.f);
				//pixelColor = RGBColor(ray.t_max, ray.t_max, ray.t_max, 1);
				//pixelColor = RGBColor((ray.normal[0] + 1.f) / 2.f, (ray.normal[1] + 1.f) / 2.f, (ray.normal[2] + 1.f) / 2.f, 1);


				float diffuse_albedo = dot(light_dir, ray.normal);

				//pixelColor = RGBColor(diffuse_albedo, diffuse_albedo, diffuse_albedo, 1);

				float proj_light_dir_on_normal_len = dot(light_dir, ray.normal);

				float intersect_position[3] = { ray.origin[0] + ray.dir[0] * ray.t_max, ray.origin[1] + ray.dir[1] * ray.t_max, ray.origin[2] + ray.dir[2] * ray.t_max };
				float reflection_vec[3]     = { 
					2 * proj_light_dir_on_normal_len * ray.normal[0] - light_dir[0], 
					2 * proj_light_dir_on_normal_len * ray.normal[1] - light_dir[1], 
					2 * proj_light_dir_on_normal_len * ray.normal[2] - light_dir[2] 
				};
				float reflection_vec_len = sqrt(reflection_vec[0] * reflection_vec[0] + reflection_vec[1] * reflection_vec[1] + reflection_vec[2] * reflection_vec[2]);
				reflection_vec[0] /= reflection_vec_len;
				reflection_vec[1] /= reflection_vec_len;
				reflection_vec[2] /= reflection_vec_len;

				float viewer_vec[3]         = { ray.origin[0] - intersect_position[0], ray.origin[1] - intersect_position[1], ray.origin[2] - intersect_position[2] };
				float viewer_vec_len = sqrt(viewer_vec[0] * viewer_vec[0] + viewer_vec[1] * viewer_vec[1] + viewer_vec[2] * viewer_vec[2]);
				viewer_vec[0] /= viewer_vec_len;
				viewer_vec[1] /= viewer_vec_len;
				viewer_vec[2] /= viewer_vec_len;

				float specular_albedo       = pow(clamp(dot(reflection_vec, viewer_vec), 0.0f, 1.0f), 20.0f);

				pixelColor = RGBColor(specular_albedo, specular_albedo, specular_albedo, 1);

				float phong_factor = clamp(diffuse_albedo + specular_albedo, 0.2f, 1.0f);

				pixelColor = RGBColor(sphere.color.r * phong_factor, sphere.color.g * phong_factor, sphere.color.b * phong_factor, 1.f);

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

	ppmFile << "P6" << std::endl;
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
