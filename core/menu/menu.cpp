#include "menu.hpp"
#include "../features/features.hpp"
#include "./components/button_component.h"
#include "./components/group_box.h"
#include "../features/hacks/ragebot/ragebot.h"
#include "../features/hacks/render/player_esp.h"
#include "../features/hacks/render/visuals.h"
#include "./utils/gui_utils.h"
//todo auto elements positioning

/*
auto do_frame = [&](std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, color bg, color header_text, color header_line, const std::string& name) {
	render::draw_filled_rect(x - 1, y - 1, w + 2, h + 2, color(variables::menu::r, variables::menu::g, variables::menu::b, 255));
	render::draw_filled_rect(x, y, w, h, bg);
	render::draw_filled_rect(x, y, w, 70, header_text);
	render::draw_filled_rect(x - 1, y + 69, w + 2, 2, color::white());
	render::draw_filled_rect(x, y + 70, w, 2, header_line);
	render::text(x + 10, y + 8, render::fonts::watermark_font, name, false, color::white());
};



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
		static groupBox box(20, 80, variables::menu::w / 2 - 20, 220, render::fonts::watermark_font_other, "Combat");
		//box.draw();
		//menu_framework::group_box(variables::menu::x + 5, variables::menu::y + 35, variables::menu::w - 10, variables::menu::h - 40, render::fonts::watermark_font, "Combat", false); {
		static buttonComponent Aimbot_checkBoxComp(&box, "Aimbot", render::fonts::watermark_font_small, combat::aimbot::isEnabled);

		static buttonComponent Trigger_comp(&box, "Triggerbot", render::fonts::watermark_font_small, combat::triggerbot::enabled);
		static buttonComponent Resolver_comp(&box, "Resolver", render::fonts::watermark_font_small, combat::resolver::enabled);
		static buttonComponent anti_aim_comp(&box, "Anti aim", render::fonts::watermark_font_small, combat::antiaim::isEnabled);
		static buttonComponent desync_comp(&box, "Desync", render::fonts::watermark_font_small, combat::antiaim::desync);
		static buttonComponent backwards_comp(&box, "Backwards", render::fonts::watermark_font_small, combat::antiaim::backwards);
		static buttonComponent jitter_comp(&box, "Jitter", render::fonts::watermark_font_small, combat::antiaim::jitter);

		static c_slider_component jitter_amount_comp(&box, "Jitter Amount", render::fonts::watermark_font_small, combat::antiaim::jitter_amount, 1, 20);
		static buttonComponent spin_comp(&box, "Spinbot", render::fonts::watermark_font_small, combat::antiaim::spinbot);
		static c_slider_component spinspeed_comp(&box, "Spinspeed", render::fonts::watermark_font_small, combat::antiaim::spinspeed, 1, 20);
		static c_slider_component yaw_comp(&box, "yaw", render::fonts::watermark_font_small, combat::antiaim::yaw, -180, 180);
		static c_slider_component pitch_comp(&box, "pitch", render::fonts::watermark_font_small, combat::antiaim::pitch, -90, 90);

		//c_gui_manager::get_ptr()->draw_box(box);
		

		
		static groupBox fakelagBox(20, 310, variables::menu::w / 2 - 20, 80, render::fonts::watermark_font_other, "fakelag");
		static buttonComponent fakelag_enabled(&fakelagBox, "enabled", render::fonts::watermark_font_small, combat::fakelag::isEnabled);
		static c_slider_component fakelag_amount(&fakelagBox, "Lag amount", render::fonts::watermark_font_small, combat::fakelag::amount, 2, 10);

		//c_gui_manager::get_ptr()->draw_box(fakelagBox);
		

		
		static groupBox ragebot_box(variables::menu::w / 2 + 10, 80, variables::menu::w / 2 - 30, 190, render::fonts::watermark_font_other, "ragebot");
		
		static buttonComponent rb_enabled(&ragebot_box, "Enabled", render::fonts::watermark_font_small, combat::ragebot::isEnabled);
		static buttonComponent rb_auto_fire(&ragebot_box, "Auto fire", render::fonts::watermark_font_small, combat::ragebot::auto_fire);
		static buttonComponent rb_silent(&ragebot_box, "Silent", render::fonts::watermark_font_small, combat::ragebot::silent);
		static buttonComponent rb_quickstop(&ragebot_box, "Quickstop", render::fonts::watermark_font_small, combat::ragebot::quickstop);
		static buttonComponent rb_teammates(&ragebot_box, "Teammates", render::fonts::watermark_font_small, combat::ragebot::teammates);
		static c_slider_component rb_hitchance(&ragebot_box, "Hitchance", render::fonts::watermark_font_small, combat::ragebot::hit_chance, 1, 100);
		static c_slider_component rb_min_dmg(&ragebot_box, "Minimum damage", render::fonts::watermark_font_small, combat::ragebot::min_dmg, 1, 100);
		static buttonComponent rb_dynamic_hitbox(&ragebot_box, "Dynamic hitbox targeting", render::fonts::watermark_font_small, combat::ragebot::dynamic_hitbox);
		static buttonComponent rb_head(&ragebot_box, "Head hitbox targeting", render::fonts::watermark_font_small, combat::ragebot::head);
		static buttonComponent rb_pelvis(&ragebot_box, "Pelvis hitbox targeting", render::fonts::watermark_font_small, combat::ragebot::pelvis);

		static buttonComponent rb_arms(&ragebot_box, "Arms hitbox targeting", render::fonts::watermark_font_small, combat::ragebot::arms);
		static buttonComponent rb_legs(&ragebot_box, "Legs hitbox targeting", render::fonts::watermark_font_small, combat::ragebot::legs);
			
		//c_gui_manager::get_ptr()->draw_box(ragebot_box);
		
		std::vector<groupBox*> boxes = { &box, &fakelagBox, &ragebot_box };

		for (groupBox* box : boxes) {
			c_gui_manager::get_ptr()->draw_box(*box);
		}

	}
	else if (current_tab == 1) {
		static groupBox ChamsBox(20, 80, variables::menu::w / 2 - 20, 220, render::fonts::watermark_font_other, "Chams");
			
		static buttonComponent chams_enabled(&ChamsBox, "Enabled", render::fonts::watermark_font_small, Render::Chams::isEnabled);
		static c_slider_component Material(&ChamsBox, "Material", render::fonts::watermark_font_small, Render::Chams::material, 1, 7);
		static buttonComponent Enemies(&ChamsBox, "Enemies", render::fonts::watermark_font_small, Render::Chams::enemies);
		static buttonComponent Teamates(&ChamsBox, "Teammates", render::fonts::watermark_font_small, Render::Chams::teammates);
		static buttonComponent Hands(&ChamsBox, "Hands", render::fonts::watermark_font_small, Render::Chams::hands);
		static c_color_picker team_color_invisible(&ChamsBox, "Team Color Invisible", render::fonts::watermark_font_small, Render::Chams::team_color_invisible);
		static c_color_picker team_color_visible(&ChamsBox, "Team Color Visible", render::fonts::watermark_font_small, Render::Chams::team_color_visible);

		c_gui_manager::get_ptr()->draw_box(ChamsBox);

		
		static groupBox esp_box(variables::menu::w / 2 + 10, 80, variables::menu::w / 2 - 30, 190, render::fonts::watermark_font_other, "player esp");
		static buttonComponent skeleton_esp(&esp_box, "Skeleton Esp", render::fonts::watermark_font_small, g_player_esp.is_enabled);

		c_gui_manager::get_ptr()->draw_box(esp_box);


		static groupBox visual_box(20, 80 + ChamsBox.getHeight() + 10, variables::menu::w / 2 - 20, 20, render::fonts::watermark_font_other, "visuals");

		static buttonComponent remove_visual_recoil(&visual_box, "remove visual recoil", render::fonts::watermark_font_small, c_visuals::get().visual_recoil);
		static buttonComponent full_bright(&visual_box, "fullbright", render::fonts::watermark_font_small, c_visuals::get().m_full_bright);

		static buttonComponent edit_smoke(&visual_box, "edit smoke", render::fonts::watermark_font_small, c_visuals::get().m_edit_smoke);
		static buttonComponent smoke_wire(&visual_box, "smoke wireframe", render::fonts::watermark_font_small, c_visuals::get().smoke_wireframe);

		c_gui_manager::get_ptr()->draw_box(visual_box);
		

		//std::vector<groupBox*> vis_boxes = { &ChamsBox, &visual_box, &esp_box };

		//for (groupBox* box : vis_boxes) {
		//	c_gui_manager::get_ptr()->draw_box(*box);
		//}
	}
	else if (current_tab == 2) {
		//menu_framework::group_box(variables::menu::x + 5, variables::menu::y + 35, variables::menu::w - 10, variables::menu::h - 40, render::fonts::watermark_font, "Movement", false); {
		//	menu_framework::check_box(variables::menu::x + 7, variables::menu::y + 45, variables::menu::x + 375, render::fonts::watermark_font_small, "Bhop", movement::bunny_hop::isEnabled);
		//	menu_framework::check_box(variables::menu::x + 7, variables::menu::y + 60, variables::menu::x + 375, render::fonts::watermark_font_small, "Auto Strafe", movement::auto_strafe::isEnabled);
			//menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 60, variables::menu::x + 375, render::fonts::watermark_font, "noFlash", variables::misc::noFlash);
			//menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 75, variables::menu::x + 375, render::fonts::watermark_font, "Fast Stop", variables::misc::fast_stop);
			//menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 90, variables::menu::x + 375, render::fonts::watermark_font, "Air Strafe", variables::misc::air_strafe);
		//}
	}else if (current_tab == 3){
		//menu_framework::group_box(variables::menu::x + 5, variables::menu::y + 35, variables::menu::w - 10, variables::menu::h - 40, render::fonts::watermark_font, "Misc", false); {
			
		//}
	}

	menu_framework::menu_movement(variables::menu::x, variables::menu::y, variables::menu::w, 40);
}

void menu::toggle() {

	if (GetAsyncKeyState(VK_INSERT) & 1 || ((GetAsyncKeyState(VK_ESCAPE) && variables::menu::opened)))
	{
		variables::menu::opened = !variables::menu::opened;
	}
}

*/

void c_menu::render() {
	if (!this->is_open) {
		return;
	}

	GetCursorPos(&cursor);

	
	if (GetAsyncKeyState(VK_LBUTTON) && !is_mouse) {
		if ((cursor.x > x) && (cursor.x < x + w) && (cursor.y > y) && (cursor.y < y + h)) {
			this->mouseClicked(cursor.x, cursor.y, VK_LBUTTON);
		}
		is_mouse = true;
	}
	else if (!GetAsyncKeyState(VK_LBUTTON) && is_mouse) {
		this->mouseReleased();
		is_mouse = false;

		//DEBUG
		//render::text(x + 10, y + 16, render::fonts::watermark_font, "dfadfa", false, color::white());
	}

	if (dragging) {
		x = cursor.x - x2;
		y = cursor.y - y2;
	}

	//std::string prev_name = "";

	//render::draw_filled_rect(x - 1, y - 1, w + 2, h + 2, color(variables::menu::r, variables::menu::g, variables::menu::b, 255));
	render::draw_rounded_rect(x, y, w, h, 10, color(36, 36, 36, 255), true, false, true, false);
	render::draw_filled_rect(x - 1, y - 1, w + 2, 72, color::white());
	render::draw_filled_rect(x, y, w, 70, color(31, 31, 31, 255));
	
	//render::draw_filled_rect(x, y + 70, w, 2, color(36, 36, 36, 255));

	render::text(x + 10, y + 8, render::fonts::watermark_font, "BeakonCS - B1.1", false, color::white());

	for (c_category_btn* btn : this->categories) {
		
		btn->draw("");

		//prev_name.append(btn->m_name + " ");
	}
}

void c_menu::toggle() {

	if (GetAsyncKeyState(VK_INSERT) & 1 || ((GetAsyncKeyState(VK_ESCAPE) & 1) && is_open))
	{
		this->is_open = !this->is_open;
	}
}

void c_menu::mouseClicked(int mouseX, int mouseY, int btn) {

	if (btn == VK_LBUTTON) {

		clicking = true;

		if (mouseX > x && mouseX < x + w && mouseY > y && mouseY < y + 70) {
			dragging = true;
		}

		for (c_category_btn* c_btn : this->categories) {

			if (mouseX > this->x + c_btn->x && mouseX < this->x + c_btn->x + c_btn->w && mouseY > this->y + c_btn->y && mouseY < this->y + c_btn->y + c_btn->h) {
				c_btn->mouseClicked(mouseX, mouseY, btn);
				dragging = false;
				break;
			}
		}

		if (dragging) {
			x2 = mouseX - x;
			y2 = mouseY - y;
		}
	}
}

void c_menu::renderTextInMenu(std::int32_t x, std::int32_t y, unsigned long font, std::string text, bool centered, color color)
{
	render::text(this->x + x, this->y + y, font, text, centered, color);
}
void c_menu::renderRectInMenu(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color)
{
	render::draw_rect(this->x + x, this->y + y, width, height, color);
}
void c_menu::renderFilledRectInMenu(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color)
{
	render::draw_filled_rect(this->x + x, this->y + y, width, height, color);
}
void c_menu::renderCircleInMenu(std::int32_t x, std::int32_t y, std::int32_t radius, std::int32_t segments, color color)
{
	render::draw_circle(this->x + x, this->y + y, radius, segments, color);
}
void c_menu::renderRoundedRectInMenu(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, std::int32_t r, color color, bool left_under, bool left_up, bool right_under, bool right_up)
{
	render::draw_rounded_rect(this->x + x, this->y + y, width, height, r, color, left_under, left_up, right_under, right_up);
}


void c_menu::mouseReleased() {
	clicking = false;
	dragging = false;
}
