#pragma once
#include "../../features.hpp"
#include "../../../menu/variables.hpp"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

#define TIME_TO_TICKS(t) ((int)(0.5f + (float)(t) / interfaces::globals->interval_per_tick))
#define TICKS_TO_TIME(t) (interfaces::globals->interval_per_tick * (t))

enum
{
	MATRIX_MAIN,
	MATRIX_NEGATIVE,
	MATRIX_ZERO,
	MATRIX_POSITIVE
};

enum resolver_side
{
	RESOLVER_ORIGINAL,
	RESOLVER_NEGATIVE,
	RESOLVER_LOW_NEGATIVE,
	RESOLVER_ZERO,
	RESOLVER_LOW_POSITIVE,
	RESOLVER_POSITIVE,
};

struct matrixes
{
	matrix_t main[MAXSTUDIOBONES];
	matrix_t negative[MAXSTUDIOBONES];
	matrix_t zero[MAXSTUDIOBONES];
	matrix_t positive[MAXSTUDIOBONES];
};

class adjust_data;

class resolver
{
	player_t* player = nullptr;
	adjust_data* player_record = nullptr;

	bool was_first_bruteforce = false;
	bool was_second_bruteforce = false;

	float lock_side = 0.0f;
	float original_goal_feet_yaw = 0.0f;
	float original_pitch = 0.0f;
public:
	void initialize(player_t* e, adjust_data* record, const float& goal_feet_yaw, const float& pitch);
	void reset();
	void resolve_yaw();
	float resolve_pitch();

	//AnimationLayer resolver_layers[3][13];
	//AnimationLayer previous_layers[13];
	float negative_goal_feet_yaw = 0.0f;
	float zero_goal_feet_yaw = 0.0f;
	float positive_goal_feet_yaw = 0.0f;

	resolver_side last_side = RESOLVER_ORIGINAL;
};

class adjust_data
{
public:
	player_t* player;
	int i;

	//AnimationLayer layers[13];
	matrixes matrixes_data;

	resolver_side side;

	bool invalid;
	bool immune;
	bool dormant;
	bool bot;
	bool shot;

	int flags;
	int bone_count;

	float simulation_time;
	float duck_amount;
	float lby;

	vec3_t angles;
	vec3_t abs_angles;
	vec3_t velocity;
	vec3_t origin;
	vec3_t mins;
	vec3_t maxs;

	adjust_data()
	{
		reset();
	}

	void reset()
	{
		player = nullptr;
		i = -1;

		side = RESOLVER_ORIGINAL;

		invalid = false;
		immune = false;
		dormant = false;
		bot = false;
		shot = false;

		flags = 0;
		bone_count = 0;

		simulation_time = 0.0f;
		duck_amount = 0.0f;
		lby = 0.0f;

		angles = {0, 0, 0};
		abs_angles = { 0, 0, 0 };
		velocity = { 0, 0, 0 };
		origin = { 0, 0, 0 };
		mins = { 0, 0, 0 };
		maxs = { 0, 0, 0 };
	}

	adjust_data(player_t* e, bool store = true)
	{
		side = RESOLVER_ORIGINAL;

		invalid = false;
		store_data(e, store);
	}

	void store_data(player_t* e, bool store = true)
	{
		if (!e->is_alive())
			return;

		player = e;
		i = player->index();

		//if (store)
		//{
			//memcpy(layers, e->get_animlayers(), e->animlayer_count() * sizeof(AnimationLayer));
			//memcpy(matrixes_data.main, player->m_CachedBoneData().Base(), player->m_CachedBoneData().Count() * sizeof(matrix3x4_t));
		//}

		immune = player->has_gun_game_immunity() || player->flags() & fl_frozen;
		dormant = player->dormant();

		player_info_t player_info;
		interfaces::engine->get_player_info(i, &player_info);

		bot = player_info.fakeplayer;
		shot = player->active_weapon()/*&& (player->m_hActiveWeapon()->m_fLastShotTime() == player->m_flSimulationTime())*/;

		flags = player->flags();
		bone_count = interfaces::model_info->get_studio_model(player->model())->bones_count;

		simulation_time = player->simulation_time();
		duck_amount = player->duck_amount();
		lby = player->lower_body_yaw();

		angles = player->eye_angles();
		abs_angles = player->abs_angles();
		velocity = player->velocity();
		origin = player->origin();
		mins = player->collideable()->mins();
		maxs = player->collideable()->maxs();
	}

	void adjust_player()
	{
		if (!valid(false))
			return;

		//memcpy(player->get_anim_state(), layers, player->animlayer_count() * sizeof(AnimationLayer));
		//memcpy(player->m_CachedBoneData().Base(), matrixes_data.main, player->m_CachedBoneData().Count() * sizeof(matrix3x4_t));

		player->flags() = flags;
		interfaces::model_info->get_studio_model(player->model())->bones_count = bone_count;

		player->simulation_time() = simulation_time;
		player->duck_amount() = duck_amount;
		player->lower_body_yaw() = lby;

		player->eye_angles() = angles;
		player->abs_angles() = abs_angles;
		player->velocity() = velocity;
		player->origin() = origin;
		player->abs_origin() = origin;
		player->collideable()->mins() = mins;
		player->collideable()->maxs() = maxs;
	}

	bool valid(bool extra_checks = true)
	{
		if (!this)
			return false;

		if (i > 0)
			player = (player_t*)interfaces::entity_list->get_client_entity(i);

		if (!player)
			return false;

		if (!player->is_alive())
			return false;

		if (immune)
			return false;

		if (dormant)
			return false;

		if (!extra_checks)
			return true;

		if (invalid)
			return false;

		auto net_channel_info = interfaces::engine->get_net_channel_info();

		if (!net_channel_info)
			return false;

		/*
		static auto sv_maxunlag = interfaces::console->get_convar("sv_maxunlag");

		auto outgoing = net_channel_info->GetLatency(FLOW_OUTGOING);
		auto incoming = net_channel_info->GetLatency(FLOW_INCOMING);

		auto correct = math::clamp(outgoing + incoming + util::get_interpolation(), 0.0f, sv_maxunlag->float_value);

		auto curtime = csgo::local_player->is_alive() ? TICKS_TO_TIME(g_ctx.globals.fixed_tickbase) : interfaces::globals->cur_time;
		auto delta_time = correct - (curtime - simulation_time);

		if (fabs(delta_time) > 0.2f)
			return false;

		auto extra_choke = 0;

		//if (g_ctx.globals.fakeducking)
		//	extra_choke = 14 - m_clientstate()->iChokedCommands;

		auto server_tickcount = extra_choke + interfaces::globals->cur_time + TIME_TO_TICKS(outgoing + incoming);
		auto dead_time = (int)(TICKS_TO_TIME(server_tickcount) - sv_maxunlag->float_value);

		if (simulation_time < (float)dead_time)
			return false;

		*/

		return true;
	}
};

class optimized_adjust_data
{
public:
	int i;

	player_t* player;

	float simulation_time;
	float speed;

	bool shot;

	optimized_adjust_data()
	{
		reset();
	}

	void reset()
	{
		i = 0;

		player = nullptr;

		simulation_time = 0.0f;
		speed = 0.0f;

		shot = false;
	}
};

extern std::deque <adjust_data> player_records[65];
