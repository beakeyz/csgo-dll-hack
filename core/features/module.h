#pragma once
#include <vector>
#include <iostream>
#include "./setting/setting.h"
#include "features.hpp"


class c_module {

public:
	c_module(const char* m_name, int m_key, std::vector<c_setting> m_settings) {
		this->m_name = m_name;
		this->m_key_bind = m_key;
		this->m_settings = m_settings;
	}

	bool is_enabled();
	void set_enabled(bool m_enabled);

	const char* get_name();

	int get_key();
	void set_key(int m_key);

	std::vector<c_setting> get_settings();

protected:
	bool m_is_enabled = false;
	const char* m_name;
	int m_key_bind;
	std::vector<c_setting> m_settings;

};

class c_module_manager : singleton<c_module_manager> {

public:
	c_module_manager(std::vector<c_module> m_modules) {
		this->m_modules = m_modules;
	}

	static c_module* get_module_by_name(const char* m_name);

	static std::vector<c_module> get_modules();

public:

	static std::vector<c_module> m_modules;
};