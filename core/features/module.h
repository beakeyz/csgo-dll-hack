#pragma once
#include <vector>
#include <iostream>
#include "./setting/setting.h"
#include "features.hpp"

class c_module {

public:
	c_module(const char* m_name, std::vector<c_setting> m_settings) {
		this->m_name = m_name;
		//this->m_key_bind = m_key;
		this->m_settings = m_settings;
	}

	c_module() {}

	bool is_enabled();
	void set_enabled(bool m_enabled);

	const char* get_name();

	std::vector<c_setting> get_settings();

protected:
	bool m_is_enabled = false;
	const char* m_name;
	std::vector<c_setting> m_settings;

};

class c_module_manager : singleton<c_module_manager> {

public:
	c_module_manager() {
		this->m_modules = {
			//g_ragebot
		};
	}

	c_module* get_module_by_name(const char* m_name);

	std::set<c_module*> get_modules();

public:

	std::set<c_module*> m_modules;
};