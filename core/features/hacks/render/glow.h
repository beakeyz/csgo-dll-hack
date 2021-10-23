#pragma once
#include "../../features.hpp"
#include "../../../menu/variables.hpp"

class c_glow : public singleton<c_glow> {

public:
	void endGlow();
	void runGlow();

	std::unordered_map<int, std::string> color_map = { {0, "Red"}, {1, "Orange"}, {2, "Yellow"}, {3, "Green"}, {4, "Cyan"}, {5, "Blue"}, {6, "Purple"}, {7, "Pink"},  {8, "White"}, {9, "Black"} };

	bool teammates;
	bool enemies;
		
	std::string team_color;
	std::string enemy_color;

	
};