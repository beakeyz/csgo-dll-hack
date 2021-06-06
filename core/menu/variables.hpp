#pragma once
#include <iostream>
#include <math.h>

/*
class Panel {

public:
	int x;
	int y;
	int w;
	int h;
	std::string name;
	Panel(int x, int y, int w, int h)
	{
		this->w = w;
		this->h = h;
		this->x = x;
		this->y = y;
	}

	void setX(int _x)
	{
		x = _x;
	}

	void setY(int _y)
	{
		y = _y;
	}
	void setW(int _w)
	{
		w = _w;
	}
	void setH(int _h)
	{
		h = _h;
	}

	void move()
	{
		GetCursorPos(&cursor);

		if (GetAsyncKeyState(VK_LBUTTON) < 0 && (cursor.x > x && cursor.x < x + w && cursor.y > y && cursor.y < y + h)) {
			should_drag = true;

			if (!should_move) {
				x2 = cursor.x - x;
				y2 = cursor.y - y;
				should_move = true;
			}
		}

		if (should_drag) {
			this->x = cursor.x - x2;
			this->y = cursor.y - y2;
		}

		if (GetAsyncKeyState(VK_LBUTTON) == 0) {
			should_drag = false;
			should_move = false;
		}
	}


private:
	POINT cursor;
	int x2;
	int y2;
	bool should_drag = false;
	bool should_move = false;
};
*/


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

	namespace Util {


		/*
		inline std::vector<vec3_t> rainbowColor(float speed)
		{
			constexpr float pi = 3.145F;
			std::vector<vec3_t> t;
			t.reserve(1);
			t.emplace_back(vec3_t(std::sin(speed * interfaces::globals->cur_time) * 0.5f + 0.5f,
				std::sin(speed * interfaces::globals->cur_time + 2 * pi / 3) * 0.5f + 0.5f,
				std::sin(speed * interfaces::globals->cur_time + 4 * pi / 3) * 0.5f + 0.5f));
			return t;
		}
		*/
	}
}