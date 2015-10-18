#include "raytracer.h"

// STD
#include <iostream>
#include <fstream>
#include <string>
#include <random>

using namespace RayTracer;

std::random_device CRayTracer::rd;
std::mt19937 CRayTracer::RandomGenerator(CRayTracer::rd());


CRayTracer::CRayTracer(void) : m_ColorBuffer(3 * IMG_WIDTH * IMG_HEIGHT), m_BackgroundColor(RGBColor::BLACK) {
}


// Renders Scene from an orthographic view
void CRayTracer::renderScene(void) {
	for(int r = 0; r < IMG_HEIGHT; r++){
		for(int c = 0; c < IMG_WIDTH; c++){
			RGBColor pixelColor = m_BackgroundColor;

			std::uniform_real_distribution<float> dis(0.0f, 1.0f);

			// trace rays to compute the pixel color

			// TODO: remove - random color generator
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

void CRayTracer::saveBitmapToFile(const std::string& _pFileName) const {
	// Save to ppm file
}

const float* CRayTracer::getColorBuffer() const {
	return m_ColorBuffer.data();
}

CRayTracer::~CRayTracer(void) {
}
