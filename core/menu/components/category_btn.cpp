#include "category_btn.h"
#include "group_box.h"
#include "./../utils/gui_utils.h"
#include "./../menu.hpp"

void c_category_btn::draw(std::string previous_name)
{
	unsigned long font = render::fonts::watermark_font_other;
	//render::text(c_menu::get_ptr()->x + 50 + render::get_text_size(font, previous_name).x, c_menu::get_ptr()->y + 30, font, this->m_name, true, color::white());
	c_menu::get_ptr()->renderTextInMenu(this->x, this->y, render::fonts::watermark_font_small, this->m_name, false, color::white());

	if (this->m_tab == this->m_locale_tab) 
	{
		for (groupBox* box : this->m_boxes)
		{
			c_gui_manager::get_ptr()->draw_box(*box);
		}
	}
}

void c_category_btn::mouseClicked(int mouseX, int mouseY, int btn)
{
	if (btn == VK_LBUTTON) {
		//console::log("changed tab\n");
		this->m_tab = this->m_locale_tab;
	}
}