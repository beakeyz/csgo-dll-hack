#pragma once
#include "./../setting.h"

class c_num_setting : public c_setting {
public:
	c_num_setting(const char* name, int min, int max, int val) {
		this->m_name = name;
		this->m_min = min;
		this->m_max = max;
		this->m_val = val;
	}

	int getMin() {
		return this->m_min;
	}

	int getMax() {
		return this->m_max;
	}

	int getVal() {
		return this->m_val;
	}

	int setVal(int new_val) {
		this->m_val = new_val;
	}

private:
	int m_min;
	int m_max;
public:
	int m_val;
};