#pragma once
#include "../../features.hpp"
#include "../../../menu/variables.hpp"

class c_fakelag : public singleton<c_fakelag>{
public:
	int m_choked; // replace this later, m_nChokedCommands in clientstate is always 0 so this will work for now
				  // cba to fix the clientstate mango
	void think(c_usercmd* cmd, bool& sendPacket);

	bool isEnabled = false;
	int amount = 6;
	std::unordered_map<int, std::string> fakelag_mode_list = { {0, "Default"}, {1, "Adaptive"} };
	std::string mode;
};