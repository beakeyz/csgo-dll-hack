#pragma once
#include "../interfaces/interfaces.hpp"

class c_physics_surface {
public:
	surface_data_t* get_surface_data(int data) {
		using original_fn = surface_data_t*(__thiscall*)(c_physics_surface*, int);
		return (*(original_fn**)this)[5](this, data);
	}
};