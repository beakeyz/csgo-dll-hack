#pragma once
#include "../features.hpp"
#include "../../menu/variables.hpp"

class c_fakelag {
private:
	enum type : int {
		MAXIMUM = 0,
		ADAPTIVE,
	};
public:
	int m_choked; // replace this later, m_nChokedCommands in clientstate is always 0 so this will work for now
				  // cba to fix the clientstate mango
	void think(c_usercmd* cmd, bool& sendPacket);
};

extern c_fakelag g_fakelag;