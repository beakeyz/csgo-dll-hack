#include "./ragebot.h"
#include "../autowall/autowall.h"

c_ragebot g_ragebot;

void c_ragebot::work(c_usercmd* cmd) {
	if (!combat::ragebot::isEnabled || !interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	auto local = csgo::local_player;
	if (!local || !local->is_alive())
		return;

	auto weapon = local->active_weapon();
	if (!weapon || weapon->clip1_count() <= 0)
		return;

	//if (g_vars.rage.key && !interfaces::input->.key_pressed(g_vars.rage.key))
	//	return;

	m_cmd = cmd;

	select_target(cmd);
	choose_angles();
	//restore_players();

	if (!m_players.empty()) {
		m_players.clear();
		cur_hitgroup = -1;
	}
}

void c_ragebot::select_target(c_usercmd* cmd) {
	auto local = csgo::local_player;
	if (!local || !local->is_alive())
		return;

	auto weapon = local->active_weapon();
	if (!weapon || weapon->clip1_count() <= 0)
		return;

	try {
		
		for (short i = 1; i < interfaces::globals->maxClients; i++) {
			
			player_t* e = static_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
			
			if (!is_valid(e))
				continue;

			std::vector< int > hitboxes = {};
			/*
			if (combat::ragebot::dynamic_hitbox) {

				auto model = e->model();

				if (!model)
					return;

				auto studio_model = interfaces::model_info->get_studio_model(model);

				if (!studio_model)
					return;
				for (auto i = 0; i < studio_model->bones_count; i++)
				{
					hitboxes.push_back(i);
				}
			}
			else {
				*/
				if (combat::ragebot::head) {
					hitboxes.push_back(8);
					hitboxes.push_back(7);
				}
				if (combat::ragebot::pelvis) {
					hitboxes.push_back(4);
					hitboxes.push_back(5);
					hitboxes.push_back(3);
					hitboxes.push_back(0);
				}
				if (combat::ragebot::arms) {
					hitboxes.push_back(65);
					hitboxes.push_back(36);
					hitboxes.push_back(63);
					hitboxes.push_back(35);
					hitboxes.push_back(90);
					hitboxes.push_back(56);
				}
				if (combat::ragebot::legs) {
					hitboxes.push_back(88);
					hitboxes.push_back(72);
					hitboxes.push_back(83);
					hitboxes.push_back(74);
					hitboxes.push_back(84);
					hitboxes.push_back(76);
				}
			//}

			float player_best_damage = 0.f;
			vec3_t player_best_point = vec3_t(0.f, 0.f, 0.f);
			const int best_min_dmg = local->active_weapon()->clip1_count() <= 3 ? e->health() : combat::ragebot::min_dmg; // ensure we get the kill
			
			for (auto& hitbox : hitboxes) {
				const vec3_t point = e->getBonePos(hitbox);
				const vec3_t local_eye_pos = csgo::local_player->get_eye_pos();
				weapon_t* cur_weapon = local->active_weapon();
				weapon_info_t* cur_weapon_info = cur_weapon->get_weapon_data();
				float current_damage = cur_weapon_info->iDamage;

				switch (hitbox)
				{
				case 8:
				case 7:
					cur_hitgroup = hitgroup_head;
					break;
				case 4:
				case 5:
				case 3:
				case 0:
					cur_hitgroup = hitgroup_chest;
					break;
				case 65:
				case 63:
					cur_hitgroup = hitgroup_leftarm;
					break;
				case 36:
				case 35:
					cur_hitgroup = hitgroup_rightarm;
					break;

				case 88:
				case 83:
				case 84:
					cur_hitgroup = hitgroup_leftleg;
					break;
				case 72:
				case 74:
				case 76:
					cur_hitgroup = hitgroup_rightleg;
					break;
				default:
					cur_hitgroup = -1;
					break;
				}

				trace_t trace;
				ray_t ray;
				trace_filter_skip_one_entity filter(local);

				ray.initialize(local_eye_pos, point);

				interfaces::trace_ray->trace_ray(ray, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &trace);

				if (cur_hitgroup != -1) {
					current_damage = g_autowall.get_damage(point);
					if (current_damage <= 0) {
						continue;
					}

					if (current_damage > e->health()) {
						player_best_damage = current_damage;
						player_best_point = point;
						break;
					}

					if (current_damage > player_best_damage && current_damage > best_min_dmg) {
						player_best_damage = current_damage;
						player_best_point = point;
					}
				}
			}

			m_players.emplace_back(e, i, static_cast<int>(player_best_damage), player_best_point, e->abs_origin().distance_to(local->abs_origin()));
		}

		const char* tmp = this->curent_sort_mode.c_str();

		std::sort(m_players.begin(), m_players.end(), [&](rage_t& a, rage_t& b) {

			
			if (std::strcmp(tmp, "Health")) {
				return a.m_damage > b.m_damage;
			}
			else if (std::strcmp(tmp, "Distance")) {
				return a.distance < b.distance;
			}
			else if (std::strcmp(tmp, "Fov")) {

				auto calculateRelativeAngle = [&](const vec3_t& source, const vec3_t& destination, const vec3_t& viewAngles)
				{
					return ((destination - source).toAngle() - viewAngles).normalized();
				};

				const auto angle1 = calculateRelativeAngle(csgo::local_player->get_eye_pos(), a.m_bestpoint, cmd->viewangles);
				const auto angle2 = calculateRelativeAngle(csgo::local_player->get_eye_pos(), b.m_bestpoint, cmd->viewangles);

				float fov1 = std::hypot(angle1.x, angle1.y);
				float fov2 = std::hypot(angle2.x, angle2.y);

				return fov1 < fov2;
			}
			else {
				return a.index < b.index;
			}
		});
	}
	catch (const std::out_of_range& ex) {
		UNREFERENCED_PARAMETER(ex);
		_RPT0(_CRT_ERROR, ex.what());
	}
}

bool c_ragebot::hitchance(vec3_t& angle, player_t* ent) {
	int traces_hit = 0;
	auto local = csgo::local_player;
	if (!local)
		return false;

	vec3_t forward, right, up;
	vec3_t eye_position = local->get_eye_pos();
	math::angle_vectors(angle - local->aim_punch_angle() * 2.f, forward, right, up); // maybe add an option to not account for punch.

	weapon_t* weapon = local->active_weapon();
	if (!weapon)
		return false;

	weapon->update_accuracy_penalty();
	float weapon_spread = weapon->get_spread();
	float weapon_cone = weapon->inaccuracy();

	const auto get_bullet_location = [&](int seed) {
		static auto random_seed = reinterpret_cast<void(*)(int)>(GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomSeed"));
		random_seed(seed);

		float a = math::random_float(0.f, 1.f);
		float b = math::random_float(0.f, 2.f * M_PI);
		float c = math::random_float(0.f, 1.f);
		float d = math::random_float(0.f, 2.f * M_PI);

		const float generated_spread = a * weapon_spread;
		const float generated_cone = c * weapon_cone;

		const vec3_t spread = vec3_t(
			std::cos(b) * generated_spread + std::cos(d) * generated_cone,
			std::sin(b) * generated_spread + std::sin(d) * generated_cone,
			0.f
		);

		return vec3_t(forward + right * -spread.x + up * -spread.y).normalized();
	};

	for (int i = 1; i <= 256; i++) {
		vec3_t spread_angle;
		vec3_t bullet_end;

		math::vector_angles(get_bullet_location(i), spread_angle);
		math::angle_to_vector(angle - (spread_angle - angle), bullet_end);

		trace_t trace;
		ray_t ray;
		ray.initialize(eye_position, eye_position + bullet_end * weapon->get_weapon_data()->flRange);

		interfaces::trace_ray->clip_ray_to_entity(ray, MASK_SHOT, ent, &trace);

		if (trace.entity == ent)
			++traces_hit;

		if (traces_hit >= static_cast<int>(combat::ragebot::hit_chance * 2.56f))
			return true;
	}

	return false;
}

void c_ragebot::choose_angles() {
	player_t* selected_target = nullptr;
	vec3_t best_hitboxpos;
	float best_damage = 0.f;

	auto local = csgo::local_player;
	if (!local || !local->is_alive() || local->health() == 0)
		return;

	auto weapon = local->active_weapon();
	if (!weapon)
		return;

	for (auto& data : m_players) {
		auto target = data.m_player;

		float player_best_damage = data.m_damage;
		vec3_t player_best_point = data.m_bestpoint;

		if (player_best_damage > best_damage) {
			selected_target = target;
			best_hitboxpos = player_best_point;
			best_damage = player_best_damage;
		}

		if (player_best_damage >= target->health()) {
			selected_target = target;
			best_hitboxpos = player_best_point;
			best_damage = player_best_damage;
			break;
		}
	}

	if (selected_target == nullptr) {
		return;
	}

	
	m_last_target = selected_target;

	//always
	if ((!local->is_scoped() && this->has_scope(weapon)) && selected_target)
		m_cmd->buttons |= in_attack2;

	vec3_t aim_angle = math::calc_angle(local->get_eye_pos(), best_hitboxpos);
	aim_angle.normalized();
	aim_angle.clamp();

	//if (!hitchance(aim_angle, selected_target)) {
		//hitchance fail
		//if ((!local->is_scoped() && this->has_scope(weapon)) && selected_target)
		//	m_cmd->buttons |= in_attack2;

		if (combat::ragebot::quickstop)
			quickstop(weapon);

		//if (combat::ragebot::auto_fire)
		//	return;
	//}
	
	if (csgo::local_player->next_attack() > interfaces::globals->curtime)
		return;

	if (combat::ragebot::auto_fire /*&& !(m_cmd->buttons & in_attack)*/) {
		m_cmd->buttons |= in_attack;
		shots_fired++;
	}

	if (m_cmd->buttons & in_attack) {
		
		m_cmd->viewangles = aim_angle;

		weapon->update_accuracy_penalty();
		m_cmd->viewangles -= local->aim_punch_angle() * 2.f;

		if (!combat::ragebot::silent)
			interfaces::engine->set_view_angles(m_cmd->viewangles);

		//if (g_vars.misc.client_hitboxes)
		//	g_misc.capsule_overlay(selected_target, g_vars.misc.client_hitboxes_duration, best_record.m_matrix);

		//g_backtrack.process_cmd(m_cmd, selected_target, best_record);
		if (shots_fired > 1) {
			m_cmd->buttons &= ~in_attack;
			m_cmd->buttons &= ~in_attack2;
			shots_fired = 0;
		}
	}
	
}

//bool c_ragebot::get_points_from_hitbox(player_t* e, std::vector< int > hitboxes, matrix3x4_t* matrix, std::vector< vec3_t >& points, float scale) {
	
//	return true;
//}

void c_ragebot::quickstop(weapon_t* local_weapon) {
	const weapon_info_t* weapon_info = local_weapon->get_weapon_data();
	if (!weapon_info)
		return;

	// note: scoped weapons use the alternate speed member.
	const float max_speed = this->has_scope(local_weapon) ? weapon_info->flMaxSpeed[1] : weapon_info->flMaxSpeed[0];

	if (unpredicted_vel.length_2d() > max_speed * .34f) {
		const vec3_t velocity = unpredicted_vel;
		const float_t speed = csgo::local_player->velocity().length();

		vec3_t direction;
		math::vector_angles(velocity, direction);

		direction.y = m_cmd->viewangles.y - direction.y;

		vec3_t forward;
		math::angle_to_vector(direction, forward);
		vec3_t negated_direction = forward * -speed;

		const auto m_max = [&](float a, float b) {
			if (a > b)return a; 
			return b;
		};

		const float factor = m_max(negated_direction.x, negated_direction.y) / 450.f;
		negated_direction *= factor;

		m_cmd->forwardmove = negated_direction.x;
		m_cmd->sidemove = negated_direction.y;
	}
}

/*
void c_ragebot::auto_revolver(c_base_combat_weapon* local_weapon, c_user_cmd* cmd) {
	if (!local_weapon || local_weapon->item_index() != WEAPON_REVOLVER)
		return;

	if (!(TIME_TO_TICKS(g_csgo.m_global_vars->m_cur_time - local_weapon->ready_time()) <= -1))
		return;

	cmd->m_buttons |= IN_ATTACK;
}
*/

bool c_ragebot::is_valid(player_t* player) {
	if (!player || player->dormant() || !player->is_player() || player == csgo::local_player)
		return false;

	if (!player->is_alive() || player->health() == 0 || player->has_gun_game_immunity())
		return false;

	if (player->team() == csgo::local_player->team() && !combat::ragebot::teammates)
		return false;

	/*if( player->survival_team() != -1 )
		return false;*/
	
	return true;
}

bool c_ragebot::has_scope(weapon_t* weapon) {
	return (weapon->get_weapon_data()->nWeaponType == WEAPONTYPE_SNIPER_RIFLE || weapon->client_class()->class_id == cweaponaug || weapon->client_class()->class_id == cweaponsg556);
}

void combat::ragebot::onEnable() {
	g_ragebot.shots_fired = 0;
}