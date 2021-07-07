#pragma once

class collideable_t {
public:
	vec3_t& mins() const {
		using original_fn = vec3_t & (__thiscall*)(const void*);
		return (*(original_fn**)this)[1](this);
	}
	vec3_t& maxs() const {
		using original_fn = vec3_t & (__thiscall*)(const void*);
		return (*(original_fn**)this)[2](this);
	}
};