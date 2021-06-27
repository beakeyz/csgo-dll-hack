#include "./timer.h"

void c_timer::reset() {
	c_timer::last_ms = interfaces::globals->cur_time;
}

bool c_timer::has_time_elapsed(float time, bool reset) {
	if (interfaces::globals->cur_time - c_timer::last_ms > time) {
		if (reset) {
			c_timer::reset();
		}
		return true;
	}
	return false;
}

float c_timer::get_time() {
	return c_timer::last_ms;
}

void c_timer::set_time(float time){
	c_timer::last_ms = time;
}