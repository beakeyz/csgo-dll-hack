#include "./button_component.h"

class comp;
class groupBox;

c_button_classifier::c_button_classifier(groupBox *parent, std::vector<sub_classifier> btns, std::string text, unsigned long font, int index)
{
	this->parent = parent;
	this->font = font;
	this->text = text;
	this->m_btns = btns;
	this->index = index;
	this->m_current_classified_index = 0;
	parent->m_comps[this->index] = this;
	parent->m_current_button_classifier = this;
}

void c_button_classifier::draw(int index)
{
	parent->current_classifier = this->m_current_classified_index;
	this->x = 15;
	this->y = index + this->parent->getY() + 2;
	this->x += this->parent->getX();

	render::draw_filled_rect(this->x, this->y, parent->getWidth() - 30, this->h, color::black());

	sub_classifier previous_btn = {this->x, this->y, 0, this->h, -1, ""};
	for (auto &btn : this->m_btns)
	{
		GetCursorPos(&cursor);
		btn.y = this->y;
		btn.h = this->h;
		btn.w = (parent->getWidth() - 30) / this->m_btns.size();
		if (previous_btn.index != -1)
		{
			btn.x = previous_btn.x + previous_btn.w;
		}
		else
		{
			btn.x = this->x;
		}

		//DEBUG
		//std::stringstream s;
		//s << btn.classified_components.size();
		//render::text(btn.x, btn.y, render::fonts::watermark_font_small, s.str(), false, color::white());

		previous_btn = btn;

		if (this->parent->current_classifier == btn.index)
		{
			render::gradient(btn.x, btn.y, btn.w, btn.h, color(100, 100, 100, 205), color(70, 70, 70, 205), render::GradientType::GRADIENT_VERTICAL);
			render::draw_rect(btn.x, btn.y, btn.w + 1, btn.h, color(100, 100, 100, 205));
			//color lmao
			render::draw_line(btn.x + 1, btn.y + btn.h - 2, btn.x + btn.w - 1, btn.y + btn.h - 2, color::white(255));
		}
		else
		{
			render::gradient(btn.x, btn.y, btn.w, btn.h, color(70, 70, 70, 205), color(30, 30, 30, 155), render::GradientType::GRADIENT_VERTICAL);
			render::draw_rect(btn.x, btn.y, btn.w + 1, btn.h, color(100, 100, 100, 205));
		}

		render::text(btn.x - render::get_text_size(font, btn.text).x / 2 + btn.w / 2,
					 btn.y - render::get_text_size(font, btn.text).y / 2 + btn.h / 2,
					 font,
					 btn.text,
					 false,
					 color::white());
	}

	if (GetAsyncKeyState(VK_LBUTTON) && !is_mouse)
	{
		if ((cursor.x > x) && (cursor.x < x + (parent->getWidth() - 30)) && (cursor.y > y) && (cursor.y < y + h))
		{
			this->mouse_clicked(cursor.x, cursor.y, VK_LBUTTON);
		}
		is_mouse = true;
	}
	else if (!GetAsyncKeyState(VK_LBUTTON) && is_mouse)
	{
		this->mouse_released();
		is_mouse = false;
	}
}

void c_button_classifier::mouse_clicked(long mouse_x, long mouse_y, int mouse_button)
{

	for (auto btn : this->m_btns)
	{
		if (mouse_x > btn.x && mouse_x < btn.x + btn.w)
		{
			this->m_current_classified_index = btn.index;
			break;
		}
	}
}

void c_button_classifier::mouse_released()
{
	this->clicking = false;
}

//BUTTONCOMPONENT section
buttonComponent::buttonComponent(groupBox *parent, int classified_index, std::string text, unsigned long font, bool &r_value, int index) : value(r_value)
{
	this->font = font;
	this->text = text;
	this->value = value;
	this->parent = parent;
	this->index = index;

	this->parent->m_comps[this->index] = this;

	if (classified_index != -1)
	{
		this->is_classified = true;
		this->classified_index = classified_index;
		if (this->parent->m_current_button_classifier != nullptr)
		{
			this->parent->m_current_button_classifier->m_btns[classified_index].classified_components[this->parent->m_current_button_classifier->m_btns[classified_index].classified_count++] = this;
		}
	}
	this->integrated_comp = nullptr;
}

buttonComponent::buttonComponent(groupBox *parent, int classified_index, std::string text, unsigned long font, bool &r_value, int index, comp *integrated_comp) : value(r_value)
{
	this->font = font;
	this->text = text;
	this->value = value;
	this->parent = parent;
	this->index = index;

	this->parent->m_comps[this->index] = this;

	if (classified_index != -1)
	{
		this->is_classified = true;
		this->classified_index = classified_index;
		if (this->parent->m_current_button_classifier != nullptr)
		{
			this->parent->m_current_button_classifier->m_btns[classified_index].classified_components[this->parent->m_current_button_classifier->m_btns[classified_index].classified_count] = this;
			this->parent->m_current_button_classifier->m_btns[classified_index].classified_count++;
		}
	}

	integrated_comp->is_integrated = true;
	this->integrated_comp = integrated_comp;
}

void buttonComponent::draw(int index)
{
	this->x = 15;
	this->y = index + this->parent->getY() + 2;
	this->x += this->parent->getX();
	//this->y += this->parent->getY();
	GetCursorPos(&cursor);

	if (GetAsyncKeyState(VK_LBUTTON) && !is_mouse)
	{
		if ((cursor.x > x) && (cursor.x < x + 10) && (cursor.y > y) && (cursor.y < y + 10))
		{
			this->mouse_clicked(cursor.x, cursor.y, VK_LBUTTON);
		}
		is_mouse = true;
	}
	else if (!GetAsyncKeyState(VK_LBUTTON))
	{
		this->mouse_released();
		is_mouse = false;
	}

	const int w = 10, h = 10;

	//checkbox background
	if (value)
	{
		render::draw_filled_rect(x, y, w, h, color(variables::menu::r, variables::menu::g, variables::menu::b, 255));
		render::draw_rect(x, y, w, h, color::black(255));
	}
	else
	{
		render::draw_filled_rect(x, y, w, h, color(45, 45, 46, 255));
		render::draw_rect(x, y, w, h, color::black(215));
	}

	//checkbox label

	render::text(x + w * 5, y - 2, font, text, false, color::white());

	//if ((cursor.x > x) && (cursor.x < x + 8) && (cursor.y > y) && (cursor.y < y + 8)) {

	//}
}

void buttonComponent::mouse_clicked(long mouse_x, long mouse_y, int mouse_button)
{

	if (mouse_button == VK_LBUTTON)
	{
		this->toggle(true);

		clicking = true;
	}
}

void buttonComponent::mouse_released()
{
	clicking = false;
}

void buttonComponent::toggle(bool hoverd)
{

	this->value = !this->value;
	//if (m_callback != nullptr && this->value == true) {
	//	m_callback();
	//}

	//TODO: test why toggeling doesn't actually do anything
	console::log("toggled");
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

std::int32_t buttonComponent::getHeight()
{
	return render::get_text_size(font, this->text).y;
}

//SLIDERCOMPONENT SECTION
c_slider_component::c_slider_component(groupBox *parent, int classified_index, std::string text, unsigned long font, int &aValue, int min_value, int max_value, int index) : value(aValue)
{
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

	if (classified_index != -1)
	{
		this->is_classified = true;
		this->classified_index = classified_index;
		if (this->parent->m_current_button_classifier != nullptr)
		{
			this->parent->m_current_button_classifier->m_btns[classified_index].classified_components[this->parent->m_current_button_classifier->m_btns[classified_index].classified_count] = this;
			this->parent->m_current_button_classifier->m_btns[classified_index].classified_count++;
		}
	}
}

void c_slider_component::draw(int index)
{
	this->x = this->get_horiz_padding() + this->parent->getX();
	this->y = index + this->parent->getY() + 2;
	const int bar_width = this->parent->getWidth() - 30;
	const int bar_pos = this->y + render::get_text_size(this->font, this->text).y + 1;
	const int bar_height = 9;

	GetCursorPos(&cursor);

	if (GetAsyncKeyState(VK_LBUTTON) && !is_mouse)
	{
		if ((cursor.x > x) && (cursor.x < x + bar_width + 4) && (cursor.y > bar_pos) && (cursor.y < bar_pos + bar_height))
		{
			this->mouse_clicked(cursor.x, cursor.y, VK_LBUTTON);
		}
		is_mouse = true;
	}
	else if (!GetAsyncKeyState(VK_LBUTTON))
	{
		this->mouse_released();
		is_mouse = false;
	}

	if (dragging && (cursor.x > x) && (cursor.x < x + bar_width + 4))
	{
		value = (cursor.x - x) / (float(bar_width) / float(max_value));
		if (value > max_value)
			value = max_value;
		if (value < min_value)
			value = min_value;
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

void c_slider_component::mouse_clicked(long mouse_x, long mouse_y, int mouse_button)
{

	if (mouse_button == VK_LBUTTON)
	{
		dragging = true;
	}
}

void c_slider_component::mouse_released()
{
	dragging = false;
}

//COLORPICKER section

c_color_picker::c_color_picker(groupBox *parent, int classified_index, std::string text, unsigned long font, color &aValue, int index) : value(aValue)
{
	this->font = font;
	this->text = text;
	this->parent = parent;
	this->index = index;

	//this->draw(0);

	this->parent->m_comps[this->index] = this;

	if (classified_index != -1)
	{
		this->is_classified = true;
		this->classified_index = classified_index;
	}
}

void c_color_picker::draw(int index)
{
	this->x = this->get_horiz_padding() + this->parent->getX();
	this->y = index + this->parent->getY() + 2;
	GetCursorPos(&cursor);
	//assert(&cursor != nullptr);
	const int width = this->parent->getWidth() - 45;

	render::text(this->x, this->y, this->font, this->text, false, color::white());

	render::draw_filled_rect(this->x + width, this->y, 20, 10, color(value.r, value.g, value.b));
	render::draw_rect(this->x + width, this->y, 20, 10, color::black(220));

	if (this->isExtended)
	{
		render::draw_filled_rect(this->x + width, this->y + 12, 50, 50, value);

		render::draw_rect(this->x + width, this->y + 12, 50, 50, color::black(220));
	}

	if (cursor.x > this->x + width && cursor.x < this->x + width + 20 && cursor.y > this->y && cursor.y < this->y + 10 && GetAsyncKeyState(VK_LBUTTON))
	{
		this->isExtended = !this->isExtended;
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
}

//MODE PICKER section

c_mode_picker::c_mode_picker(groupBox *parent, int classified_index, std::string text, unsigned long font, std::unordered_map<int, std::string> settings, std::string default_setting, std::string &setting, int index) : current_setting(setting)
{
	this->font = font;
	this->text = text;
	this->parent = parent;
	this->index = index;
	this->m_settings = settings;

	for (auto &elm : this->m_settings)
	{
		if (elm.second == default_setting)
		{
			this->m_current_index = elm.first;
		}
	}
	this->current_setting = default_setting;

	this->parent->m_comps[this->index] = this;

	for (auto &elm : this->m_settings)
	{
		if (longest_str < render::get_text_size(this->font, elm.second).x)
		{
			longest_str = render::get_text_size(this->font, elm.second).x;
		}
	}
	cycle_btn_width = 12;
	picker_end_pos = this->parent->getWidth() - 30;
	picker_start_pos = picker_end_pos - cycle_btn_width * 2 - longest_str - 8;
	bar_height = 12;

	if (classified_index != -1)
	{
		this->is_classified = true;
		this->classified_index = classified_index;
		if (this->parent->m_current_button_classifier != nullptr)
		{
			this->parent->m_current_button_classifier->m_btns[classified_index].classified_components[this->parent->m_current_button_classifier->m_btns[classified_index].classified_count] = this;
			this->parent->m_current_button_classifier->m_btns[classified_index].classified_count++;
		}
	}
	this->is_integrated = false;
}

void c_mode_picker::draw(int index)
{
	this->x = 15;

	this->y = index + this->parent->getY() + (this->is_integrated ? 0 : 6);
	this->x += this->parent->getX();
	//this->y += this->parent->getY();

	GetCursorPos(&cursor);

	if (GetAsyncKeyState(VK_LBUTTON) && !is_mouse)
	{
		if ((cursor.x > this->x + picker_start_pos) && (cursor.x < this->x + picker_start_pos + cycle_btn_width) && (cursor.y > y) && (cursor.y < y + bar_height))
		{
			this->mouse_clicked(cursor.x, cursor.y, VK_LBUTTON);
		}
		if ((cursor.x > this->x + picker_end_pos - cycle_btn_width) && (cursor.x < this->x + picker_end_pos) && (cursor.y > y) && (cursor.y < y + bar_height))
		{
			this->mouse_clicked(cursor.x, cursor.y, VK_LBUTTON);
		}
		is_mouse = true;
	}
	else if (!GetAsyncKeyState(VK_LBUTTON))
	{
		this->mouse_released();
		is_mouse = false;
	}

	if (!this->is_integrated)
		render::text(x, y - 2, font, this->text + ":", false, color::white());

	render::draw_filled_rect(this->x + picker_start_pos - 1, this->y - 1, picker_end_pos - picker_start_pos + 2, bar_height + 2, color(18, 18, 18, 255));
	render::draw_filled_rect(this->x + picker_start_pos, this->y, picker_end_pos - picker_start_pos, bar_height, color(34, 34, 34, 235));

	render::draw_filled_rect(this->x + picker_start_pos, this->y, cycle_btn_width, bar_height, color(40, 40, 40, 255));
	render::draw_filled_rect(this->x + picker_end_pos - cycle_btn_width, this->y, cycle_btn_width, bar_height, color(40, 40, 40, 255));

	render::text(this->x + (picker_start_pos + cycle_btn_width / 2) - render::get_text_size(font, "-").x / 2, this->y - 1, font, "-", false, color::white(255));
	render::text(this->x + (picker_end_pos - cycle_btn_width / 2) - render::get_text_size(font, "+").x / 2, this->y - 1, font, "+", false, color::white(255));

	render::text(this->x + (picker_end_pos - (picker_end_pos - picker_start_pos) / 2) - render::get_text_size(font, current_setting).x / 2, y - 1, font, this->current_setting, false, color::white());
}

void c_mode_picker::mouse_clicked(long mouse_x, long mouse_y, int mouse_button)
{

	if (mouse_button == VK_LBUTTON)
	{

		if ((mouse_x > this->x + picker_start_pos) && (mouse_x < this->x + picker_start_pos + cycle_btn_width) && (mouse_y > y) && (mouse_y < y + bar_height))
		{
			this->cycle_left();
		}

		if ((mouse_x > this->x + picker_end_pos - cycle_btn_width) && (mouse_x < this->x + picker_end_pos) && (mouse_y > y) && (mouse_y < y + bar_height))
		{
			this->cycle_right();
		}
	}
}

void c_mode_picker::mouse_released()
{
	//nothing lmao
}

void c_mode_picker::cycle_left()
{

	if (this->m_current_index - 1 < 0)
	{
		this->m_current_index = this->m_settings.size() - 1;
	}
	else
	{
		this->m_current_index--;
	}
	this->current_setting = this->m_settings.at(this->m_current_index);
}

void c_mode_picker::cycle_right()
{
	if (this->m_current_index + 1 > this->m_settings.size() - 1)
	{
		this->m_current_index = 0;
	}
	else
	{
		this->m_current_index++;
	}
	this->current_setting = this->m_settings.at(this->m_current_index);
}

std::string c_mode_picker::get_mode(int mode_index)
{
	if (this->m_settings.at(mode_index) != "")
	{
		return this->m_settings.at(mode_index);
	}
	return this->m_default_setting;
}

c_button_spacer::c_button_spacer(groupBox *parent, int classified_index, int height, int index)
{
	this->parent = parent;
	this->index = index;

	this->parent->m_comps[this->index] = this;

	if (classified_index != -1)
	{
		this->is_classified = true;
		this->classified_index = classified_index;
		if (this->parent->m_current_button_classifier != nullptr)
		{
			this->parent->m_current_button_classifier->m_btns[classified_index].classified_components[this->parent->m_current_button_classifier->m_btns[classified_index].classified_count++] = this;
		}
	}
	this->integrated_comp = nullptr;
	this->indexed_height = height;
}

void c_button_spacer::draw(int index)
{
	this->x = 15;
	this->y = index + this->parent->getY() + 2;
	this->x += this->parent->getX();
}
