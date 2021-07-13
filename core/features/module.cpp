#include "module.h"

c_module* c_module_manager::get_module_by_name(const char* m_name) {
	if (m_modules.empty()) {
		return nullptr;
	}

	for (c_module* mod : m_modules) {
		if (strcmp(mod->get_name(), m_name) == 0) {
			return mod;
		}
	}
}

std::set<c_module*> c_module_manager::get_modules() {
	return c_module_manager::m_modules;
}

bool c_module::is_enabled() {
	return this->m_is_enabled;
}

void c_module::set_enabled(bool m_enabled) {
	this->m_is_enabled = m_enabled;
}

const char* c_module::get_name() {
	return this->m_name;
}

std::vector<c_setting> c_module::get_settings() {
	return this->m_settings;
}