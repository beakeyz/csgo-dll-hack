#pragma once

class c_setting {

public:
	c_setting(const char* m_name) {
		this->m_name = m_name;
	}

	const char* get_name();

protected:
	const char* m_name;

};