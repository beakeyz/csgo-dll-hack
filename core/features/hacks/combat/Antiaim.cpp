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

	if (weapon->client_class()->class_id == cbasecsgrenade)
	{
		bSendPacket = true;
		return;
	}

	handle_yaw(cmd, bSendPacket);
	handle_pitch(cmd);

	if (combat::antiaim::desync)
	{
		bool Moving = csgo::local_player->velocity().length_2d() > 0.1f || (cmd->sidemove != 0.f || cmd->forwardmove != 0.f);
		bool InAir = !(csgo::local_player->flags() & fl_onground);
		bool Standing = !Moving && !InAir;

		int FakeLagTicks = /*Vars.ragebot_fakelag_amt*/ 1;

		if (FakeLagTicks == 0)
		{
			static bool sw = false;
			bSendPacket = sw;
			sw = !sw;
		}

		static vec3_t LastRealAngle = vec3_t{ 0, 0, 0 };
		static float desync_angle = cmd->viewangles.y;

		if (!bSendPacket && !(cmd->buttons & in_attack))
		{
			static bool bFlip = false;
			desync_angle += 90.f;
			cmd->viewangles.y += 90.f;
		}

		if (bSendPacket)
		{
			LastRealAngle = cmd->viewangles;
		}
	}
}

void combat::antiaim::handle_yaw(c_usercmd* cmd, bool fake)
{
	bool Moving = csgo::local_player->velocity().length_2d() > 0.1f || (cmd->sidemove != 0.f || cmd->forwardmove != 0.f);
	bool InAir = !(csgo::local_player->flags() & fl_onground);
	bool Standing = !Moving && !InAir;
	//YawAntiAims mode = (YawAntiAims)Vars.ragebot_antiaim_yaw;

	//float CustomYaw = 0.f;

	//switch (mode)
	//{
	//case YawAntiAims::BACKWARDS:
	//	cmd->viewangles.yaw -= 180.f;
	//	break;

	//case YawAntiAims::SPINBOT:
	//	cmd->viewangles.yaw = fmodf(g_GlobalVars->tickcount * 10.f, 360.f);
	//	break;

	//case YawAntiAims::LOWER_BODY:
	//	cmd->viewangles.yaw = g_LocalPlayer->m_flLowerBodyYawTarget();
	//	break;
	//case YawAntiAims::FREESTANDING:
	//{
	//	float ang = 0.f;
	//	bool canuse = Freestanding(g_LocalPlayer, ang);

	//	if (!canuse)
	//	{
		//	cmd->viewangles.yaw -= 180.f;
	//	}
	//	else
	//	{
	//		cmd->viewangles.yaw = ang;
	//	}

	//	break;
	//}
	//}

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
}

void combat::antiaim::handle_pitch(c_usercmd* cmd)
{
	bool Moving = csgo::local_player->velocity().length_2d() > 0.1f || (cmd->sidemove != 0.f || cmd->forwardmove != 0.f);
	bool InAir = !(csgo::local_player->flags() & fl_onground);
	bool Standing = !Moving && !InAir;
	//PitchAntiAims mode = (PitchAntiAims)Vars.ragebot_antiaim_pitch;

	//float CustomPitch = 0.f;

	/*
	switch (mode)
	{
	case PitchAntiAims::EMOTION:
		cmd->viewangles.pitch = 82.f;
		break;

	case PitchAntiAims::DOWN:
		cmd->viewangles.pitch = 90.f;
		break;

	case PitchAntiAims::UP:
		cmd->viewangles.pitch = -90.f;
		break;

	case PitchAntiAims::ZERO:
		cmd->viewangles.pitch = 0.f;
		break;
	}

	*/

	cmd->viewangles.x = combat::antiaim::pitch;
}