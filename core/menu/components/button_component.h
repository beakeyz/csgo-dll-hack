#pragma once
#include <iostream>
#include "../menu.hpp"
#include "../../features/features.hpp"
#include "group_box.h"

class buttonComponent {

public:
	buttonComponent(double x, double y, groupBox* parent, std::string text, unsigned long font, bool& r_value) : value(r_value) {
		this->x = x;
		this->x2 = x;
		this->y = y;
		this->y2 = y;
		this->font = font;
		this->text = text;
		this->value = value;
		this->parent = parent;
	}

	void draw() {
		this->x = this->x2;
		this->y = this->y2;
		this->x += this->parent->getX();
		this->y += this->parent->getY();
		GetCursorPos(&cursor);

		const int w = 10, h = 10;
		bool hoverd = false;

		if ((cursor.x > x) && (cursor.x < x + 8) && (cursor.y > y) && (cursor.y < y + 8)) {
			hoverd = true;
		}

		//if (hoverd)
		//{
		//	render::draw_filled_rect(x, y + 2, w + render::get_text_size(font, text).x + 6, h + 2, color(46, 46, 46, 255));
		//}

		//checkbox background
		if (value) {
			render::draw_filled_rect(x, y, w, h, color(variables::menu::r, variables::menu::g, variables::menu::b, 255));
			render::draw_rect(x, y, w, h, color::black(255));
		}
		else {
			render::draw_filled_rect(x, y, w, h, color(45, 45, 46, 255));
			render::draw_rect(x, y, w, h, color::black(215));
		}
		
		//checkbox label
		render::text(x + w * 5, y - 2, font, text, false, color::white());

		this->toggle();
	}

	void toggle() {
		if ((cursor.x > x) && (cursor.x < x + 8) && (cursor.y > y) && (cursor.y < y + 8) && GetAsyncKeyState(VK_LBUTTON) & 1) {
			this->value = !value;
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

		}
	}

	std::int32_t getHeight() {
		return render::get_text_size(font, this->text).y;
	}

	void setY(double y) {
		this->y2 = y;
		this->y = y;
	}

	double getY() {
		return this->y;
	}

protected:
	double x;
	double y;
	double x2;
	double y2;
	std::string text;
	unsigned long font;
	bool& value;
	groupBox* parent;
	POINT cursor;

public:
	int index;
	static const int indexed_height = 12;
};

class c_slider_component {

public:
	c_slider_component(double x, double y, groupBox* parent, std::string text, unsigned long font, int& aValue, int min_value, int max_value) : value(aValue) {
		this->x = x;
		this->x2 = x;
		this->y = y;
		this->y2 = y;
		this->font = font;
		this->text = text;
		this->parent = parent;
		this->min_value = min_value;
		this->max_value = max_value;
		this->width = this->parent->getWidth() / 2;
	}

	void draw() {
		this->x = this->x2 + this->parent->getX();
		this->y = this->y2 + this->parent->getY();
		const int bar_width = this->parent->getWidth() - 30;
		const int bar_pos = this->y + render::get_text_size(this->font, this->text).y + 4;
		const int bar_height = 9;

		GetCursorPos(&cursor);

		if ((cursor.x > x) && (cursor.x < x + bar_width + 4) && (cursor.y > bar_pos) && (cursor.y < bar_pos + bar_height) && (GetAsyncKeyState(VK_LBUTTON)))
			value = (cursor.x - x) / (float(bar_width) / float(max_value));


		render::text(this->x, this->y, this->font, this->text, false, color::white());

		render::draw_filled_rect(this->x, bar_pos, bar_width, bar_height, color(12, 12, 12, 152));
		render::draw_rect(this->x, bar_pos, bar_width, bar_height, color::black(200));

		render::draw_filled_rect(this->x, bar_pos, value * (float(bar_width) / float(max_value)), bar_height, color(variables::menu::r, variables::menu::g, variables::menu::b, 255));
		render::draw_rect(this->x, bar_pos, value * (float(bar_width) / float(max_value)), bar_height, color::black(255));

		std::ostringstream value_stream;
		value_stream << value << "";

		render::text((this->x + value * (float(bar_width) / float(max_value))) - render::get_text_size(render::fonts::watermark_font_other, value_stream.str().c_str()).x / 2, bar_pos + bar_height / 3, render::fonts::watermark_font_other, value_stream.str().c_str(), false, color::white());
	}

	void setY(double y) {
		this->y2 = y;
		this->y = y;
	}

	double getY() {
		return this->y;
	}

protected:
	double x;
	double y;
	double x2;
	double y2;
	double width;
	std::string text;
	unsigned long font;
	int& value;
	int min_value;
	int max_value;
	groupBox* parent;
	POINT cursor;

public:
	int index;
	static const int indexed_height = 28;
};

class c_color_picker {

public:

	c_color_picker(double x, double y, groupBox* parent, std::string text, unsigned long font, color& aValue) : value(aValue) {
		this->x = x;
		this->x2 = x;
		this->y = y;
		this->y2 = y;
		this->font = font;
		this->text = text;
		this->parent = parent;	
	}

	void draw() {
		this->x = this->x2 + this->parent->getX();
		this->y = this->y2 + this->parent->getY();
		GetCursorPos(&cursor);
		//assert(&cursor != nullptr);
		const int width = this->parent->getWidth() - 45;

		render::text(this->x, this->y, this->font, this->text, false, color::white());

		render::draw_filled_rect(this->x + width, this->y, 20, 10, value);
		render::draw_rect(this->x + width, this->y, 20, 10, color::black(190));

		if (isExtended) {
			render::draw_filled_rect(this->x + width, this->y + 12, 50, 50, value);

			render::draw_rect(this->x + width, this->y + 12, 50, 50, color::black(190));
		}

		if ((cursor.x > this->x + width && cursor.x < this->x + width + 20 && cursor.y > this->y && cursor.y < this->y + 10) && (GetAsyncKeyState(VK_LBUTTON) & 1)) {
			this->isExtended = !isExtended;
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

protected:
	double x;
	double y;
	double x2;
	double y2;
	std::string text;
	unsigned long font;
	color value;
	groupBox* parent;
	POINT cursor;

public:
	bool isExtended = false;
};


class c_mode_component {

};
