#pragma once
#include "../../features.hpp"
#include "../../../menu/variables.hpp"

class c_visuals : public singleton<c_visuals> {

public:
	void full_bright();
	void remove_smoke();

public:
	bool visual_recoil = false;
	bool m_full_bright = false;
	bool m_edit_smoke = false;
	bool smoke_wireframe = false;

};