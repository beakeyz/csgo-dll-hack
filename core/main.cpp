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

	groupBox box(20, 80, variables::menu::w / 2 - 20, 220, render::fonts::watermark_font_other, "Combat", e_boxtype::COMBAT);

	buttonComponent Aimbot_checkboxComp(&box, "Aimbot", render::fonts::watermark_font_small, combat::aimbot::isEnabled, 0);
	buttonComponent Trigger_comp(&box, "Triggerbot", render::fonts::watermark_font_small, combat::triggerbot::enabled, 1);
	buttonComponent Resolver_comp(&box, "Resolver", render::fonts::watermark_font_small, combat::resolver::enabled, 2);
	buttonComponent anti_aim_comp(&box, "Anti aim", render::fonts::watermark_font_small, combat::antiaim::isEnabled, 3);
	buttonComponent desync_comp(&box, "Desync", render::fonts::watermark_font_small, combat::antiaim::desync, 4);
	buttonComponent backwards_comp(&box, "Backwards", render::fonts::watermark_font_small, combat::antiaim::backwards, 5);
	buttonComponent jitter_comp(&box, "Jitter", render::fonts::watermark_font_small, combat::antiaim::jitter, 6);
	c_slider_component jitter_amount_comp(&box, "Jitter Amount", render::fonts::watermark_font_small, combat::antiaim::jitter_amount, 1, 20, 7);
	buttonComponent spin_comp(&box, "Spinbot", render::fonts::watermark_font_small, combat::antiaim::spinbot, 8);
	c_slider_component spinspeed_comp(&box, "Spinspeed", render::fonts::watermark_font_small, combat::antiaim::spinspeed, 1, 20, 9);
	c_slider_component yaw_comp(&box, "yaw", render::fonts::watermark_font_small, combat::antiaim::yaw, -180, 180, 10);
	c_slider_component pitch_comp(&box, "pitch", render::fonts::watermark_font_small, combat::antiaim::pitch, -90, 90, 11);


	groupBox fakelagbox (20, 310, variables::menu::w / 2 - 20, 80, render::fonts::watermark_font_other, "Fakelag", e_boxtype::FAKELAG);

	buttonComponent fakelag_enabled(&fakelagbox, "enabled", render::fonts::watermark_font_small, combat::fakelag::isEnabled, 0);
	c_slider_component fakelag_amount(&fakelagbox, "Lag amount", render::fonts::watermark_font_small, combat::fakelag::amount, 2, 10, 1);

	 
	groupBox ragebot_box (variables::menu::w / 2 + 10, 80, variables::menu::w / 2 - 30, 190, render::fonts::watermark_font_other, "Ragebot", e_boxtype::RAGEBOT);

	buttonComponent rb_enabled(&ragebot_box, "Enabled", render::fonts::watermark_font_small, combat::ragebot::isEnabled, 0);
	buttonComponent rb_auto_fire(&ragebot_box, "Auto fire", render::fonts::watermark_font_small, combat::ragebot::auto_fire, 1);
	buttonComponent rb_silent(&ragebot_box, "Silent", render::fonts::watermark_font_small, combat::ragebot::silent, 2);
	buttonComponent rb_quickstop(&ragebot_box, "Quickstop", render::fonts::watermark_font_small, combat::ragebot::quickstop, 3);
	buttonComponent rb_teammates(&ragebot_box, "Teammates", render::fonts::watermark_font_small, combat::ragebot::teammates, 4);
	c_slider_component rb_hitchance(&ragebot_box, "Hitchance", render::fonts::watermark_font_small, combat::ragebot::hit_chance, 1, 100, 5);
	c_slider_component rb_min_dmg(&ragebot_box, "Minimum damage", render::fonts::watermark_font_small, combat::ragebot::min_dmg, 1, 100, 6);
	buttonComponent rb_dynamic_hitbox(&ragebot_box, "Dynamic hitbox targeting", render::fonts::watermark_font_small, combat::ragebot::dynamic_hitbox, 7);
	buttonComponent rb_head(&ragebot_box, "Head hitbox targeting", render::fonts::watermark_font_small, combat::ragebot::head, 8);
	buttonComponent rb_pelvis(&ragebot_box, "Pelvis hitbox targeting", render::fonts::watermark_font_small, combat::ragebot::pelvis, 9);
	buttonComponent rb_arms(&ragebot_box, "Arms hitbox targeting", render::fonts::watermark_font_small, combat::ragebot::arms, 10);
	buttonComponent rb_legs(&ragebot_box, "Legs hitbox targeting", render::fonts::watermark_font_small, combat::ragebot::legs, 11);


	groupBox Chamsbox (20, 80, variables::menu::w / 2 - 20, 220, render::fonts::watermark_font_other, "Chams", e_boxtype::CHAMS);

	buttonComponent chams_enabled(&Chamsbox, "Enabled", render::fonts::watermark_font_small, Render::Chams::isEnabled, 0);
	c_slider_component Material(&Chamsbox, "Material", render::fonts::watermark_font_small, Render::Chams::material, 1, 7, 1);
	buttonComponent Enemies(&Chamsbox, "Enemies", render::fonts::watermark_font_small, Render::Chams::enemies, 2);
	buttonComponent Teamates(&Chamsbox, "Teammates", render::fonts::watermark_font_small, Render::Chams::teammates, 3);
	buttonComponent Hands(&Chamsbox, "Hands", render::fonts::watermark_font_small, Render::Chams::hands, 4);
	c_color_picker team_color_invisible(&Chamsbox, "Team Color Invisible", render::fonts::watermark_font_small, Render::Chams::team_color_invisible, 5);
	c_color_picker team_color_visible(&Chamsbox, "Team Color Visible", render::fonts::watermark_font_small, Render::Chams::team_color_visible, 6);


	groupBox esp_box (variables::menu::w / 2 + 10, 80, variables::menu::w / 2 - 30, 190, render::fonts::watermark_font_other, "Player esp", e_boxtype::PLAYER_ESP);

	buttonComponent skeleton_esp(&esp_box, "Skeleton Esp", render::fonts::watermark_font_small, g_player_esp.is_enabled, 0);


	groupBox visual_box (20, 80 + Chamsbox.getHeight() + 10, variables::menu::w / 2 - 20, 20, render::fonts::watermark_font_other, "Visuals", e_boxtype::VISUALS);

	buttonComponent remove_visual_recoil(&visual_box, "remove visual recoil", render::fonts::watermark_font_small, c_visuals::get().visual_recoil, 0);
	buttonComponent full_bright(&visual_box, "fullbright", render::fonts::watermark_font_small, c_visuals::get().m_full_bright, 1);
	buttonComponent edit_smoke(&visual_box, "edit smoke", render::fonts::watermark_font_small, c_visuals::get().m_edit_smoke, 2);
	buttonComponent smoke_wire(&visual_box, "smoke wireframe", render::fonts::watermark_font_small, c_visuals::get().smoke_wireframe, 3);


	c_menu::get_ptr()->combat_btn.m_boxes.insert(&box);
	c_menu::get_ptr()->combat_btn.m_boxes.insert(&fakelagbox);
	c_menu::get_ptr()->combat_btn.m_boxes.insert(&ragebot_box);

	c_menu::get_ptr()->visuals_btn.m_boxes.insert(&Chamsbox);
	c_menu::get_ptr()->visuals_btn.m_boxes.insert(&esp_box);
	c_menu::get_ptr()->visuals_btn.m_boxes.insert(&visual_box);

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
