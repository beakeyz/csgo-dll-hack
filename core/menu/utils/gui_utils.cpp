#include "gui_utils.h"

class groupBox;

void c_gui_manager::draw_box(groupBox& box) {

	int j = 20;
	for (auto& elm : box.get_comps()) {
		j += elm.second->get_standard_height();
	}
	box.set_height(j);
	box.draw();

	int i = 10;
	for (auto& elm : box.get_comps()) {
		elm.second->draw(i);
		i += elm.second->get_standard_height();
	}
}