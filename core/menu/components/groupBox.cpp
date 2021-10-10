#include "group_box.h"
#include "../menu.hpp"

groupBox::groupBox(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, std::string string, e_boxtype type) {
	this->x = x;
	this->x2 = x;

	this->y = y;
	this->y2 = y;

	this->w = w;
	this->h = h;
	this->font = font;
	this->string = string;

	this->m_comps = {};
	this->m_boxtype = type;
}

void groupBox::draw() {

	this->x = c_menu::get_ptr()->x + this->x2;
	this->y = c_menu::get_ptr()->y + this->y2;

	render::draw_filled_rect(x, y, w, h, color(25, 25, 25, 255));

	//groupbox outline
	render::draw_rect(x, y, w, h, color(45, 45, 45, 255));

	render::draw_rect(x - 1, y - 1, w + 2, h + 2, color::black(150));

	//groupbox label
	render::text(x + 6, y - 8, font, string, false, color::white());

}

std::unordered_map<int, comp*> groupBox::get_comps() {
	return this->m_comps;
}
