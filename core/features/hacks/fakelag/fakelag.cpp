#include "./fakelag.h"

void c_fakelag::think(c_usercmd* cmd, bool& sendPacket)
{
	if (!csgo::local_player) {
		return;
	}

	g_ctx.globals.send_packet = true;

	static int choke = 0;
	auto local = csgo::local_player;
	if (!this->isEnabled && combat::antiaim::isEnabled) {
		choke = 1;
	}

	if (this->isEnabled) {
		if (this->mode.c_str() == "Default"){
			choke = std::min< int >(this->amount, 14);
		}else if (this->mode.c_str() == "Adaptive"){
			choke = std::min< int >(static_cast<int>(std::ceilf(64 / (csgo::local_player->velocity().length() * interfaces::globals->interval_per_tick))), this->amount);
			
		}
	}

	if (choke > static_cast<int>(interfaces::clientstate->choked_commands)) {
		g_ctx.globals.send_packet = false;
	}
}