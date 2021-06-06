#include "./fakelag.h"

void c_fakelag::think(c_usercmd* cmd, bool& sendPacket)
{
	if (!csgo::local_player) {
		return;
	}

	//sendPacket = true;
	
	static int choke = 0;
	static bool m_sendpacket = false;
	auto local = csgo::local_player;
	if (!combat::fakelag::isEnabled && combat::antiaim::isEnabled) {
		choke = 1;
		sendPacket = true;
	}

	if (combat::fakelag::isEnabled) {
		//switch (g_vars.misc.fakelag.type) {
		//case 0: { // Maximum
		//choke = std::min< int >(combat::fakelag::amount, 14);
		//	break;
		//}
		//case 1: { // Adaptive
		//	choke = std::min<int>(static_cast<int>(std::ceilf(64 / (csgo::local_player->velocity().length() * interfaces::globals->interval_per_tick))), combat::fakelag::amount);
		//	break;
		//}
		//}

		choke++;
		if (choke > combat::fakelag::amount * 5) {
			
			sendPacket = true;
			choke = 0;
		}
		else {
			sendPacket = false;
		}

		
		//sendPacket = m_sendpacket;
		//m_sendpacket = !m_sendpacket;
	}

	//if (choke > static_cast<int>(interfaces::clientstate->choked_commands)) {
		//sendPacket = false;
	//}
	
}

//define global external to make the linker shut up :D
c_fakelag g_fakelag;