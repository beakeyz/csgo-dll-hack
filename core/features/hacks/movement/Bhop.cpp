#include "../../../menu/variables.hpp"
#include "../../features.hpp"

void movement::bunny_hop::bunny_hop(c_usercmd* cmd) {

	static convar* sv_autobunnyhopping = interfaces::console->get_convar("sv_autobunnyhopping");
	sv_autobunnyhopping->set_value(movement::bunny_hop::isEnabled);
	static convar* sv_enablebunnyhopping = interfaces::console->get_convar("sv_enablebunnyhopping");
	sv_enablebunnyhopping->set_value(movement::bunny_hop::isEnabled);

	if (csgo::local_player->move_type() == movetype_ladder || csgo::local_player->move_type() == movetype_noclip) {
		return;
	}

	if ((cmd->buttons & in_jump) && !(csgo::local_player->flags() & fl_onground)) {
		// bhop.
		if (movement::bunny_hop::isEnabled)
			cmd->buttons &= ~in_jump;

		// duck jump ( crate jump ).
		//if (g_cfg.misc.crouch_in_air)
		//	g_ctx.get_command()->m_buttons |= IN_DUCK;
	}

	/*
	if (!movement::bunny_hop::isEnabled)
		return;

	static bool jumped_last_tick = false;
	static bool should_fake_jump = false;

	if (!jumped_last_tick && should_fake_jump) {
		should_fake_jump = false;
		cmd->buttons |= in_jump;
	}
	else if (cmd->buttons & in_jump) {
		if (csgo::local_player->flags() & fl_onground) {
			jumped_last_tick = true;
			should_fake_jump = true;
		}
		else {
			cmd->buttons &= ~in_jump;
			jumped_last_tick = false;
		}
	}
	else {
		jumped_last_tick = false;
		should_fake_jump = false;
	}
	*/
}