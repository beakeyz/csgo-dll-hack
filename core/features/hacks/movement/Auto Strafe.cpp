#include "../../../menu/variables.hpp"
#include "../../features.hpp"

#define CheckIfNonValidNumber(x) (fpclassify(x) == FP_INFINITE || fpclassify(x) == FP_NAN || fpclassify(x) == FP_SUBNORMAL)

template<class T, class U>
T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}

void movement::auto_strafe::auto_strafe(c_usercmd* cmd) {

	auto local = csgo::local_player;
	if (!local || !local->is_alive())
		return;

	if (local->flags() & fl_onground && !(cmd->buttons & in_jump))
		return;

	if (local->move_type() & (movetype_ladder | movetype_noclip))
		return;

	static float side_speed = interfaces::console->get_convar("cl_sidespeed")->float_value;

	/*
	vec3_t viewangles;
	interfaces::engine->get_view_angles(viewangles);

	static bool side_switch = false;
	side_switch = !side_switch;

	cmd->forwardmove = 0.f;
	cmd->sidemove = side_switch ? 450.f : -450.f;

	float velocity_yaw = local->velocity().y;

	float rotation = clamp<float>(math::rad_to_deg(std::atan2(15.f, local->velocity().length_2d())), 0.f, 90.f);

	float abs_yaw = std::fabs(math::normalize_angle(velocity_yaw - viewangles.y));

	float ideal_yaw_rotation = (side_switch ? rotation : -rotation) + (abs_yaw < 5.f ? velocity_yaw : viewangles.y);

	float cos_rot = std::cos(math::deg_to_rad(viewangles.y - ideal_yaw_rotation));
	float sin_rot = std::sin(math::deg_to_rad(viewangles.y - ideal_yaw_rotation));

	cmd->forwardmove = (cos_rot * cmd->forwardmove) - (sin_rot * cmd->sidemove);
	cmd->sidemove = (sin_rot * cmd->forwardmove) + (cos_rot * cmd->sidemove);
	*/

	vec3_t engine_angles;
	interfaces::engine->get_view_angles(engine_angles);

	auto velocity = csgo::local_player->velocity();

	cmd->forwardmove = min(5850.0f / velocity.length_2d(), side_speed);
	cmd->sidemove= cmd->command_number % 2 ? side_speed : -side_speed;

	vec3_t tmp = { 0.0f, 0.0f, 0.0f };

	auto yaw_velocity = math::calculate_angle(tmp, velocity).y;
	auto ideal_rotation = math::clamp(RAD2DEG(atan2(15.0f, velocity.length_2d())), 0.0f, 45.0f);

	auto yaw_rotation = fabs(yaw_velocity - engine_angles.y) + (cmd->command_number % 2 ? ideal_rotation : -ideal_rotation);
	auto ideal_yaw_rotation = yaw_rotation < 5.0f ? yaw_velocity : engine_angles.y;

	utilities::rotate_movement(cmd, ideal_yaw_rotation);
}