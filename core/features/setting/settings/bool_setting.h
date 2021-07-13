#pragma once
#include "../setting.h"

class c_bool_setting : public c_setting {

public:
	c_bool_setting(const char* name, bool& value) : m_value(value){
		this->m_value = m_value;
		this->m_name = name;
	}

	void set_value(bool new_value) { this->m_value = new_value; }

	bool get_value() { return this->m_value; }

	void toggle() { this->m_value = !this->m_value; }
	
public:
	bool& m_value;
};
