#pragma once
#include "utilities.hpp"
#include "../utilities/csgo.hpp"
#include <psapi.h>
#include "./../../core/menu/components/button_component.h"
#include "./../../core/menu/components/group_box.h"
#include "./../../core/menu/components/category_btn.h"

class player_t;
class weapon_t;
class CUserCmd;



class ctx_t
{
	CUserCmd* m_pcmd = nullptr;
public:
	struct Globals
	{
		vec3_t wish_angle = { 0,0,0 };
		bool should_remove_smoke = false;
		bool send_packet = true;
		weapon_t* weapon = nullptr;
		int offset = 0;
		int flag_count = 0;
	} globals;

};

extern ctx_t g_ctx;