#pragma once
#include <iostream>
#include "../../features/features.hpp"
#include "./../../features/setting/setting.h"
#include "group_box.h"

class groupBox;

class comp {

public:
	virtual void draw(int index) {}
	virtual double get_standard_height() { return 12; }
	virtual const double get_horiz_padding() { return 15; }

	int index;
	std::string text;
};

class CompComparator {

public:

	bool operator() (const comp& msg1, const comp& msg2) const
	{
		if (msg1.index < msg2.index)
			return true;
		else
			return false;
	}
};

class buttonComponent : public comp {

public:
	buttonComponent(groupBox* parent, std::string text, unsigned long font, bool& r_value, int index);

	/*
	buttonComponent(double x, double y, groupBox* parent, std::string text, unsigned long font, bool& r_value, void(*callback)()) : value(r_value) {
		this->x = x;
		this->x2 = x;
		this->y = y;
		this->y2 = y;
		this->font = font;
		this->text = text;
		this->value = value;
		this->parent = parent;
		this->m_callback = callback;

		parent->m_comps.insert(this);

		this->draw();
	}
	*/

	void draw(int index);

	void mouse_clicked(long mouse_x, long mouse_y, int mouse_button);

	void mouse_released();

	void toggle(bool hoverd);

	std::int32_t getHeight();

	void setY(double y) {
		//this->y2 = y;
		this->y = y;
	}

	double getY() {
		return this->y;
	}

	double get_standard_height() {
		return this->indexed_height;
	}

protected:
	double x;
	double y;
	//void(*m_callback)() = nullptr;
	
	unsigned long font;
	bool& value;
	groupBox* parent;
	POINT cursor;

	bool is_mouse;
	bool clicking;

public:
	const int indexed_height = 12;

};

class c_slider_component : public comp{

public:
	c_slider_component(groupBox* parent, std::string text, unsigned long font, int& aValue, int min_value, int max_value, int index);

	void draw(int index);

	void mouse_clicked(long mouse_x, long mouse_y, int mouse_button);

	void mouse_released();

	void setY(double y) {
		//this->y2 = y;
		this->y = y;
	}

	double getY() {
		return this->y;
	}

	double get_standard_height() {
		return this->indexed_height;
	}

protected:
	double x;
	double y;
	double width;
	unsigned long font;
	int& value;
	int min_value;
	int max_value;
	groupBox* parent;
	POINT cursor;
	bool dragging;
	bool is_mouse;

public:
	const int indexed_height = 25;
};

class c_color_picker : public comp {

public:

	c_color_picker(groupBox* parent, std::string text, unsigned long font, color& aValue, int index);

	void draw(int index);

	double get_standard_height() {
		return this->indexed_height;
	}

protected:
	double x;
	double y;
	unsigned long font;
	color& value;
	groupBox* parent;
	POINT cursor;

public:
	bool isExtended = false;
	const int indexed_height = 21;
};

class c_mode_picker : public comp {

public:
	c_mode_picker(groupBox* parent, std::string text, unsigned long font, std::unordered_map<int, std::string> settings, std::string default_setting, std::string& setting, int index);

	void draw(int index);
	void cycle_left();
	void cycle_right();
	std::string get_mode(int mode_index);
	void mouse_clicked(long mouse_x, long mouse_y, int mouse_button);

	void mouse_released();

	double get_standard_height() {
		return this->indexed_height;
	}


public:
	double x;
	double y;

	unsigned long font;
	groupBox* parent;
	
	std::string m_default_setting;
	std::unordered_map<int, std::string> m_settings;
	std::string& current_setting;
	int m_current_index;

	POINT cursor;
	bool is_mouse;
	const double indexed_height = 18;

private:

	int longest_str = 0;
	int cycle_btn_width;
	int picker_end_pos;
	int picker_start_pos;
	int bar_height;
};
