#include "./button_component.h"

class comp;
class groupBox;

//BUTTONCOMPONENT section
buttonComponent::buttonComponent(groupBox* parent, std::string text, unsigned long font, bool& r_value, int index) : value(r_value) {
	this->font = font;
	this->text = text;
	this->value = value;
	this->parent = parent;
	this->index = index;

	this->parent->m_comps[this->index] = this;

	
}

void buttonComponent::draw(int index) {
	this->x = 15;
	this->y = index + this->parent->getY() + 2;
	this->x += this->parent->getX();
	//this->y += this->parent->getY();
	GetCursorPos(&cursor);

	if (GetAsyncKeyState(VK_LBUTTON) && !is_mouse) {
		if ((cursor.x > x) && (cursor.x < x + 10) && (cursor.y > y) && (cursor.y < y + 10)) {
			this->mouse_clicked(cursor.x, cursor.y, VK_LBUTTON);
		}
		is_mouse = true;
	}
	else if (!GetAsyncKeyState(VK_LBUTTON)) {
		this->mouse_released();
		is_mouse = false;
	}

	const int w = 10, h = 10;

	//checkbox background
	if (value) {
		render::draw_filled_rect(x, y, w, h, color(variables::menu::r, variables::menu::g, variables::menu::b, 255));
		render::draw_rect(x, y, w, h, color::black(255));
	}
	else {
		render::draw_filled_rect(x, y, w, h, color(45, 45, 46, 255));
		render::draw_rect(x, y, w, h, color::black(215));
	}

	//checkbox label

	render::text(x + w * 5, y - 2, font, text, false, color::white());

	//if ((cursor.x > x) && (cursor.x < x + 8) && (cursor.y > y) && (cursor.y < y + 8)) {

	//}
}

void buttonComponent::mouse_clicked(long mouse_x, long mouse_y, int mouse_button) {

	if (mouse_button == VK_LBUTTON) {
		this->toggle(true);

		clicking = true;
	}
}

void buttonComponent::mouse_released() {
	clicking = false;
}

void buttonComponent::toggle(bool hoverd) {

	this->value = !this->value;
	//if (m_callback != nullptr && this->value == true) {
	//	m_callback();
	//}
	
	 
	//TODO: test why toggeling doesn't actually do anything
	console::log("toggled");
	std::this_thread::sleep_for(std::chrono::milliseconds(1));

}

std::int32_t buttonComponent::getHeight() {
	return render::get_text_size(font, this->text).y;
}

//SLIDERCOMPONENT SECTION
c_slider_component::c_slider_component(groupBox* parent, std::string text, unsigned long font, int& aValue, int min_value, int max_value, int index) : value(aValue) {
	this->font = font;
	this->text = text;
	this->parent = parent;
	this->min_value = min_value;
	this->max_value = max_value;
	this->width = this->parent->getWidth() / 2;
	this->dragging = false;
	this->is_mouse = false;
	this->index = index;


	this->parent->m_comps[this->index] = this;
	//this->draw(0);
}

void c_slider_component::draw(int index) {

	this->x = this->get_horiz_padding() + this->parent->getX();
	this->y = index + this->parent->getY() + 2;
	const int bar_width = this->parent->getWidth() - 30;
	const int bar_pos = this->y + render::get_text_size(this->font, this->text).y + 1;
	const int bar_height = 9;

	GetCursorPos(&cursor);


	if (GetAsyncKeyState(VK_LBUTTON) && !is_mouse) {
		if ((cursor.x > x) && (cursor.x < x + bar_width + 4) && (cursor.y > bar_pos) && (cursor.y < bar_pos + bar_height)) {
			this->mouse_clicked(cursor.x, cursor.y, VK_LBUTTON);
		}
		is_mouse = true;
	}
	else if (!GetAsyncKeyState(VK_LBUTTON)) {
		this->mouse_released();
		is_mouse = false;
	}


	if (dragging && (cursor.x > x) && (cursor.x < x + bar_width + 4)) {
		value = (cursor.x - x) / (float(bar_width) / float(max_value));
	}

	render::text(this->x, this->y, this->font, this->text, false, color::white());

	render::draw_filled_rect(this->x, bar_pos, bar_width, bar_height, color(12, 12, 12, 152));
	render::draw_rect(this->x, bar_pos, bar_width, bar_height, color::black(250));

	render::draw_filled_rect(this->x, bar_pos, value * (float(bar_width) / float(max_value)), bar_height, color(variables::menu::r, variables::menu::g, variables::menu::b, 255));
	render::draw_rect(this->x, bar_pos, value * (float(bar_width) / float(max_value)), bar_height, color::black(255));

	std::ostringstream value_stream;
	value_stream << value << "";

	render::text((this->x + value * (float(bar_width) / float(max_value))) - render::get_text_size(render::fonts::watermark_font_other, value_stream.str().c_str()).x / 2, bar_pos + bar_height / 3, render::fonts::watermark_font_other, value_stream.str().c_str(), false, color::white());
}

void c_slider_component::mouse_clicked(long mouse_x, long mouse_y, int mouse_button) {

	if (mouse_button == VK_LBUTTON) {
		dragging = true;
	}
}

void c_slider_component::mouse_released() {
	dragging = false;
}

//COLORPICKER section

c_color_picker::c_color_picker(groupBox* parent, std::string text, unsigned long font, color& aValue, int index) : value(aValue) {
	this->font = font;
	this->text = text;
	this->parent = parent;
	this->index = index;

	//this->draw(0);

	this->parent->m_comps[this->index] = this;
}

void c_color_picker::draw(int index) {
	this->x = this->get_horiz_padding() + this->parent->getX();
	this->y = index + this->parent->getY() + 2;
	GetCursorPos(&cursor);
	//assert(&cursor != nullptr);
	const int width = this->parent->getWidth() - 45;

	render::text(this->x, this->y, this->font, this->text, false, color::white());

	render::draw_filled_rect(this->x + width, this->y, 20, 10, color(value.r, value.g, value.b));
	render::draw_rect(this->x + width, this->y, 20, 10, color::black(220));

	if (this->isExtended) {
		render::draw_filled_rect(this->x + width, this->y + 12, 50, 50, value);

		render::draw_rect(this->x + width, this->y + 12, 50, 50, color::black(220));
	}

	if (cursor.x > this->x + width && cursor.x < this->x + width + 20 && cursor.y > this->y && cursor.y < this->y + 10 && GetAsyncKeyState(VK_LBUTTON)) {
		this->isExtended = !this->isExtended;
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
}
