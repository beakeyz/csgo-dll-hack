#include "category_btn.h"
#include "group_box.h"
#include "./../utils/gui_utils.h"
#include "./../menu.hpp"

void c_category_btn::draw(std::string previous_name)
{
	unsigned long font = render::fonts::watermark_font_small;
	
	
	if (this->m_tab == this->m_locale_tab) 
	{
		c_menu::get_ptr()->renderFilledRectInMenu(this->x - 2, this->y - 2, this->w + 4, this->h + 2, color::white(25));
		c_menu::get_ptr()->renderFilledRectInMenu(this->x - 1, this->y - 1, this->w + 2, this->h + 1, color::white(255));
		c_menu::get_ptr()->renderFilledRectInMenu(this->x, this->y, this->w, this->h + 1, color(36, 36, 36, 255));
		
		//c_menu::get_ptr()->renderRoundedRectInMenu(this->x - 2, this->y - 2, this->w + 4, this->h, 10, color::white(255), false, true, false, true);
		//c_menu::get_ptr()->renderRoundedRectInMenu(this->x, this->y, this->w, this->h - 1, 10, color(36, 36, 36, 255), false, true, false, true);

		groupBox* prev_box = nullptr;
		int total_height_left = 0;

		for (auto& box : this->m_boxes)
		{
			c_gui_manager::get_ptr()->draw_box(*box.second, prev_box, total_height_left);
			prev_box = box.second;
		}
	}
	else {
		
		//c_menu::get_ptr()->renderCircleInMenu(this->x + 10, this->y + 10, 10, 180, color(25, 25, 25, 255));
		//c_menu::get_ptr()->renderCircleInMenu(this->x + this->w - 10, this->y + 10, 10, 180, color(25, 25, 25, 255));
		//c_menu::get_ptr()->renderFilledRectInMenu(this->x, this->y + 10, this->w, this->h - 11, color(25, 25, 25, 255));
		c_menu::get_ptr()->renderFilledRectInMenu(this->x, this->y, this->w, this->h, color(25, 25, 25, 255));
		c_menu::get_ptr()->renderRectInMenu(this->x, this->y, this->w, this->h, color(60, 60, 60, 255));
		//c_menu::get_ptr()->renderRoundedRectInMenu(this->x, this->y, this->w, this->h - 1, 10, color(34, 34, 34, 255), false, true, false, true);

	}
	c_menu::get_ptr()->renderTextInMenu((this->x + this->w / 2) - render::get_text_size(font, this->m_name).x / 2,
		(this->y + (this->h / 2)) - render::get_text_size(font, this->m_name).y / 2,
		font,
		this->m_name,
		false,
		color::white()
	);

}

void c_category_btn::mouseClicked(int mouseX, int mouseY, int btn)
{
	if (btn == VK_LBUTTON) {
		//console::log("changed tab\n");
		this->m_tab = this->m_locale_tab;
	}
}