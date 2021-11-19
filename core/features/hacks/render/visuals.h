#pragma once
#include "../../features.hpp"
#include "../../../menu/variables.hpp"

class c_visuals : public singleton<c_visuals> {

public:
	void run_visuals();
	void remove_smoke();
	void nightmode(entity_t* ent, float override_brightness = 0.f);
	
public:
	std::unordered_map<int, std::string> color_map = { {0, "Red"}, {1, "Orange"}, {2, "Yellow"}, {3, "Green"}, {4, "Cyan"}, {5, "Blue"}, {6, "Purple"}, {7, "Pink"},  {8, "White"}, {9, "Black"} };

	std::string dropped_weapon_ammo_color;

	//world extra
	bool m_edit_smoke = false;
	int m_nightmode = 1.0F;
	//world general
	bool dropped_weapons;
	bool dropped_weapons_ammo;
	bool projectile_prediction;
	bool projectiles;

	//Effects
	bool visual_recoil = false;
};