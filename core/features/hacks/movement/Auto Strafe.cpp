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

	static auto old_yaw = 0.0f;

	auto get_velocity_degree = [](float velocity)
	{
		auto tmp = RAD2DEG(atan(30.0f / velocity));

		if (CheckIfNonValidNumber(tmp) || tmp > 90.0f)
			return 90.0f;

		else if (tmp < 0.0f)
			return 0.0f;
		else
			return tmp;
	};

	if (csgo::local_player->move_type() != movetype_walk)
		return;

	auto velocity = csgo::local_player->velocity();
	velocity.z = 0.0f;

	auto forwardmove = cmd->forwardmove;
	auto sidemove = cmd->sidemove;

	if (velocity.length_2d() < 5.0f && !forwardmove && !sidemove)
		return;

	static auto flip = false;
	flip = !flip;

	auto turn_direction_modifier = flip ? 1.0f : -1.0f;
	auto viewangles = cmd->viewangles;

	if (forwardmove || sidemove)
	{
		cmd->forwardmove = 0.0f;
		cmd->sidemove = 0.0f;

		auto turn_angle = atan2(-sidemove, forwardmove);
		viewangles.y += turn_angle * M_RADPI;
	}
	else if (forwardmove)
		cmd->forwardmove = 0.0f;

	auto strafe_angle = RAD2DEG(atan(15.0f / velocity.length_2d()));

	if (strafe_angle > 90.0f)
		strafe_angle = 90.0f;
	else if (strafe_angle < 0.0f)
		strafe_angle = 0.0f;

	auto temp = vec3_t(0.0f, viewangles.y - old_yaw, 0.0f);
	temp.y = math::normalize_yaw(temp.y);

	auto yaw_delta = temp.y;
	old_yaw = viewangles.y;

	auto abs_yaw_delta = fabs(yaw_delta);

	if (abs_yaw_delta <= strafe_angle || abs_yaw_delta >= 30.0f)
	{
		vec3_t velocity_angles;
		math::vector_angles(velocity, velocity_angles);

		temp = vec3_t(0.0f, viewangles.y - velocity_angles.y, 0.0f);
		temp.y = math::normalize_yaw(temp.y);

		auto velocityangle_yawdelta = temp.y;
		auto velocity_degree = get_velocity_degree(velocity.length_2d());

		if (velocityangle_yawdelta <= velocity_degree || velocity.length_2d() <= 15.0f)
		{
			if (-velocity_degree <= velocityangle_yawdelta || velocity.length_2d() <= 15.0f)
			{
				viewangles.y += strafe_angle * turn_direction_modifier;
				cmd->sidemove = side_speed * turn_direction_modifier;
			}
			else
			{
				viewangles.y = velocity_angles.y - velocity_degree;
				cmd->sidemove = side_speed;
			}
		}
		else
		{
			viewangles.y = velocity_angles.y + velocity_degree;
			cmd->sidemove = -side_speed;
		}
	}
	else if (yaw_delta > 0.0f)
		cmd->sidemove = -side_speed;
	else if (yaw_delta < 0.0f)
		cmd->sidemove = side_speed;

	auto move = vec3_t(cmd->forwardmove, cmd->sidemove, 0.0f);
	auto speed = move.length();

	vec3_t angles_move;
	math::vector_angles(move, angles_move);

	auto normalized_x = fmod(cmd->viewangles.x + 180.0f, 360.0f) - 180.0f;
	auto normalized_y = fmod(cmd->viewangles.y + 180.0f, 360.0f) - 180.0f;

	auto yaw = DEG2RAD(normalized_y - viewangles.y + angles_move.y);

	if (normalized_x >= 90.0f || normalized_x <= -90.0f || cmd->viewangles.x >= 90.0f && cmd->viewangles.x <= 200.0f || cmd->viewangles.x <= -90.0f && cmd->viewangles.x <= 200.0f) //-V648
		cmd->forwardmove = -cos(yaw) * speed;
	else
		cmd->forwardmove = cos(yaw) * speed;

	cmd->sidemove = sin(yaw) * speed;
}