#include "menu.hpp"
#include "../features/features.hpp"
#include "./components/button_component.h"
#include "./components/group_box.h"
#include "../features/hacks/ragebot/ragebot.h"
#include "../features/hacks/render/player_esp.h"
#include "../features/hacks/render/visuals.h"
//todo auto elements positioning



auto do_frame = [&](std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, color bg, color header_text, color header_line, const std::string& name) {
	render::draw_filled_rect(x - 1, y - 1, w + 2, h + 2, color(variables::menu::r, variables::menu::g, variables::menu::b, 255));
	render::draw_filled_rect(x, y, w, h, bg);
	render::draw_filled_rect(x, y, w, 70, header_text);
	render::draw_filled_rect(x - 1, y + 69, w + 2, 2, color::white());
	render::draw_filled_rect(x, y + 70, w, 2, header_line);
	render::text(x + 10, y + 8, render::fonts::watermark_font, name, false, color::white());
};

void draw_box(groupBox& box) {
	box.draw();
	int i = 10;
	for (comp* m_c : box.get_comps()) {
		m_c->draw();
		i += m_c->get_standard_height();
	}

	i += 10;
	box.set_height(i);
}


void menu::render() {
	if (!variables::menu::opened)
		return;


	do_frame(variables::menu::x, variables::menu::y, variables::menu::w, variables::menu::h, color(36, 36, 36, 255), color(45, 45, 45, 235), color(36, 36, 36, 255), "BeakonCS - B1.1");
	const std::int32_t offset = 20;
	//menu_framework::group_box(variables::menu::x + 5, variables::menu::y + 35, 100, 250, render::fonts::watermark_font, "tabs", false); {
	menu_framework::tab((variables::menu::x + 100) + offset, variables::menu::y + 15, render::get_text_size(render::fonts::font_bigboi, "Combat").x + 50, 45, render::fonts::font_bigboi, "Combat", menu::current_tab, 0);
	menu_framework::tab((variables::menu::x + 220) + offset, variables::menu::y + 15, render::get_text_size(render::fonts::font_bigboi, "Visuals").x + 50, 45, render::fonts::font_bigboi, "Visuals", menu::current_tab, 1);
	menu_framework::tab((variables::menu::x + 330) + offset, variables::menu::y + 15, render::get_text_size(render::fonts::font_bigboi, "Movement").x + 50, 45, render::fonts::font_bigboi, "Movement", menu::current_tab, 2);
	menu_framework::tab((variables::menu::x + 460) + offset, variables::menu::y + 15, render::get_text_size(render::fonts::font_bigboi, "Misc").x + 50, 45, render::fonts::font_bigboi, "Misc", menu::current_tab, 3);
	
	if (current_tab == 0) {
		{
			static groupBox box(20, 80, variables::menu::w / 2 - 20, 220, render::fonts::watermark_font_other, "Combat");
			//box.draw();
			//menu_framework::group_box(variables::menu::x + 5, variables::menu::y + 35, variables::menu::w - 10, variables::menu::h - 40, render::fonts::watermark_font, "Combat", false); {
			static buttonComponent Aimbot_checkBoxComp(15, 10, &box, "Aimbot", render::fonts::watermark_font_small, combat::aimbot::isEnabled);

			static buttonComponent Trigger_comp(15, 22, &box, "Triggerbot", render::fonts::watermark_font_small, combat::triggerbot::enabled);
			static buttonComponent Resolver_comp(15, 34, &box, "Resolver", render::fonts::watermark_font_small, combat::resolver::enabled);
			static buttonComponent anti_aim_comp(15, 46, &box, "Anti aim", render::fonts::watermark_font_small, combat::antiaim::isEnabled);
			static buttonComponent desync_comp(15, 58, &box, "Desync", render::fonts::watermark_font_small, combat::antiaim::desync);
			static buttonComponent backwards_comp(15, 70, &box, "Backwards", render::fonts::watermark_font_small, combat::antiaim::backwards);
			static buttonComponent jitter_comp(15, 82, &box, "Jitter", render::fonts::watermark_font_small, combat::antiaim::jitter);

			static c_slider_component jitter_amount_comp(15, 94, &box, "Jitter Amount", render::fonts::watermark_font_small, combat::antiaim::jitter_amount, 1, 20);
			static buttonComponent spin_comp(15, 122, &box, "Spinbot", render::fonts::watermark_font_small, combat::antiaim::spinbot);
			static c_slider_component spinspeed_comp(15, 132, &box, "Spinspeed", render::fonts::watermark_font_small, combat::antiaim::spinspeed, 1, 20);
			static c_slider_component yaw_comp(15, 156, &box, "yaw", render::fonts::watermark_font_small, combat::antiaim::yaw, -180, 180);
			static c_slider_component pitch_comp(15, 180, &box, "pitch", render::fonts::watermark_font_small, combat::antiaim::pitch, -90, 90);

			draw_box(box);
		}

		{
			static groupBox fakelagBox(20, 310, variables::menu::w / 2 - 20, 80, render::fonts::watermark_font_other, "fakelag");
			static buttonComponent fakelag_enabled(15, 10, &fakelagBox, "enabled", render::fonts::watermark_font_small, combat::fakelag::isEnabled);
			static c_slider_component fakelag_amount(15, 22, &fakelagBox, "Lag amount", render::fonts::watermark_font_small, combat::fakelag::amount, 2, 10);

			draw_box(fakelagBox);
		}

		{
			static groupBox ragebot_box(variables::menu::w / 2 + 10, 80, variables::menu::w / 2 - 30, 190, render::fonts::watermark_font_other, "ragebot");
		
			//ragebot_box.draw();
			static buttonComponent rb_enabled(15, 10, &ragebot_box, "Enabled", render::fonts::watermark_font_small, combat::ragebot::isEnabled);
			static buttonComponent rb_auto_fire(15, 22, &ragebot_box, "Auto fire", render::fonts::watermark_font_small, combat::ragebot::auto_fire);
			static buttonComponent rb_silent(15, 34, &ragebot_box, "Silent", render::fonts::watermark_font_small, combat::ragebot::silent);
			static buttonComponent rb_quickstop(15, 46, &ragebot_box, "Quickstop", render::fonts::watermark_font_small, combat::ragebot::quickstop);
			static buttonComponent rb_teammates(15, 58, &ragebot_box, "Teammates", render::fonts::watermark_font_small, combat::ragebot::teammates);
			static c_slider_component rb_hitchance(15, 70, &ragebot_box, "Hitchance", render::fonts::watermark_font_small, combat::ragebot::hit_chance, 1, 100);
			static c_slider_component rb_min_dmg(15, 94, &ragebot_box, "Minimum damage", render::fonts::watermark_font_small, combat::ragebot::min_dmg, 1, 100);
			static buttonComponent rb_dynamic_hitbox(15, 125, &ragebot_box, "Dynamic hitbox targeting", render::fonts::watermark_font_small, combat::ragebot::dynamic_hitbox);
			static buttonComponent rb_head(15, 137, &ragebot_box, "Head hitbox targeting", render::fonts::watermark_font_small, combat::ragebot::head);
			static buttonComponent rb_pelvis(15, 149, &ragebot_box, "Pelvis hitbox targeting", render::fonts::watermark_font_small, combat::ragebot::pelvis);

			static buttonComponent rb_arms(15, 161, &ragebot_box, "Arms hitbox targeting", render::fonts::watermark_font_small, combat::ragebot::arms);
			static buttonComponent rb_legs(15, 173, &ragebot_box, "Legs hitbox targeting", render::fonts::watermark_font_small, combat::ragebot::legs);
			
			draw_box(ragebot_box);
		}
		
	}
	else if (current_tab == 1) {
		//menu_framework::group_box(variables::menu::x + 110, variables::menu::y + 35, 305, 260, render::fonts::watermark_font, "visuals", false); {
			//menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 45, variables::menu::x + 275, render::fonts::watermark_font, "SnapLines", variables::visuals::snapLines);
			//menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 60, variables::menu::x + 275, render::fonts::watermark_font, "2D Box", variables::visuals::twoD_box);
			//menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 75, variables::menu::x + 275, render::fonts::watermark_font, "2D Corner", variables::visuals::twoD_Corner);
			//menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 105, variables::menu::x + 275, render::fonts::watermark_font, "Glow", variables::visuals::glow);
			//menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 120, variables::menu::x + 275, render::fonts::watermark_font, "ColorWorld", variables::visuals::colorWorld);
		//}
			static groupBox ChamsBox(20, 80, variables::menu::w / 2 - 20, 220, render::fonts::watermark_font_other, "Chams");
			//ChamsBox.draw();
			static buttonComponent chams_enabled(15, 10, &ChamsBox, "Enabled", render::fonts::watermark_font_small, Render::Chams::isEnabled);
			static c_slider_component Material(15, 22, &ChamsBox, "Material", render::fonts::watermark_font_small, Render::Chams::material, 1, 7);
			static buttonComponent Enemies(15, 58, &ChamsBox, "Enemies", render::fonts::watermark_font_small, Render::Chams::enemies);
			static buttonComponent Teamates(15, 70, &ChamsBox, "Teammates", render::fonts::watermark_font_small, Render::Chams::teammates);
			static buttonComponent Hands(15, 82, &ChamsBox, "Hands", render::fonts::watermark_font_small, Render::Chams::hands);
			static c_color_picker team_color_invisible(15, 94, &ChamsBox, "Team Color Invisible", render::fonts::watermark_font_small, Render::Chams::team_color_invisible);
			static c_color_picker team_color_visible(15, 112, &ChamsBox, "Team Color Visible", render::fonts::watermark_font_small, Render::Chams::team_color_visible);

			draw_box(ChamsBox);

		{
			static groupBox esp_box(variables::menu::w / 2 + 10, 80, variables::menu::w / 2 - 30, 190, render::fonts::watermark_font_other, "player esp");
			static buttonComponent skeleton_esp(15, 10, &esp_box, "Skeleton Esp", render::fonts::watermark_font_small, g_player_esp.is_enabled);

			draw_box(esp_box);
		}

		{
			static groupBox visual_box(20, 80 + ChamsBox.getHeight() + 10, variables::menu::w / 2 - 20, 20, render::fonts::watermark_font_other, "visuals");

			static buttonComponent remove_visual_recoil(15, 10, &visual_box, "remove visual recoil", render::fonts::watermark_font_small, c_visuals::get().visual_recoil);
			static buttonComponent full_bright(15, 22, &visual_box, "fullbright", render::fonts::watermark_font_small, c_visuals::get().m_full_bright);

			static buttonComponent edit_smoke(15, 34, &visual_box, "edit smoke", render::fonts::watermark_font_small, c_visuals::get().m_edit_smoke);
			static buttonComponent smoke_wire(15, 46, &visual_box, "smoke wireframe", render::fonts::watermark_font_small, c_visuals::get().smoke_wireframe);

			draw_box(visual_box);
		}
		/*
		menu_framework::group_box(variables::menu::x + 5, variables::menu::y + 35, variables::menu::w - 10, variables::menu::h - 40, render::fonts::watermark_font, "Visuals", false); {
			//menu_framework::check_box(variables::menu::x + 7, variables::menu::y + 35, variables::menu::x + 275, render::fonts::watermark_font_other, "Chams", Render::Chams::isEnabled);
			//menu_framework::sliderInt(variables::menu::x + 7, variables::menu::y + 50, render::fonts::watermark_font_small, "Material", Render::Chams::material, 1, 7);
			//menu_framework::check_box(variables::menu::x + 7, variables::menu::y + 65, variables::menu::x + 275, render::fonts::watermark_font_small, "Enemies", Render::Chams::enemies);
			//menu_framework::check_box(variables::menu::x + 7, variables::menu::y + 80, variables::menu::x + 275, render::fonts::watermark_font_small, "Teammates", Render::Chams::teammates);
			//menu_framework::check_box(variables::menu::x + 7, variables::menu::y + 95, variables::menu::x + 275, render::fonts::watermark_font_small, "Hands", Render::Chams::hands);
			
			menu_framework::sliderInt(variables::menu::x + 7, variables::menu::y + 115, render::fonts::watermark_font_small, "TeamRedVisible", Render::Chams::TeamRedVisible, 1, 255);
			menu_framework::sliderInt(variables::menu::x + 7, variables::menu::y + 130, render::fonts::watermark_font_small, "TeamGreenVisible", Render::Chams::TeamGreenVisible, 1, 255);
			menu_framework::sliderInt(variables::menu::x + 7, variables::menu::y + 145, render::fonts::watermark_font_small, "TeamBlueVisible", Render::Chams::TeamBlueVisible, 1, 255);
			menu_framework::sliderInt(variables::menu::x + 7, variables::menu::y + 160, render::fonts::watermark_font_small, "TeamRedInVisible", Render::Chams::TeamRedInVisible, 1, 255);
			menu_framework::sliderInt(variables::menu::x + 7, variables::menu::y + 175, render::fonts::watermark_font_small, "TeamGreenInVisible", Render::Chams::TeamGreenInVisible, 1, 255);
			menu_framework::sliderInt(variables::menu::x + 7, variables::menu::y + 190, render::fonts::watermark_font_small, "TeamBlueInVisible", Render::Chams::TeamBlueInVisible, 1, 255);
			menu_framework::sliderInt(variables::menu::x + 7, variables::menu::y + 210, render::fonts::watermark_font_small, "EnemyRedVisible", Render::Chams::EnemyRedVisible, 1, 255);
			menu_framework::sliderInt(variables::menu::x + 7, variables::menu::y + 225, render::fonts::watermark_font_small, "EnemyGreenVisible", Render::Chams::EnemyGreenVisible, 1, 255);
			menu_framework::sliderInt(variables::menu::x + 7, variables::menu::y + 240, render::fonts::watermark_font_small, "EnemyBlueVisible", Render::Chams::EnemyBlueVisible, 1, 255);
			menu_framework::sliderInt(variables::menu::x + 7, variables::menu::y + 255, render::fonts::watermark_font_small, "EnemyRedInVisible", Render::Chams::EnemyRedInVisible, 1, 255);
			menu_framework::sliderInt(variables::menu::x + 7, variables::menu::y + 270, render::fonts::watermark_font_small, "EnemyGreenInVisible", Render::Chams::EnemyGreenInVisible, 1, 255);
			menu_framework::sliderInt(variables::menu::x + 7, variables::menu::y + 285, render::fonts::watermark_font_small, "EnemyBlueInVisible", Render::Chams::EnemyBlueInVisible, 1, 255);

			int GuiOffset = 300;
			menu_framework::sliderInt(variables::menu::x + 7, (variables::menu::y + 110) + GuiOffset, render::fonts::watermark_font_small, "Red: Gui", variables::menu::r, 0, 255);
			menu_framework::sliderInt(variables::menu::x + 7, (variables::menu::y + 125) + GuiOffset, render::fonts::watermark_font_small, "Green: Gui", variables::menu::g, 0, 255);
			menu_framework::sliderInt(variables::menu::x + 7, (variables::menu::y + 140) + GuiOffset, render::fonts::watermark_font_small, "Blue: Gui", variables::menu::b, 0, 255);
			menu_framework::check_box(variables::menu::x + 7, (variables::menu::y + 155) + GuiOffset, variables::menu::x + 275, render::fonts::watermark_font_small, "Rainbow", variables::menu::rainbow);
			menu_framework::slider(variables::menu::x + 7, (variables::menu::y + 170) + GuiOffset, render::fonts::watermark_font_small, "Speed", variables::menu::speed, 1, 20);

			menu_framework::check_box(variables::menu::x + 300, variables::menu::y + 35, variables::menu::x + 275, render::fonts::watermark_font_other, "Esp", Render::Esp::isEnabled);
			menu_framework::sliderInt(variables::menu::x + 300, variables::menu::y + 50, render::fonts::watermark_font_small, "Red", Render::Esp::R, 0, 255);
			menu_framework::sliderInt(variables::menu::x + 300, variables::menu::y + 65, render::fonts::watermark_font_small, "Green", Render::Esp::G, 0, 255);
			menu_framework::sliderInt(variables::menu::x + 300, variables::menu::y + 80, render::fonts::watermark_font_small, "Blue", Render::Esp::B, 0, 255);


			menu_framework::check_box(variables::menu::x + 300, variables::menu::y + 95, variables::menu::x + 275, render::fonts::watermark_font_other, "Glow", Render::Glow::isEnabled);
		}
		*/
	}
	else if (current_tab == 2) {
		menu_framework::group_box(variables::menu::x + 5, variables::menu::y + 35, variables::menu::w - 10, variables::menu::h - 40, render::fonts::watermark_font, "Movement", false); {
			menu_framework::check_box(variables::menu::x + 7, variables::menu::y + 45, variables::menu::x + 375, render::fonts::watermark_font_small, "Bhop", movement::bunny_hop::isEnabled);
			menu_framework::check_box(variables::menu::x + 7, variables::menu::y + 60, variables::menu::x + 375, render::fonts::watermark_font_small, "Auto Strafe", movement::auto_strafe::isEnabled);
			//menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 60, variables::menu::x + 375, render::fonts::watermark_font, "noFlash", variables::misc::noFlash);
			//menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 75, variables::menu::x + 375, render::fonts::watermark_font, "Fast Stop", variables::misc::fast_stop);
			//menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 90, variables::menu::x + 375, render::fonts::watermark_font, "Air Strafe", variables::misc::air_strafe);
		}
	}else if (current_tab == 3){
		menu_framework::group_box(variables::menu::x + 5, variables::menu::y + 35, variables::menu::w - 10, variables::menu::h - 40, render::fonts::watermark_font, "Misc", false); {
			
		}
	}

	menu_framework::menu_movement(variables::menu::x, variables::menu::y, variables::menu::w, 40);
}

void menu::toggle() {

	if (GetAsyncKeyState(VK_INSERT) & 1 || ((GetAsyncKeyState(VK_ESCAPE) && variables::menu::opened)))
	{
		variables::menu::opened = !variables::menu::opened;
	}
}
