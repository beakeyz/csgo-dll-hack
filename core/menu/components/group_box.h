#pragma once
#include "../menu.hpp"
#include "../../features/features.hpp"
#include "button_component.h"

class comp;

class groupBox {

public:
	groupBox(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, std::string string) {
		this->x = x;
		this->x2 = x;

		this->y = y;
		this->y2 = y;

		this->w = w;
		this->h = h;
		this->font = font;
		this->string = string;

		this->m_comps = {};

	}

	~groupBox() {
		this->m_comps.clear();
	}

	void draw() {

		this->x = variables::menu::x + this->x2;
		this->y = variables::menu::y + this->y2;

		render::draw_filled_rect(x, y, w, h, color(25, 25, 25, 255));

		//groupbox outline
		render::draw_rect(x, y, w, h, color(45, 45, 45, 255));

		render::draw_rect(x - 1, y - 1, w + 2, h + 2, color::black(150));

		//groupbox label
		render::text(x + 6, y - 8, font, string, false, color::white());

	}

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

	std::set<comp*> get_comps() {
		return this->m_comps;
	}

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
	std::set<comp*> m_comps = {};
};
