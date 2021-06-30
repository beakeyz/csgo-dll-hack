#pragma once
#include "../features.hpp"
#include "../../menu/variables.hpp"

class c_ragebot{

	struct rage_t {
		rage_t(player_t* player, int i, int damage, vec3_t point, float dist) {
			m_player = player;
			index = i;
			m_damage = damage;
			m_bestpoint = point;
			distance = dist;
		}

		vec3_t m_bestpoint;
		player_t* m_player;
		//lag_record_t m_record;
		int index;
		int m_damage;
		float distance;
	};

	
	std::vector< rage_t > m_players;
	c_usercmd* m_cmd;

	void choose_angles();
	void select_target();
	//bool get_points_from_hitbox(player_t* e, std::vector< int > hitboxes, matrix3x4_t* matrix, std::vector< vec3_t >& points, float scale);
	void quickstop(weapon_t* local_weapon);
	bool is_valid(player_t* player);
	bool hitchance(vec3_t& angle, player_t* ent);

public:
	vec3_t unpredicted_vel;
	int shots_fired;
	player_t* m_last_target = nullptr;
	player_t* cur_target;
	int cur_hitgroup;

	void work(c_usercmd* cmd);
	void auto_revolver(weapon_t* local_weapon, c_usercmd* cmd);
};

extern c_ragebot g_ragebot;