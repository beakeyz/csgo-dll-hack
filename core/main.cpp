#include "../dependencies/utilities/csgo.hpp"
#include "features/features.hpp"
#include "../core/menu/variables.hpp"
#include "../core/menu/menu.hpp"
#include "../core/menu/utils/gui_utils.h"
#include "../core/features/hacks/features.h"

unsigned long WINAPI release() {
	hooks::release();

#ifdef _DEBUG
	console::release();
#endif
	return TRUE;
}

unsigned long WINAPI initialize(void* instance) {
	while (!GetModuleHandleA("serverbrowser.dll"))
		Sleep(200);

#ifdef _DEBUG
	console::initialize("csgo-cheat console");
#endif

	try
	{
		interfaces::initialize();
		render::initialize();
		hooks::initialize();

	}
	catch (const std::runtime_error & error) {
		MessageBoxA(nullptr, error.what(), "error while initializing hooks!", MB_OK | MB_ICONERROR);
		FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
	}

	groupBox box(e_position::LEFT, 80, 0, render::fonts::watermark_font_other, "Combat", e_boxtype::COMBAT, 0);

	buttonComponent Aimbot_checkboxComp(&box, -1, "Aimbot", render::fonts::watermark_font_small, combat::aimbot::isEnabled, 0);
	buttonComponent Trigger_comp(&box, -1, "Triggerbot", render::fonts::watermark_font_small, combat::triggerbot::enabled, 1);
	//buttonComponent Resolver_comp(&box, "Resolver", render::fonts::watermark_font_small, combat::resolver::enabled, 2);
	buttonComponent anti_aim_comp(&box, -1, "Anti aim", render::fonts::watermark_font_small, combat::antiaim::isEnabled, 3);
	buttonComponent desync_comp(&box, -1, "Desync", render::fonts::watermark_font_small, combat::antiaim::desync, 4);
	buttonComponent backwards_comp(&box, -1, "Backwards", render::fonts::watermark_font_small, combat::antiaim::backwards, 5);
	buttonComponent jitter_comp(&box, -1, "Jitter", render::fonts::watermark_font_small, combat::antiaim::jitter, 6);
	c_slider_component jitter_amount_comp(&box, -1, "Jitter Amount", render::fonts::watermark_font_small, combat::antiaim::jitter_amount, 1, 20, 7);
	buttonComponent spin_comp(&box, -1, "Spinbot", render::fonts::watermark_font_small, combat::antiaim::spinbot, 8);
	c_slider_component spinspeed_comp(&box, -1, "Spinspeed", render::fonts::watermark_font_small, combat::antiaim::spinspeed, 1, 20, 9);
	c_slider_component yaw_comp(&box, -1, "yaw", render::fonts::watermark_font_small, combat::antiaim::yaw, -180, 180, 10);
	c_slider_component pitch_comp(&box, -1, "pitch", render::fonts::watermark_font_small, combat::antiaim::pitch, -90, 90, 11);


	groupBox fakelagbox (e_position::LEFT, 80, 0, render::fonts::watermark_font_other, "Fakelag", e_boxtype::FAKELAG, 1);
	
	buttonComponent fakelag_enabled(&fakelagbox, -1, "enabled", render::fonts::watermark_font_small, c_fakelag::get_ptr()->isEnabled, 0);
	c_slider_component fakelag_amount(&fakelagbox, -1, "Lag amount", render::fonts::watermark_font_small, c_fakelag::get_ptr()->amount, 2, 10, 1);

	c_mode_picker fakelag_mode(&fakelagbox, -1, "Mode", render::fonts::watermark_font_small, c_fakelag::get_ptr()->fakelag_mode_list, "Default", c_fakelag::get_ptr()->mode, 2);
	 

	groupBox ragebot_box (e_position::LEFT, 80, 0, render::fonts::watermark_font_other, "Ragebot", e_boxtype::RAGEBOT, 2);

	buttonComponent rb_enabled(&ragebot_box, -1, "Enabled", render::fonts::watermark_font_small, combat::ragebot::isEnabled, 0);
	buttonComponent rb_auto_fire(&ragebot_box, -1, "Auto fire", render::fonts::watermark_font_small, combat::ragebot::auto_fire, 1);
	buttonComponent rb_silent(&ragebot_box, -1, "Silent", render::fonts::watermark_font_small, combat::ragebot::silent, 2);
	buttonComponent rb_quickstop(&ragebot_box, -1, "Quickstop", render::fonts::watermark_font_small, combat::ragebot::quickstop, 3);
	buttonComponent rb_teammates(&ragebot_box, -1, "Teammates", render::fonts::watermark_font_small, combat::ragebot::teammates, 4);
	c_slider_component rb_hitchance(&ragebot_box, -1, "Hitchance", render::fonts::watermark_font_small, combat::ragebot::hit_chance, 1, 100, 5);
	c_slider_component rb_min_dmg(&ragebot_box, -1, "Minimum damage", render::fonts::watermark_font_small, combat::ragebot::min_dmg, 1, 100, 6);
	buttonComponent rb_dynamic_hitbox(&ragebot_box, -1, "Dynamic hitbox targeting", render::fonts::watermark_font_small, combat::ragebot::dynamic_hitbox, 7);
	buttonComponent rb_head(&ragebot_box, -1, "Head hitbox targeting", render::fonts::watermark_font_small, combat::ragebot::head, 8);
	buttonComponent rb_pelvis(&ragebot_box, -1, "Pelvis hitbox targeting", render::fonts::watermark_font_small, combat::ragebot::pelvis, 9);
	buttonComponent rb_arms(&ragebot_box, -1, "Arms hitbox targeting", render::fonts::watermark_font_small, combat::ragebot::arms, 10);
	buttonComponent rb_legs(&ragebot_box, -1, "Legs hitbox targeting", render::fonts::watermark_font_small, combat::ragebot::legs, 11);

	groupBox PlayerEspBox(e_position::LEFT, 80, 0, render::fonts::watermark_font_other, "Player esp", e_boxtype::PLAYER_ESP, 0);

	c_button_classifier target_type(&PlayerEspBox, { {0,0,0,0,0,"Teammates"}, {0,0,0,0,1,"Enemies"} }, "", render::fonts::watermark_font_other, 0);

	buttonComponent player_esp_box_team(&PlayerEspBox, 0, "Bounding boxes", render::fonts::watermark_font_small, c_player_esp::get_ptr()->type[0].boxes, 1);
	buttonComponent player_esp_name_team(&PlayerEspBox, 0, "Name", render::fonts::watermark_font_small, c_player_esp::get_ptr()->type[0].names, 2);

	buttonComponent player_esp_box_enemy(&PlayerEspBox, 1, "Bounding boxes", render::fonts::watermark_font_small, c_player_esp::get_ptr()->type[1].boxes, 3);
	buttonComponent player_esp_name_enemy(&PlayerEspBox, 1, "Name", render::fonts::watermark_font_small, c_player_esp::get_ptr()->type[1].names, 4);

	groupBox Chamsbox (e_position::LEFT, 80, 0, render::fonts::watermark_font_other, "Chams", e_boxtype::CHAMS, 1);

	buttonComponent Enemies(&Chamsbox, -1, "Enemies", render::fonts::watermark_font_small, c_chams::get_ptr()->enemies, 0);
	buttonComponent Teamates(&Chamsbox, -1, "Teammates", render::fonts::watermark_font_small, c_chams::get_ptr()->teammates, 1);
	buttonComponent Hands(&Chamsbox, -1, "Arms", render::fonts::watermark_font_small, c_chams::get_ptr()->hands, 2);
	buttonComponent Weapons(&Chamsbox, -1, "Weapons", render::fonts::watermark_font_small, c_chams::get_ptr()->weapons, 3);

	c_mode_picker Material_enemy(&Chamsbox, -1, "Enemy material", render::fonts::watermark_font_small, c_chams::get_ptr()->material_map, "Normal", c_chams::get_ptr()->enemy_material, 4);
	c_mode_picker Material_team(&Chamsbox, -1, "Team material", render::fonts::watermark_font_small, c_chams::get_ptr()->material_map, "Normal", c_chams::get_ptr()->team_material, 5);
	c_mode_picker Material_arms(&Chamsbox, -1, "Arm material", render::fonts::watermark_font_small, c_chams::get_ptr()->material_map, "Normal", c_chams::get_ptr()->arms_material, 6);
	c_mode_picker Material_weapon(&Chamsbox, -1, "Weapon material", render::fonts::watermark_font_small, c_chams::get_ptr()->material_map, "Normal", c_chams::get_ptr()->weapons_material, 7);
	
	c_mode_picker color_enemy(&Chamsbox, -1, "Enemy unoccluded color", render::fonts::watermark_font_small, c_chams::get_ptr()->color_map, "Red", c_chams::get_ptr()->enemy_color, 8);
	c_mode_picker color_occluded_enemy(&Chamsbox, -1, "Enemy occluded color", render::fonts::watermark_font_small, c_chams::get_ptr()->color_map, "Yellow", c_chams::get_ptr()->enemy_color_occluded, 9);

	c_mode_picker color_team(&Chamsbox, -1, "Team unoccluded color", render::fonts::watermark_font_small, c_chams::get_ptr()->color_map, "Green", c_chams::get_ptr()->team_color, 10);
	c_mode_picker color_occluded_team(&Chamsbox, -1, "Team occluded color", render::fonts::watermark_font_small, c_chams::get_ptr()->color_map, "Cyan", c_chams::get_ptr()->team_color_occluded, 11);

	c_mode_picker color_weapon(&Chamsbox, -1, "Weapon unoccluded color", render::fonts::watermark_font_small, c_chams::get_ptr()->color_map, "Purple", c_chams::get_ptr()->weapons_color, 12);
	c_mode_picker color_occluded_weapon(&Chamsbox, -1, "Weapon occluded color", render::fonts::watermark_font_small, c_chams::get_ptr()->color_map, "Pink", c_chams::get_ptr()->weapons_color_occluded, 13);

	c_mode_picker color_arms(&Chamsbox, -1, "Arm color", render::fonts::watermark_font_small, c_chams::get_ptr()->color_map, "Pink", c_chams::get_ptr()->arms_color, 14);

	groupBox visual_box (e_position::LEFT, 80, 0, render::fonts::watermark_font_other, "Visuals", e_boxtype::VISUALS, 2);

	buttonComponent remove_visual_recoil(&visual_box, -1, "remove visual recoil", render::fonts::watermark_font_small, c_visuals::get().visual_recoil, 0);
	buttonComponent full_bright(&visual_box, -1, "fullbright", render::fonts::watermark_font_small, c_visuals::get().m_full_bright, 1);
	buttonComponent edit_smoke(&visual_box, -1, "edit smoke", render::fonts::watermark_font_small, c_visuals::get().m_edit_smoke, 2);
	buttonComponent smoke_wire(&visual_box, -1, "smoke wireframe", render::fonts::watermark_font_small, c_visuals::get().smoke_wireframe, 3);

	groupBox esp_box(e_position::LEFT, 80, 0, render::fonts::watermark_font_other, "skeleton esp", e_boxtype::PLAYER_ESP, 3);

	buttonComponent skeleton_esp(&esp_box, -1, "Teammates", render::fonts::watermark_font_small, c_skeleton_esp::get_ptr()->teammates, 0);
	buttonComponent skeleton_esp_enemies(&esp_box, -1, "Enemies", render::fonts::watermark_font_small, c_skeleton_esp::get_ptr()->enemies, 1);
	c_mode_picker seketon_esp_color_team(&esp_box, -1, "Team color", render::fonts::watermark_font_small, c_skeleton_esp::get_ptr()->color_map, "Green", c_skeleton_esp::get_ptr()->team_color, 2);
	c_mode_picker seketon_esp_color_enemy(&esp_box, -1, "Enemy color", render::fonts::watermark_font_small, c_skeleton_esp::get_ptr()->color_map, "Red", c_skeleton_esp::get_ptr()->enemy_color, 3);

	groupBox glow_box(e_position::LEFT, 80, 0, render::fonts::watermark_font_other, "Glow", e_boxtype::VISUALS, 4);

	buttonComponent glow_esp(&glow_box, -1, "Teammates", render::fonts::watermark_font_small, c_glow::get_ptr()->teammates, 0);
	buttonComponent glow_esp_enemies(&glow_box, -1, "Enemies", render::fonts::watermark_font_small, c_glow::get_ptr()->enemies, 1);
	c_mode_picker glow_esp_color_team(&glow_box, -1, "Team color", render::fonts::watermark_font_small, c_glow::get_ptr()->color_map, "Green", c_glow::get_ptr()->team_color, 2);
	c_mode_picker glow_esp_color_enemy(&glow_box, -1, "Enemy color", render::fonts::watermark_font_small, c_glow::get_ptr()->color_map, "Red", c_glow::get_ptr()->enemy_color, 3);

	groupBox gen_misc_box(e_position::LEFT, 80, 0, render::fonts::watermark_font_other, "General Misc", e_boxtype::CHAMS, 0);
	buttonComponent thirdperson_btn(&gen_misc_box, -1, "ThirdPerson", render::fonts::watermark_font_small, c_misc::get_ptr()->m_third_person, 0);
	buttonComponent thirdperson_disable_btn(&gen_misc_box, -1, "ThirdPerson disable on weapon", render::fonts::watermark_font_small, c_misc::get_ptr()->m_third_person_disable_on_weapon, 1);

	c_menu::get_ptr()->combat_btn.m_boxes[box.index] = &box;
	c_menu::get_ptr()->combat_btn.m_boxes[fakelagbox.index] = &fakelagbox;
	c_menu::get_ptr()->combat_btn.m_boxes[ragebot_box.index] = &ragebot_box;

	c_menu::get_ptr()->visuals_btn.m_boxes[PlayerEspBox.index] = &PlayerEspBox;
	c_menu::get_ptr()->visuals_btn.m_boxes[Chamsbox.index] = &Chamsbox;
	c_menu::get_ptr()->visuals_btn.m_boxes[visual_box.index] = &visual_box;
	c_menu::get_ptr()->visuals_btn.m_boxes[esp_box.index] = &esp_box;
	c_menu::get_ptr()->visuals_btn.m_boxes[glow_box.index] = &glow_box;

	c_menu::get_ptr()->misc_btn.m_boxes[gen_misc_box.index] = &gen_misc_box;

	while (true) {

		if (GetAsyncKeyState(VK_END))
		{
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));

	}

	if (c_menu::get_ptr()->is_open) {
		c_menu::get_ptr()->is_open = false;
		//wait for paint_traverse::hook to be called and restore input.
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	release();

	FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
}

std::int32_t WINAPI DllMain(const HMODULE instance [[maybe_unused]], const unsigned long reason, const void* reserved [[maybe_unused]] ) {
	DisableThreadLibraryCalls(instance);

	switch (reason) {
		case DLL_PROCESS_ATTACH: {
			if (auto handle = CreateThread(nullptr, NULL, initialize, instance, NULL, nullptr))
				CloseHandle(handle);

			break;
		}

		case DLL_PROCESS_DETACH: {
			release();
			break;
		}
	}

	return true;
}
