#pragma once
#include "./../components/button_component.h"

class c_gui_manager : public singleton<c_gui_manager> {
public:
	void draw_box(groupBox& box, groupBox* prev_box, int& total_height_left);
};