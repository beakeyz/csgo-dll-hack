#pragma once
#include "./../variables.hpp"
#include "./../../features/features.hpp"


class c_animation {
public:
	c_animation(int ms, double intended_out) {
		this->m_duration = ms;
		this->m_intended_out = intended_out;
	}



protected:
	int m_duration;
	double m_intended_out;
	c_timer t;

};
