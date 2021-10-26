#pragma once
#include "./../../features.hpp"

class c_misc : public singleton<c_misc> {

public:
	void bhop();
	void auto_strafe();
	void fast_stop();
	void third_person();

public:
	int debug_fov = 90;
	bool m_third_person = false;
	bool m_third_person_disable_on_weapon = false;
};