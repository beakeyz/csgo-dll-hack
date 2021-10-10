#pragma once
#include <iostream>
#include <math.h>

namespace variables {
	
	namespace menu {
		//inline std::vector<Panel> panels;
		//inline bool openedForFirstTime = false;

		inline bool opened = false;

		inline int x = 840, y = 140;
		inline int w = 600, h = 700;

		inline const int component_height = 15;

		inline int r = 255;
		inline int g = 255;
		inline int b = 255;
		inline float speed = 10;
		inline bool rainbow = false;
	}
}