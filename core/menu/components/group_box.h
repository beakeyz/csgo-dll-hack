#pragma once
#include "../menu.hpp"
#include "../../features/features.hpp"

class groupBox {

public:
	groupBox(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, std::string string) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
		this->font = font;
		this->string = string;
	}

	void draw() {

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

protected:
	std::int32_t x;
	std::int32_t y;
	std::int32_t w;
	std::int32_t h;
	unsigned long font;
	std::string string;

};