#include "nade_pred.h"
#include "./../features.h"
#include "./../../../menu/menu.hpp"

#include <mutex>

void c_nade_prediction::Tick(int buttons)
{
	if (!c_visuals::get_ptr()->projectile_prediction)
		return;

	bool _in_attack = buttons & in_attack;
	bool _in_attack2 = buttons & in_attack2;

	act = (_in_attack && _in_attack2) ? ACT::ACT_LOB : (_in_attack2) ? ACT::ACT_DROP : (_in_attack) ? ACT::ACT_THROW : ACT::ACT_NONE;
}

bool c_nade_prediction::is_enabled(c_usercmd* cmd)
{
	if (!csgo::local_player)
		return false;

	player = csgo::local_player;
	if (player->is_alive())
		Tick(cmd->buttons);

	else if (player->observer_target())
	{
		player = (player_t*)interfaces::entity_list->get_client_entity_handle(player->observer_target());
		act = 0;
	}
	else
		return false;

	if (player->move_type() == movetype_noclip)
		return false;

	weapon = player->active_weapon();
	if (!weapon || weapon->get_weapon_data()->nWeaponType != WEAPONTYPE_GRENADE)
		return false;

	return true;
}

void c_nade_prediction::fetch_points(c_usercmd* cmd)
{
	if (!c_visuals::get_ptr()->projectile_prediction || !interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return;

	if ((prediction::data.buttons & in_attack && !(cmd->buttons & in_attack2)) || (prediction::data.buttons & in_attack2 && !(cmd->buttons & in_attack2))) {
		
		screen_points.clear();
		return;
	}

	render_mutex.lock();

	screen_points.clear();

	if (!is_enabled(cmd))
	{
		render_mutex.unlock();

		return;
	}

	was_flashed = player->is_flashed();

	vec3_t vecSrc, vecThrow;
	Setup(vecSrc, vecThrow, cmd->viewangles);

	float interval = interfaces::globals->interval_per_tick;

	int logstep = static_cast<int>(0.05f / interval);
	int logtimer = 0;

	std::vector<vec3_t> path;

	for (unsigned int i = 0; i < path.max_size() - 1; ++i)
	{
		if (!logtimer) {
			path.emplace_back(vecSrc);
		}
		
		vec3_t move{0,0,0};
		AddGravityMove(move, vecThrow, interval, false);

		// Push entity
		trace_t tr;
		PushEntity(vecSrc, move, tr);

		int result = 0;

		if (weapon->check_detonate(vecThrow, tr, i, interval)) {
			result |= 1;
		}

		if (tr.flFraction != 1.0f)
		{
			result |= 2; // Collision!
			ResolveFlyCollisionCustom(tr, vecThrow, interval);
		}

		vecSrc = tr.end;

		if (result & 1)
			break;
			
		if ((result & 2) || logtimer >= logstep)
			logtimer = 0;
		else
			++logtimer;
	}

	path.emplace_back(vecSrc);

	vec3_t prev = path[0];
	vec3_t nadeStart, nadeEnd;

	for (auto& nade : path)
	{
		if (math::world_to_screen(prev, nadeStart) && math::world_to_screen(nade, nadeEnd))
		{
			screen_points.emplace_back(std::pair<vec3_t, vec3_t>{ nadeStart, nadeEnd });
		}
		prev = nade;
	}

	render_mutex.unlock();
}

void c_nade_prediction::Setup(vec3_t& vecSrc, vec3_t& vecThrow, vec3_t viewangles)
{
	vec3_t angThrow = viewangles;
	//roll
	float pitch = angThrow.x;

	if (pitch <= 90.0f)
	{
		if (pitch < -90.0f)
		{
			pitch += 360.0f;
		}
	}
	else
	{
		pitch -= 360.0f;
	}

	float a = pitch - (90.0f - fabs(pitch)) * 10.0f / 90.0f;
	angThrow.x = a;

	float flVel = 750.0f * 0.9f;

	static const float power[] = { 1.0f, 1.0f, 0.5f, 0.0f };
	float b = power[act];
	b = b * 0.7f;
	b = b + 0.3f;
	flVel *= b;

	vec3_t vForward, vRight, vUp;
	math::angle_vectors(angThrow, vForward, vRight, vUp);

	vecSrc = csgo::local_player->get_eye_pos();
	float off = (power[act] * 12.0f) - 12.0f;
	vecSrc.z += off;

	trace_t tr;
	vec3_t vecDest = vecSrc;
	vecDest += vForward * 22.0f;

	TraceHull(vecSrc, vecDest, tr);

	vec3_t vecBack = vForward; vecBack *= 6.0f;
	vecSrc = tr.end;
	vecSrc -= vecBack;

	vecThrow = csgo::local_player->velocity(); vecThrow *= 1.25f;
	vecThrow += vForward * flVel;
}

void c_nade_prediction::ResolveFlyCollisionCustom(trace_t& tr, vec3_t& vecVelocity, float interval)
{
	if (!c_visuals::get_ptr()->projectile_prediction)
		return;

	// Calculate elasticity
	float flSurfaceElasticity = 1.0;
	float flGrenadeElasticity = 0.45f;
	float flTotalElasticity = flGrenadeElasticity * flSurfaceElasticity;
	if (flTotalElasticity > 0.9f) flTotalElasticity = 0.9f;
	if (flTotalElasticity < 0.0f) flTotalElasticity = 0.0f;

	// Calculate bounce
	vec3_t vecAbsVelocity;
	PhysicsClipVelocity(vecVelocity, tr.plane.normal, vecAbsVelocity, 2.0f);
	vecAbsVelocity *= flTotalElasticity;

	float flSpeedSqr = vecAbsVelocity.length_sqr();
	static const float flMinSpeedSqr = 20.0f * 20.0f;

	if (flSpeedSqr < flMinSpeedSqr)
	{
		vecAbsVelocity.x = 0.0f;
		vecAbsVelocity.y = 0.0f;
		vecAbsVelocity.z = 0.0f;
	}

	if (tr.plane.normal.z > 0.7f)
	{
		vecVelocity = vecAbsVelocity;
		vecAbsVelocity *= ((1.0f - tr.flFraction) * interval);
		PushEntity(tr.end, vecAbsVelocity, tr);
	}
	else
	{
		vecVelocity = vecAbsVelocity;
	}
}

void c_nade_prediction::TraceHull(vec3_t& src, vec3_t& end, trace_t& tr)
{
	if (!c_visuals::get_ptr()->projectile_prediction)
		return;

	ray_t ray;
	ray.initialize(src, end, vec3_t(-2.0f, -2.0f, -2.0f), vec3_t(2.0f, 2.0f, 2.0f));

	trace_filter filter;
	filter.skip = csgo::local_player;

	interfaces::trace_ray->trace_ray(ray, 0x200400B, &filter, &tr);
}

void c_nade_prediction::PushEntity(vec3_t& src, const vec3_t& move, trace_t& tr)
{
	if (!c_visuals::get_ptr()->projectile_prediction)
		return;

	vec3_t vecAbsEnd = src;
	vecAbsEnd += move;
	TraceHull(src, vecAbsEnd, tr);
}

void c_nade_prediction::AddGravityMove(vec3_t& move, vec3_t& vel, float frametime, bool onground)
{
	move = { 0,0,0 };
	move.x = (vel.x) * frametime;
	move.y = (vel.y) * frametime;

	if (onground)
	{
		move.z = (vel.z) * frametime;
	}
	else
	{
		float gravity = 800.0f * 0.4f;
		float newZ = vel.z - (gravity * frametime);
		move.z = ((vel.z + newZ) / 2.0f) * frametime;
		vel.z = newZ;
	}
}

int c_nade_prediction::PhysicsClipVelocity(const vec3_t& in, const vec3_t& normal, vec3_t& out, float overbounce)
{
	static const float STOP_EPSILON = 0.1f;

	float    backoff;
	float    change;
	float    angle;
	int        i, blocked;

	blocked = 0;

	angle = normal[2];

	if (angle > 0)
	{
		blocked |= 1;        // floor
	}
	if (!angle)
	{
		blocked |= 2;        // step
	}

	backoff = in.Dot(normal) * overbounce;

	for (i = 0; i < 3; i++)
	{
		change = normal[i] * backoff;
		out[i] = in[i] - change;
		if (out[i] > -STOP_EPSILON && out[i] < STOP_EPSILON)
		{
			out[i] = 0;
		}
	}

	return blocked;
}

void c_nade_prediction::render()
{
	if (c_menu::get_ptr()->is_open || !interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return;

	if (!c_visuals::get_ptr()->projectile_prediction)
		return;

	if (render_mutex.try_lock())
	{
		saved_points = screen_points;

		render_mutex.unlock();
	}

	if (saved_points.empty())
		return;

	static const auto red_color = color::red();
	static const auto white_color = color::white();
	static const auto black_color = color::black();
	static const auto blue_color = color::blue();

	for (auto& point : saved_points)
	{
		render::draw_line(point.first.x, point.first.y, point.second.x, point.second.y, was_flashed ? black_color : white_color);
	}

	//const auto last_point = saved_points[saved_points.size() - 1].second;
	//render::draw_circle(last_point.x, last_point.y, 5, 180, red_color);

	//const auto& first_point = saved_points.at(0);
	//render::draw_filled_rect(first_point.first.x - 2.5f, first_point.first.y - 2.5f, 5.f, 5.f, was_flashed ? black_color : white_color);
}