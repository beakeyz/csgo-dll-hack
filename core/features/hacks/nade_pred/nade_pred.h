#pragma once
#include "./../../features.hpp"

class CCSGrenadeHint : public singleton<CCSGrenadeHint>
{
public:
	void Tick(int buttons);
	void View();
	void Paint();

	void Setup(player_t* pl, vec3_t& vecSrc, vec3_t& vecThrow, const vec3_t& angEyeAngles);
	void Simulate(vec3_t& Angles, player_t* pLocal);
	int Step(vec3_t& vecSrc, vec3_t& vecThrow, int tick, float interval);
	bool CheckDetonate(vec3_t& vecThrow, const trace_t& tr, int tick, float interval);
	void TraceHull(vec3_t& src, vec3_t& end, trace_t& tr);
	void AddGravityMove(vec3_t& move, vec3_t& vel, float frametime, bool onground);
	void PushEntity(vec3_t& src, const vec3_t& move, trace_t& tr);
	void ResolveFlyCollisionCustom(trace_t& tr, vec3_t& vecVelocity, float interval);
	int PhysicsClipVelocity(const vec3_t& in, const vec3_t& normal, vec3_t& out, float overbounce);

	enum
	{
		ACT_NONE,
		ACT_LOB,
		ACT_DROP,
		ACT_THROW
	};

	int act = 0;
	int type = 0;
	std::vector<vec3_t> path;
	std::vector<std::pair<vec3_t, vec3_t>> OtherCollisions;
	color TracerColor = color(255, 255, 0, 255);
	bool firegrenade_didnt_hit = false;

	bool is_enabled = false;
};