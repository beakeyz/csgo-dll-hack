#include "menu.hpp"
#include "../features/features.hpp"
#include "./components/button_component.h"
#include "./components/group_box.h"
#include "../features/hacks/ragebot/ragebot.h"
#include "../features/hacks/render/player_esp.h"
#include "../features/hacks/render/visuals.h"
#include "./utils/gui_utils.h"

void c_menu::render() {
	if (!this->is_open) {
		return;
	}

	GetCursorPos(&cursor);

	
	if (GetAsyncKeyState(VK_LBUTTON) && !is_mouse) {
		if ((cursor.x > x) && (cursor.x < x + w) && (cursor.y > y) && (cursor.y < y + h)) {
			this->mouseClicked(cursor.x, cursor.y, VK_LBUTTON);
		}
		is_mouse = true;
	}
	else if (!GetAsyncKeyState(VK_LBUTTON) && is_mouse) {
		this->mouseReleased();
		is_mouse = false;

		//DEBUG
		//render::text(x + 10, y + 16, render::fonts::watermark_font, "dfadfa", false, color::white());
	}

	if (dragging) {
		x = cursor.x - x2;
		y = cursor.y - y2;
	}

	//std::string prev_name = "";

	//render::draw_filled_rect(x - 1, y - 1, w + 2, h + 2, color(variables::menu::r, variables::menu::g, variables::menu::b, 255));
	render::draw_rounded_rect(x, y, w, h, 25, color(36, 36, 36, 255), true, false, true, false);
	render::draw_filled_rect(x - 1, y - 1, w + 2, 72, color::white());
	render::draw_filled_rect(x, y, w, 70, color(31, 31, 31, 255));
	
	//render::draw_filled_rect(x, y + 70, w, 2, color(36, 36, 36, 255));

	render::text(x + 10, y + 8, render::fonts::watermark_font, "BeakonCS - B1.1", false, color::white());

	for (c_category_btn* btn : this->categories) {
		
		btn->draw("");

		//prev_name.append(btn->m_name + " ");
	}
}

void c_menu::toggle() {

	if (GetAsyncKeyState(VK_INSERT) & 1 || ((GetAsyncKeyState(VK_ESCAPE) & 1) && is_open))
	{
		this->is_open = !this->is_open;
	}
}

void c_menu::mouseClicked(int mouseX, int mouseY, int btn) {

	if (btn == VK_LBUTTON) {

		clicking = true;

		if (mouseX > x && mouseX < x + w && mouseY > y && mouseY < y + 70) {
			dragging = true;
		}

		for (c_category_btn* c_btn : this->categories) {

			if (mouseX > this->x + c_btn->x && mouseX < this->x + c_btn->x + c_btn->w && mouseY > this->y + c_btn->y && mouseY < this->y + c_btn->y + c_btn->h) {
				c_btn->mouseClicked(mouseX, mouseY, btn);
				dragging = false;
				break;
			}
		}

		if (dragging) {
			x2 = mouseX - x;
			y2 = mouseY - y;
		}
	}
}

void c_menu::renderTextInMenu(std::int32_t x, std::int32_t y, unsigned long font, std::string text, bool centered, color color)
{
	render::text(this->x + x, this->y + y, font, text, centered, color);
}
void c_menu::renderRectInMenu(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color)
{
	render::draw_rect(this->x + x, this->y + y, width, height, color);
}
void c_menu::renderFilledRectInMenu(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color)
{
	render::draw_filled_rect(this->x + x, this->y + y, width, height, color);
}
void c_menu::renderCircleInMenu(std::int32_t x, std::int32_t y, std::int32_t radius, std::int32_t segments, color color)
{
	render::draw_circle(this->x + x, this->y + y, radius, segments, color);
}
void c_menu::renderRoundedRectInMenu(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, std::int32_t r, color color, bool left_under, bool left_up, bool right_under, bool right_up)
{
	render::draw_rounded_rect(this->x + x, this->y + y, width, height, r, color, left_under, left_up, right_under, right_up);
}


void c_menu::mouseReleased() {
	clicking = false;
	dragging = false;
}
