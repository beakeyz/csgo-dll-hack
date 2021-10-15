#pragma once
#include "../../features/features.hpp"
#include "button_component.h"

class comp;
class buttonComponent;
class c_slider_component;
class c_color_picker;

enum e_boxtype {
	RAGEBOT,
	CHAMS,
	VISUALS,
	FAKELAG,
	COMBAT,
	PLAYER_ESP
};

enum class e_position {
	LEFT,
	RIGHT
};

class groupBox {

public:
	groupBox(e_position pos, std::int32_t y, std::int32_t h, unsigned long font, std::string string, e_boxtype type, int index);

	~groupBox() {
		this->m_comps.clear();
	}

	void draw();

	std::int32_t getX() {
		return this->x;
	}

	std::int32_t getY() {
		return this->y;
	}

	std::int32_t getWidth() {
		return this->w;
	}

	std::int32_t getHeight() {
		return this->h;
	}

	void set_height(std::int32_t height) {
		this->h = height;
	}

	void set_posititon(e_position new_pos);

	std::unordered_map<int, comp*> get_comps();

public:
	std::int32_t x;
	std::int32_t x2;
	std::int32_t y;
	std::int32_t y2;
	std::int32_t w;
	std::int32_t h;
	unsigned long font;
	std::string string;
	e_position m_pos;
	int index;

public:
	e_boxtype m_boxtype;
	std::unordered_map<int, comp*> m_comps = {};
};
