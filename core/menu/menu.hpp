#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "framework.hpp"
#include "variables.hpp"
#include "./components/category_btn.h"

class c_category_btn;

class c_menu : public singleton<c_menu> {

public:

	void render();
	void toggle();
	void mouseClicked(int mouseX, int mouseY, int btn);
	void mouseReleased();

	void renderTextInMenu(std::int32_t x, std::int32_t y, unsigned long font, std::string text, bool centered, color color);
	void renderRectInMenu(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color);
	void renderFilledRectInMenu(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color);
	void renderCircleInMenu(std::int32_t x, std::int32_t y, std::int32_t radius, std::int32_t segments, color color);
	void renderRoundedRectInMenu(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, std::int32_t r, color color, bool left_under, bool left_up, bool right_under, bool right_up);

	int current_tab{ 0 };
	bool is_open = false;
	int x = 840, y = 140;
	int w = 600, h = 500;
	POINT cursor;
	bool is_mouse;
	bool clicking;
	bool dragging;
	int x2;
	int y2;

	c_category_btn combat_btn = { current_tab, 0, "Combat", 25, 35, 100, 35 };
	c_category_btn visuals_btn = { current_tab, 1, "Visuals", 175, 35, 100, 35 };
	c_category_btn movement_btn = { current_tab, 2, "Movement", 325, 35, 100, 35 };
	c_category_btn misc_btn = { current_tab, 3, "Misc", 475, 35, 100, 35 };
	std::set<c_category_btn*> categories = { &combat_btn, &visuals_btn, &movement_btn, &misc_btn };
};

