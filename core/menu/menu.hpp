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

	c_category_btn combat_btn = { current_tab, 0, "Combat" };
	c_category_btn visuals_btn = { current_tab, 1, "Visuals" };
	c_category_btn movement_btn = { current_tab, 2, "Movement" };
	c_category_btn misc_btn = { current_tab, 3, "Misc" };
	std::set<c_category_btn*> categories = { &combat_btn, &visuals_btn, &movement_btn, &misc_btn };
};

