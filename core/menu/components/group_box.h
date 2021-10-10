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

class groupBox {

public:
	groupBox(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, std::string string, e_boxtype type);

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

	std::unordered_map<int, comp*> get_comps();

protected:
	std::int32_t x;
	std::int32_t x2;
	std::int32_t y;
	std::int32_t y2;
	std::int32_t w;
	std::int32_t h;
	unsigned long font;
	std::string string;

public:
	e_boxtype m_boxtype;
	std::unordered_map<int, comp*> m_comps = {};
};
