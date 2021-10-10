#include "framework.hpp"
#include "menu.hpp"

//credits to harcuz for menu framework (https://www.unknowncheats.me/forum/members/2669363.html),
POINT cursor;
POINT cursor_corrected;


void menu_framework::group_box(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string string, bool show_label) {
	//groupbox background
	render::draw_filled_rect(x, y, w, h, color(25, 25, 25, 255));

	//groupbox outline
	render::draw_rect(x, y, w, h, color(45, 45, 45, 255));

	//groupbox label
	if (show_label)
		render::text(x + 2, y - 12, font, string, false, color::white());
}

void menu_framework::tab(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string string, std::int32_t& tab, std::int32_t count) {
	GetCursorPos(&cursor);

	if ((cursor.x > x) && (cursor.x < x + w) && (cursor.y > y) && (cursor.y < y + h) && (GetAsyncKeyState(VK_LBUTTON) & 1))
		tab = count;

	//tab background
	//render::draw_filled_rect(x, y + h, w, 1, tab == count ? color(variables::menu::r, variables::menu::g, variables::menu::b, 255) : color(25, 25, 25, 255));
	//if (menu::current_tab == count) {
	//	render::draw_filled_rect(x - 1, y - 1, w + 2, h + 10, color(255, 255, 255, 255));

	//}
	//render::draw_filled_rect(x, y, w, (menu::current_tab == count ? h + 10 : h), color(45, 45, 45, 255));
	
	//render::draw_rect(x, y, w, h, color(75, 75, 75, 245));

	//tab label
	render::text(x + w / 2, y + h / 2 - 6, font, string, true, tab == count ? color(variables::menu::r, variables::menu::g, variables::menu::b, 255) : color::white());
}

void menu_framework::check_box(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, bool& value) {
	GetCursorPos(&cursor);

	int w = 10, h = 10;
	bool hoverd = false;

	if ((cursor.x > x) && (cursor.x < x + render::get_text_size(font, string).x + 5 + w) && (cursor.y > y + 2) && (cursor.y < y + (h + 2)) && GetAsyncKeyState(VK_LBUTTON) & 1)
		value = !value;

	if ((cursor.x > x) && (cursor.x < x + render::get_text_size(font, string).x + 5 + w) && (cursor.y > y + 2) && (cursor.y < y + (h + 2))) {
		hoverd = true;
	}

	if (hoverd)
	{
		render::draw_filled_rect(x, y + 2, w + render::get_text_size(font, string).x + 6, h + 2, color(46, 46, 46, 255));
	}

	//checkbox background
	if (value) {
		render::draw_filled_rect(x + render::get_text_size(font, string).x + 5, y + 3, w, h, color(36, 36, 36, 255));
		render::draw_filled_rect(x + render::get_text_size(font, string).x + 5, y + 3, w, h, color(variables::menu::r, variables::menu::g, variables::menu::b, 255));
	}
	else {
		render::draw_filled_rect(x + render::get_text_size(font, string).x + 5, y + 3, w, h, color(36, 36, 36, 255));
	}
	//checkbox label
	render::text(x + 2, y + 2, font, string, false, color::white());
}

void menu_framework::slider(std::int32_t x, std::int32_t y, unsigned long font, const std::string string, float& value, float min_value, float max_value) {
	GetCursorPos(&cursor);


	int ix = x + render::get_text_size(font, string).x + 7;
	int position = render::get_text_size(font, string).x + 47;
	int yi = y + 4;
	bool hoverd = false;

	if ((cursor.x > ix) && (cursor.x < ix + position + 1) && (cursor.y > yi - 4) && (cursor.y < yi + 10) && (GetAsyncKeyState(VK_LBUTTON)))
		value = (cursor.x - ix) / (float(position) / float(max_value));

	if ((cursor.x > ix) && (cursor.x < ix + position + 1) && (cursor.y > yi - 4) && (cursor.y < yi + 10)) {
		hoverd = true;
	}

	//slider background
	render::draw_filled_rect(ix, yi, position, 6, hoverd ? color(66, 66, 66, 255) : color(36, 36, 36, 255));
	render::draw_filled_rect(ix, yi, value * (float(position) / float(max_value)), 6, color(variables::menu::r, variables::menu::g, variables::menu::b, 255));

	//slider label
	render::text(x + 2, y + 2, font, (std::stringstream{ } << string << " ").str(), false, color::white());
	render::text((value * (float(position) / float(max_value))) + ix, y + 8, font, (std::stringstream{ } << value).str(), false, color::white());
}

void menu_framework::sliderInt(std::int32_t x, std::int32_t y, unsigned long font, const std::string string, int& value, int min_value, int max_value) {
	GetCursorPos(&cursor);


	int ix = x + render::get_text_size(font, string).x + 7;
	int position = render::get_text_size(font, string).x + 90;
	int yi = y + 4;
	bool hoverd = false;

	if ((cursor.x > ix) && (cursor.x < ix + position + 1) && (cursor.y > yi - 4) && (cursor.y < yi + 10) && (GetAsyncKeyState(VK_LBUTTON))) {
		float percent = (float)((value - min_value) / (max_value - min_value));
		double mousePercent = (cursor.x - ix) / float(position),
			newValue = (mousePercent * (max_value - min_value)) + min_value;
		//value = (cursor.x - ix) / (float(position) / float(max_value));

		value = newValue;
	}

	if ((cursor.x > ix) && (cursor.x < ix + position + 1) && (cursor.y > yi - 4) && (cursor.y < yi + 10)) {
		hoverd = true;
	}

	//slider background
	render::draw_filled_rect(ix, yi, position, 6, hoverd ? color(66, 66, 66, 255) : color(36, 36, 36, 255));
	render::draw_filled_rect(ix, yi, value * (float(position) / float(max_value)), 6, color(variables::menu::r, variables::menu::g, variables::menu::b, 255));

	//slider label
	render::text(x + 2, y + 2, font, (std::stringstream{ } << string << " ").str(), false, color::white());
	render::text((value * (float(position) / float(max_value))) + ix, y + 8, font, (std::stringstream{ } << value).str(), false, color::white());
}

void menu_framework::combo_box(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string name, std::string values...) {
	GetCursorPos(&cursor);

	//TODO

}

void menu_framework::menu_movement(std::int32_t& x, std::int32_t& y, std::int32_t w, std::int32_t h) {
	GetCursorPos(&cursor);

	if (GetAsyncKeyState(VK_LBUTTON) < 0 && (cursor.x > x && cursor.x < x + w && cursor.y > y && cursor.y < y + h)) {
		should_drag = true;

		if (!should_move) {
			cursor_corrected.x = cursor.x - x;
			cursor_corrected.y = cursor.y - y;
			should_move = true;
		}
	}

	/*
	if (GetAsyncKeyState(VK_LBUTTON) < 0 && (cursor.x > x + w - 5 && cursor.x < x + w && cursor.y > y + h - 5 && cursor.y < y + h) && h > 300 && h < 1000 && w > 300 && w < 800) {
		should_drag = true;

		if (!should_move) {
			cursor_corrected.x = cursor.x - x;
			cursor_corrected.y = cursor.y - y;
			should_move = true;
		}
	}
	*/
	if (should_drag) {
		x = cursor.x - cursor_corrected.x;
		y = cursor.y - cursor_corrected.y;
	}

	if (GetAsyncKeyState(VK_LBUTTON) == 0) {
		should_drag = false;
		should_move = false;
	}

	

}

