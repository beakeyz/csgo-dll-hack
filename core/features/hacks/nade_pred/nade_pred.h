#pragma once
#include "./../../features.hpp"

enum ACT {
	ACT_NONE,
	ACT_THROW,
	ACT_LOB,
	ACT_DROP
};

class c_nade_prediction : public singleton<c_nade_prediction>{

	int act = 0;
	bool has_grenade = false;
	bool was_flashed = false;

	std::vector<std::pair<vec3_t, vec3_t>> screen_points;
	std::vector<std::pair<vec3_t, vec3_t>> saved_points;

	player_t* player = nullptr;
	weapon_t* weapon = nullptr;

	std::mutex render_mutex;

public:
	void Tick(int buttons);
	bool is_enabled(c_usercmd* cmd);
	void fetch_points(c_usercmd* cmd);
	void Setup(vec3_t& vecSrc, vec3_t& vecThrow, vec3_t viewangles);
	void ResolveFlyCollisionCustom(trace_t& tr, vec3_t& vecVelocity, float interval);
	void TraceHull(vec3_t& src, vec3_t& end, trace_t& tr);
	void PushEntity(vec3_t& src, const vec3_t& move, trace_t& tr);
	void AddGravityMove(vec3_t& move, vec3_t& vel, float frametime, bool onground);
	int PhysicsClipVelocity(const vec3_t& in, const vec3_t& normal, vec3_t& out, float overbounce);
	void render();
};

extern c_nade_prediction g_nadepred;