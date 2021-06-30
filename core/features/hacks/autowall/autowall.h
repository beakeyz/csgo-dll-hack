#pragma once
#include "../../features.hpp"
#include "../../../menu/variables.hpp"

struct autowall_info_t {
	autowall_info_t(const vec3_t start, const vec3_t end, player_t* to, player_t* from) {
		m_start = start;
		m_end = end;
		m_to = to;
		m_from = from;
	}

	vec3_t m_start;
	vec3_t m_end;
	player_t* m_to;
	player_t* m_from;
};

class c_autowall {
private:
	//void clip_trace(const vec3_t& src, vec3_t& end, trace_t& tr, player_t* target);
	//bool is_breakable(entity_t* entity) const;
	//bool trace_to_exit(vec3_t& end, const vec3_t& start, const vec3_t& dir, trace_t* enter_trace, trace_t* exit_trace) const;
	//bool handle_bullet_pen(surface_data_t* enter_surface, trace_t* enter_trace, const vec3_t& direction, vec3_t* origin, float penetration, int& penetration_count, float& current_damage, float min_dmg);

public:

	struct fire_bullet_data
	{
		vec3_t src;
		trace_t enter_trace;
		vec3_t direction;
		trace_filter filter;
		float trace_length;
		float trace_length_remaining;
		float current_damage;
		int penetrate_count;
	};

	float get_damage(const vec3_t& point);
	float get_damage(player_t* player, const vec3_t& vecPoint);
	bool simulate_fire_bullet(player_t* ignore, weapon_t* pWeapon, fire_bullet_data& data);
	bool handle_bullet_penetration(weapon_info_t* weaponInfo, fire_bullet_data& data);
	bool trace_to_exit(vec3_t& end, trace_t* enter_trace, vec3_t start, vec3_t dir, trace_t* exit_trace);
	void scale_damage(int hitbox, player_t* enemy, float weapon_armor_ratio, float& current_damage);
	void scale_damage(const int& hit_group, const float& weapon_armor_ratio, const bool& has_helmet, const float& armor_value, float& current_damage);
	void trace_line(const vec3_t& start, const vec3_t& end, unsigned int mask, player_t* ignore, trace_t* ptr);
	void ScaleDamage(int hitgroup, player_t* enemy, float weapon_armor_ratio, float& current_damage);

};

extern c_autowall g_autowall;