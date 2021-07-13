#pragma once
#include "../setting.h"
#include <vector>
#include <string>

class c_setting;

class c_mode_setting : public c_setting {

public:
	c_mode_setting(const char* name, const char* default_mode, std::vector<std::string>& modes) : m_modes(modes) {
		this->m_name = name;
		this->m_modes = m_modes;
		this->m_default = default_mode;
		for (int i = 0; i < this->m_modes.size(); i++) {
			const char* tmp = this->m_modes.at(i).c_str();
			if (strcmp(tmp, default_mode) == 0) {
				this->m_curr_mode_index = i;
				break;
			}
			this->m_curr_mode_index = 0;
		}
	}

	std::string get_mode() {
		return this->m_modes.at(m_curr_mode_index);
	}

	bool is_mode(std::string mode) {
		int i = 0;

		for (; i < this->m_modes.size(); i++) {
			const char* tmp = this->m_modes.at(i).c_str();
			if (strcmp(tmp, mode.c_str()) == 0) {
				return this->m_curr_mode_index == i;
			}
		}
		return false;
	}

	void set_mode(std::string new_mode) {
		int i = 0;

		for (; i < this->m_modes.size(); i++) {
			const char* tmp = this->m_modes.at(i).c_str();
			if (strcmp(tmp, new_mode.c_str()) == 0) {
				m_curr_mode_index == i;
			}
		}
	}

	void cycle() {
		if (this->m_curr_mode_index < this->m_modes.size() - 1) {
			this->m_curr_mode_index++;
		}
		else {
			this->m_curr_mode_index = 0;
		}
	}

public:
	std::string m_default;
	std::vector<std::string>& m_modes;
	int m_curr_mode_index;
};