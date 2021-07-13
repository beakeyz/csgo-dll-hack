#pragma once
#include "utilities.hpp"
#include "../utilities/csgo.hpp"
#include <psapi.h>

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
	} globals;

};

extern ctx_t g_ctx;