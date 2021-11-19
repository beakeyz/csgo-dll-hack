#include "../../features.hpp"
#include "../../../menu/variables.hpp"

vec3_t calculateRelativeAngle(const vec3_t& source, const vec3_t& destination, const vec3_t& viewAngles) noexcept
{
	return ((destination - source).toAngle() - viewAngles).normalized();
}

void combat::aimbot::aim_bot(c_usercmd* cmd) {
	if (!combat::aimbot::isEnabled)
		return;
	if (!csgo::local_player || !(uintptr_t*)csgo::local_player || csgo::local_player->next_attack() > interfaces::globals->curtime || csgo::local_player->is_defusing() || csgo::local_player->health() <= 0)
		return;

	const auto curr_weapon = csgo::local_player->active_weapon();
	if (!curr_weapon || !curr_weapon->clip1_count() || !curr_weapon->clip2_count())
		return;


	if (cmd->buttons & in_attack) {

		//TODO values
		float max = 180;
		vec3_t current = { 0, 0, 0 };
		const auto localPlayerEyePosition = csgo::local_player->get_eye_pos();
		vec3_t* aimPunch = csgo::local_player->active_weapon()->get_weapon_data()->bFullAuto ? &csgo::local_player->aim_punch_angle() : new vec3_t{ 0, 0, 0 };

		for (short i = 1; i < 64; i++) {
			entity_t* e = static_cast<entity_t*>(interfaces::entity_list->get_client_entity(i));

			if (!e || !(uintptr_t*)e || !csgo::local_player || !(uintptr_t*)csgo::local_player || e == csgo::local_player)
				continue;

			if (e->dormant() || !e->is_player() || e->health() == 0 || e->team() == csgo::local_player->team())
				continue;

			const auto angle = calculateRelativeAngle(localPlayerEyePosition, e->getBonePos(8), cmd->viewangles + *aimPunch);

			float fov = std::hypot(angle.x, angle.y);
			if (fov > max)
				continue;

			if (fov < max) {
				max = fov;
				current = e->getBonePos(8);
			}
		}

		if (current.noNull()) {

			static vec3_t* lastAngles{ &cmd->viewangles };
			static int lastCommand{ };

			if (lastCommand == cmd->command_number - 1 && lastAngles->noNull() /*&& variables::aimbot::silent*/)
				cmd->viewangles = *lastAngles;

			auto angle = calculateRelativeAngle(localPlayerEyePosition, current, cmd->viewangles + *aimPunch);
			bool clamped{ false };

			if (std::abs(angle.x) > 255 || std::abs(angle.y) > 255) {
				angle.x = std::clamp((int)angle.x, -255, 255);
				angle.y = std::clamp((int)angle.y, -255, 255);
				clamped = true;
			}

			angle -= *aimPunch;
			cmd->viewangles += angle;
			//if (!variables::aimbot::silent) {
			//	interfaces::engine->set_view_angles(cmd->viewangles);
			//}

			if (clamped)
				cmd->buttons &= in_attack;

			if (clamped)lastAngles = &cmd->viewangles;
			else lastAngles = new vec3_t{ 0, 0, 0 };
			lastCommand = cmd->command_number;
		}
	}
}