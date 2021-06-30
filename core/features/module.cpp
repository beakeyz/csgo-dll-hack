#include "module.h"


bool c_module::is_enabled() {
	return this->m_is_enabled;
}

void c_module::set_enabled(bool m_enabled) {
	this->m_is_enabled = m_enabled;
}

const char* c_module::get_name() {
	return this->m_name;
}

int c_module::get_key() {
	return this->m_key_bind;
}

void c_module::set_key(int m_key) {
	this->m_key_bind = m_key;
}

std::vector<c_setting> c_module::get_settings() {
	return this->m_settings;
}