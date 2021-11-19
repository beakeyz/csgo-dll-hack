#pragma once
#include "./../../../dependencies/utilities/csgo.hpp"

//Timer MIGHT prevent injection
class c_timer {

public:
	c_timer() {
		last_ms = interfaces::globals->curtime;
	}

	void reset();
	bool has_time_elapsed(float time, bool reset);
	float get_time();
	void set_time(float time);

protected:
	float last_ms;

};