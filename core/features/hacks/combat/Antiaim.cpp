#include "../../features.hpp"
#include "../../../menu/variables.hpp"
#include <stdlib.h> 

void combat::antiaim::run(c_usercmd* cmd, bool& bSendPacket)
{
	if (!combat::antiaim::isEnabled)
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
	{
		return;
	}

	int movetype = csgo::local_player->move_type();

	if (
		movetype == movetype_fly
		|| movetype == movetype_noclip
		|| movetype == movetype_ladder
		|| cmd->buttons & in_use
		|| cmd->buttons & in_grenade1
		|| cmd->buttons & in_grenade2
		)
	{
		return;
	}

	weapon_t* weapon = csgo::local_player->active_weapon();

	if (!weapon)
	{
		return;
	}

	if (weapon->next_primary_attack() - interfaces::globals->curtime < interfaces::globals->interval_per_tick && (cmd->buttons & in_attack || cmd->buttons & in_attack2))
	{
		return;
	}

	if (weapon->get_weapon_data()->nWeaponType == WEAPONTYPE_GRENADE)
	{
		return;
	}

	handle_yaw(cmd, bSendPacket);
	handle_pitch(cmd);

	if (combat::antiaim::desync)
	{
		
	}
}

void combat::antiaim::handle_yaw(c_usercmd* cmd, bool fake)
{
	bool Moving = csgo::local_player->velocity().length_2d() > 0.1f || (cmd->sidemove != 0.f || cmd->forwardmove != 0.f);
	bool InAir = !(csgo::local_player->flags() & fl_onground);
	bool Standing = !Moving && !InAir;

	if (combat::antiaim::spinbot) {
		cmd->viewangles.y = fmodf(interfaces::globals->tickcount * (float)combat::antiaim::spinspeed, 360.f);
	}
	else {
		if (combat::antiaim::backwards) {
			cmd->viewangles.y -= 180.F;
		}

		if (combat::antiaim::jitter) {
			cmd->viewangles.y += (rand() % combat::antiaim::jitter_amount) * 3.f;
		}
		else
		{
			cmd->viewangles.y = combat::antiaim::yaw;
		}
	}
	//idk if this does sm
	math::clamp(cmd->viewangles.y, -180, 180);
}

void combat::antiaim::handle_pitch(c_usercmd* cmd)
{
	bool Moving = csgo::local_player->velocity().length_2d() > 0.1f || (cmd->sidemove != 0.f || cmd->forwardmove != 0.f);
	bool InAir = !(csgo::local_player->flags() & fl_onground);
	bool Standing = !Moving && !InAir;

	cmd->viewangles.x = combat::antiaim::pitch;
}