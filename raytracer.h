#pragma once

// STD
#include <vector>
#include <string>
#include <random>

#include "defines.h"

namespace RayTracer
{
	class CRayTracer
	{
	private:
		RGBColor m_BackgroundColor;							//!< Background color
		std::vector<float> m_ColorBuffer;					//!< Color buffer
		
		// Random Generator
		// NOTE: C++11 - NEVER USE rand() for random generation.
		static std::random_device rd;
		static std::mt19937 RandomGenerator;

	public:
		CRayTracer(void);
		~CRayTracer(void);

		void renderScene(void);
		void saveBitmapToFile(const std::string& filename) const;

		const float* getColorBuffer() const;		

	private:
		void drawPixel(const int& _pRow, const int& _pCol, const RGBColor& _pPixelColor);
	};
}