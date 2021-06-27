#pragma once
#include "./../../../dependencies/utilities/csgo.hpp"

class c_timer {

public:
	c_timer() {
		last_ms = interfaces::globals->cur_time;
	}

	void reset();
	bool has_time_elapsed(float time, bool reset);
	float get_time();
	void set_time(float time);

protected:
	float last_ms;

};