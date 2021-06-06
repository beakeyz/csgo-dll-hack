#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../menu/variables.hpp"

namespace menu_framework {
	void group_box(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string string, bool show_label);
	void tab(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string, std::int32_t& tab, std::int32_t count);
	void check_box(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, bool& value);
	void slider(std::int32_t x, std::int32_t y, unsigned long font, const std::string string, float& value, float min_value, float max_value);
	void sliderInt(std::int32_t x, std::int32_t y, unsigned long font, const std::string string, int& value, int min_value, int max_value);
	void menu_movement(std::int32_t& x, std::int32_t& y, std::int32_t w, std::int32_t h);
	//void menu_movement_panel(Panel p);
	//TODO combobox
	void combo_box(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string name, std::string values...);

	inline bool should_drag = false;
	inline bool should_move = false;
};