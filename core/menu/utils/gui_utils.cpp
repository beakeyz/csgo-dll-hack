#include "gui_utils.h"
#include "./../menu.hpp"
class groupBox;

void c_gui_manager::draw_box(groupBox& box, groupBox* prev_box, int& total_height) {

	int j = 20;
	for (auto& elm : box.get_comps()) {
		j += elm.second->get_standard_height();
	}
	box.set_height(j);

	if (prev_box == nullptr) {
		
		total_height = j;

		box.draw();

		int i = 10;
		for (auto& elm : box.get_comps()) {
			if (elm.second->classified_index != box.current_classifier && elm.second->is_classified) {
				continue;
			}

			elm.second->draw(i);
			i += elm.second->get_standard_height();
		}
		return;
	}

	total_height += box.h + 20;

	if (total_height <= c_menu::get_ptr()->h - 80) {
		box.y2 = prev_box->y2 + prev_box->h + 20;
		box.y = prev_box->y + prev_box->h + 20;

		if (prev_box->m_pos == e_position::RIGHT) box.set_posititon(e_position::RIGHT);

		box.draw();

		int ji = 10;
		for (auto& elm : box.get_comps()) {
			if (elm.second->classified_index != box.current_classifier && elm.second->is_classified) {
				continue;
			}

			elm.second->draw(ji);
			ji += elm.second->get_standard_height();
		}
	}
	else {

		box.set_posititon(e_position::RIGHT);

		this->draw_box(box, nullptr, total_height);

		return;
	}
}