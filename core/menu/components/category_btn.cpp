#include "category_btn.h"
#include "group_box.h"
#include "./../utils/gui_utils.h"
#include "./../menu.hpp"

void c_category_btn::draw(std::string previous_name)
{

	unsigned long font = render::fonts::watermark_font_other;
	render::text(c_menu::get_ptr()->x + 50 + render::get_text_size(font, previous_name).x, c_menu::get_ptr()->y + 30, font, this->m_name, true, color::white());

	if (c_menu::get_ptr()->current_tab == this->m_locale_tab) {
		for (groupBox* box : m_boxes) {
			c_gui_manager::get_ptr()->draw_box(*box);
		}
	}
}

void c_category_btn::mouseClicked(int mouseX, int mouseY, int btn)
{
	if (btn == VK_LBUTTON)
		this->m_tab = this->m_locale_tab;
}