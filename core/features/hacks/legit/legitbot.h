#pragma once

#include "../../features.hpp"
#include "../../../menu/variables.hpp"
#include "../../module.h"
#include "../../setting/settings/bool_setting.h"

struct target_info
{
	float fov;
	int hitbox;
};

class legit_bot : public singleton <legit_bot>
{
public:
	void createmove(CUserCmd* cmd);

	void find_target();
	void find_best_point(CUserCmd* cmd, float fov);

	void auto_pistol(CUserCmd* cmd);
	void do_rcs(CUserCmd* cmd);
	void calc_fov();
	void calc_smooth();
	void do_smooth(vec3_t& currentAngle, vec3_t& aimAngle, vec3_t& angle);
	int hitchance(player_t* target, vec3_t& aim_angle, vec3_t& point, int hitbox);
	bool hitbox_intersection(player_t* e, matrix3x4_t* matrix, int hitbox, vec3_t& start, vec3_t& end);
	player_t* target;
	int tickcount = -1;
	int target_index = -1;
	float target_switch_delay;
private:
	target_info targets[65];
	float target_fov;
	float fov_t;
	float smooth_t;
	float reaction_t;
	vec3_t point = vec3_t(0, 0, 0);
	vec3_t aim_punch = vec3_t(0, 0, 0);
	vec3_t aim_angle = vec3_t(0, 0, 0);

private:

	bool enabled;
	bool friendly_fire;
	bool autopistol;

	bool autoscope;
	bool unscope;
	bool sniper_in_zoom_only;

	bool do_if_local_flashed;
	bool do_if_local_in_air;
	bool do_if_enemy_in_smoke;

	int autofire_delay;
	
	struct weapon_t
	{
		int priority;

		bool auto_stop;

		int fov_type;
		float fov;

		int smooth_type;
		float smooth;

		float silent_fov;

		int rcs_type;
		float rcs;
		float custom_rcs_smooth;
		float custom_rcs_fov;

		int awall_dmg;

		float target_switch_delay;
		int autofire_hitchance;
	} weapon[8];
};