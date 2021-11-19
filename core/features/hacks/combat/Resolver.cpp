#include "Resolver.h"
#include "../autowall/autowall.h"


void resolver::initialize(player_t* e, adjust_data* record, const float& goal_feet_yaw, const float& pitch)
{
	player = e;
	player_record = record;

	original_goal_feet_yaw = math::normalize_yaw(goal_feet_yaw);
	original_pitch = math::normalize_pitch(pitch);
}

void resolver::reset()
{
	player = nullptr;
	player_record = nullptr;

	was_first_bruteforce = false;
	was_second_bruteforce = false;

	original_goal_feet_yaw = 0.0f;
	original_pitch = 0.0f;
}

void resolver::resolve_yaw()
{
	player_info_t player_info;
	auto animstate = player->get_anim_state();
	static float old_sim_time;
	interfaces::engine->get_player_info(player->index(), &player_info);

	if (!animstate || player_info.fakeplayer || !csgo::local_player->is_alive() || player->team() == csgo::local_player->team() || abs(TIME_TO_TICKS(player->simulation_time() - old_sim_time) - 1) || player_record->shot)
	{
		player_record->side = RESOLVER_ORIGINAL;
		return;
	}

	//if (g_ctx.globals.missed_shots[player->EntIndex()])
	//{
		switch (last_side)
		{
		case RESOLVER_ORIGINAL:
			//g_ctx.globals.missed_shots[player->EntIndex()] = 0;
			break;
		case RESOLVER_ZERO:
			player_record->side = RESOLVER_LOW_POSITIVE;

			was_first_bruteforce = false;
			was_second_bruteforce = false;
			return;
		case RESOLVER_POSITIVE:
			player_record->side = was_second_bruteforce ? RESOLVER_ZERO : RESOLVER_NEGATIVE;

			was_first_bruteforce = true;
			return;
		case RESOLVER_NEGATIVE:
			player_record->side = was_first_bruteforce ? RESOLVER_ZERO : RESOLVER_POSITIVE;

			was_second_bruteforce = true;
			return;
		case RESOLVER_LOW_POSITIVE:
			player_record->side = RESOLVER_LOW_NEGATIVE;
			return;
		case RESOLVER_LOW_NEGATIVE:
			player_record->side = RESOLVER_POSITIVE;
			return;
		}
	//}

	auto valid_move = true;
	if (animstate->velocity > 0.1f)
	{
		valid_move = animstate->last_time_started_moving < 0.22f;
	}

	if (valid_move)
	{
		auto delta = math::angle_difference(player->eye_angles().y, zero_goal_feet_yaw);
		auto positive_resolver = (2 * (delta <= 0.0f) - 1) > 0;
		player_record->side = positive_resolver ? RESOLVER_POSITIVE : RESOLVER_NEGATIVE;
	}
	else
	{
		if (interfaces::globals->curtime - lock_side > 2.0f)
		{
			//g_autowall.
			//auto fire_data_positive = autowall::get().wall_penetration(g_ctx.globals.eye_pos, player->mathitbox_position_matrix(HITBOX_HEAD, player_record->matrixes_data.positive), player);
			//auto fire_data_negative = autowall::get().wall_penetration(g_ctx.globals.eye_pos, player->hitbox_position_matrix(HITBOX_HEAD, player_record->matrixes_data.negative), player);

			//if (fire_data_positive.visible != fire_data_negative.visible)
			//{
			//	player_record->side = fire_data_negative.visible ? RESOLVER_POSITIVE : RESOLVER_NEGATIVE;
			//	lock_side = interfaces::globals->cur_time;
			//}
			//else
			//{
			//	if (fire_data_positive.damage != fire_data_negative.damage)
			//	{
			//		player_record->side = fire_data_negative.damage > fire_data_positive.damage ? RESOLVER_POSITIVE : RESOLVER_NEGATIVE;
			//	}
			player_record->side = RESOLVER_LOW_NEGATIVE;
		
		}
	}

	old_sim_time = player->simulation_time();
}

float resolver::resolve_pitch()
{
	return original_pitch;
}