#include "../../features.hpp"
#include "../../../menu/variables.hpp"
#include "player_esp.h"
#include "./../features.h"
#include <string>

color c_skeleton_esp::get_color(std::unordered_map<int, std::string> p_color_map, std::string c) {

	int index = 0;
	for (auto& elm : p_color_map) {
		if (elm.second == c) {
			index = elm.first;
		}
	}

	if (index == 0) {
		return color(255, 0, 0, 255); //red
	}
	else if (index == 1) {
		return color(125, 125, 0, 255); //orange
	}
	else if (index == 2) {
		return color(255, 255, 0, 255); //yellow
	}
	else if (index == 3) {
		return color(0, 255, 0, 255); //green
	}
	else if (index == 4) {
		return color(0, 100, 155, 255); //cyan
	}
	else if (index == 5) {
		return color(0, 0, 255, 255); //blue
	}
	else if (index == 6) {
		return color(255, 0, 255, 255); //purple
	}
	else if (index == 7) {
		return color(255, 0, 100, 255); //pink
	}
	else if (index == 8) {
		return color(255, 255, 255, 255); //white
	}
	else if (index == 9) {
		return color(1, 1, 1, 255); //black
	}
	return color(1, 1, 1, 255);
}

void c_skeleton_esp::on_draw()
{
	if (!this->teammates && !this->enemies)
		return;

	if (interfaces::engine->is_in_game())
	{
		if (!csgo::local_player)
			return;

		for (int i = 1; i <= interfaces::globals->max_clients; ++i) {
			auto entity = static_cast<entity_t*>(interfaces::entity_list->get_client_entity(i));

			if (!entity || entity->dormant() || entity == csgo::local_player || entity->health() == 0)
				continue;

			if (entity != nullptr) {

				matrix_t bone_matrices[128];
				if (entity->setup_bones(bone_matrices, 128, 256, 0.0f)) {
					if (entity->team() == csgo::local_player->team() && this->teammates) {
						//c_player_esp::get_ptr()->player((player_t*)entity);
						this->draw_skeleton((player_t*)entity, this->get_color(this->color_map, this->team_color), bone_matrices);
					}
					else if(entity->team() != csgo::local_player->team() && this->enemies){
						//c_player_esp::get_ptr()->player((player_t*)entity);
						this->draw_skeleton((player_t*)entity, this->get_color(this->color_map, this->enemy_color), bone_matrices);
					}
				}

				//if (utilities::get_bbox(entity, b, true)) {

					//render::draw_rect(b.x, b.y, 10, 10, inside_color);
					//draw_2d_box(entity, b);
					//draw_name(entity, b);
				//}
			}
		}
	}
}


void c_skeleton_esp::draw_skeleton(player_t* e, color color, matrix_t matrix[MAXSTUDIOBONES])
{
	auto model = e->model();

	if (!model)
		return;

	auto studio_model = interfaces::model_info->get_studio_model(model);

	if (!studio_model)
		return;

	auto get_bone_position = [&](int bone) -> vec3_t
	{
		vec3_t tmp = { matrix[bone][0][3], matrix[bone][1][3], matrix[bone][2][3] };
		return tmp;
	};

	auto upper_direction = get_bone_position(7) - get_bone_position(6);
	auto breast_bone = get_bone_position(6) + upper_direction * 0.5f;

	for (auto i = 0; i < studio_model->bones_count; i++)
	{
		auto bone = studio_model->bone(i);

		if (!bone)
			continue;

		if (bone->parent == -1)
			continue;

		if (!(bone->flags & BONE_USED_BY_HITBOX))
			continue;

		auto child = get_bone_position(i);
		auto parent = get_bone_position(bone->parent);

		auto delta_child = child - breast_bone;
		auto delta_parent = parent - breast_bone;

		if (delta_parent.length() < 9.0f && delta_child.length() < 9.0f)
			parent = breast_bone;

		if (i == 5)
			child = breast_bone;

		if (fabs(delta_child.z) < 5.0f && delta_parent.length() < 5.0f && delta_child.length() < 5.0f || i == 6)
			continue;

		vec3_t schild = {0, 0, 0};
		vec3_t sparent = {0, 0, 0};

		if (math::world_to_screen(child, schild) && math::world_to_screen(parent, sparent)) {
			render::draw_line(schild.x, schild.y, sparent.x, sparent.y, color);
		}
	}
}

//playeresp

void c_player_esp::run() {

	if (interfaces::engine->is_in_game())
	{
		if (!csgo::local_player)
			return;

		for (int i = 1; i <= interfaces::globals->max_clients; ++i) {
			auto entity = static_cast<entity_t*>(interfaces::entity_list->get_client_entity(i));

			if (!entity || entity->dormant() || entity == csgo::local_player || entity->health() == 0)
				continue;

			if (entity != nullptr) {

				c_player_esp::get_ptr()->player((player_t*)entity);
			}
		}
	}

}

void c_player_esp::player(player_t* entity) {
	const auto index = entity->index();

	auto local = csgo::local_player;
	if (!local || entity == local)
		return;

	if (!local->is_alive()) {
		const auto spec_handle = local->observer_target();
		const auto spec = static_cast<entity_t*>(interfaces::entity_list->get_client_entity(spec_handle));
		if (spec == entity)
			return;
	}

	Box box{};
	if (!utilities::get_bbox(entity, box, false))
		return;

	weapon_t* weapon = entity->active_weapon();
	if (!weapon)
		return;

	weapon_info_t* info = interfaces::weapon_system->get_weapon_data(weapon->item_definition_index());
	if (!info)
		return;

	const auto name_color = c_skeleton_esp::get_ptr()->get_color(this->color_map, this->name_color);
	const auto box_color = c_skeleton_esp::get_ptr()->get_color(this->color_map, this->box_color);
	const auto ammo_bar_color = c_skeleton_esp::get_ptr()->get_color(this->color_map, this->ammobar_color);
	const auto weapon_color = c_skeleton_esp::get_ptr()->get_color(this->color_map, this->weapon_color);
	const auto flag_color = c_skeleton_esp::get_ptr()->get_color(this->color_map, this->flag_color);
	const auto money_color = c_skeleton_esp::get_ptr()->get_color(this->color_map, this->money_color);
	const auto flashed_color = c_skeleton_esp::get_ptr()->get_color(this->color_map, this->flashed_color);
	
	g_ctx.globals.flag_count = 0;
	g_ctx.globals.offset = 0;

	bool is_team = (entity->team() == csgo::local_player->team());

	if (boxes){
		
		draw_box(box_color, index, box.x, box.y, box.w, box.h);
	}
	
	if (names) {
		player_info_t info;
		interfaces::engine->get_player_info(index, &info);

		std::string name = info.name;

		if (name.length() > 17)
			name = name.substr(0, 17) + "...";

		//std::transform( name.begin( ), name.end( ), name.begin( ), tolower );
		render::text(box.x - render::get_text_size(render::fonts::watermark_font_other, name).x / 2 + box.w * 0.5, box.y - render::get_text_size(render::fonts::watermark_font_other, name).y, render::fonts::watermark_font_other, name, false, name_color);
	}
	
	if (this->healthbar)
		this->draw_healthbar(entity, box.x, box.y, box.w, box.h);

	if (this->flashed)
		this->flash_bar(entity, flashed_color, box.x, box.y, box.w, box.h);

	if (this->ammunition)
		this->ammo_bar(weapon, entity, ammo_bar_color, box.x, box.y, box.w, box.h, weapon->clip1_count());

	if (this->weapon)
		this->weapon_name(weapon, entity, weapon_color, box.x, box.y, box.w, box.h);

	if (this->money) {
		std::stringstream s;
		s << entity->money() << "$";
		render::text(box.x + box.w + 3, box.y + g_ctx.globals.flag_count++ * 9, render::fonts::watermark_font_other, s.str(), false, money_color);
	}
	if (this->flags) {
		this->draw_flags(entity, flag_color, box.x, box.y, box.w, box.h);
	}
}

void c_player_esp::draw_healthbar(player_t* entity, float x, float y, float w, float h) {
	int hp = std::clamp(entity->health(), 0, 100);
	const color health_color = color(255 - hp * 2.55, hp * 2.55, 0, 220);

	const int height = hp * static_cast<int>(h) / 100;

	render::draw_filled_rect(x - 6, y - 1, 4, h + 2, color(10, 10, 10, 220));

	color to_color = color(health_color.r - 40, health_color.g - 40, health_color.b - 40, health_color.a - 10);

	render::gradient(x - 5, y + h - height, 2, height, health_color, to_color, render::GradientType::GRADIENT_VERTICAL);

	//if (hp >= 90 || hp <= 10)
	//	return;

	std::stringstream s;
	s << hp << "%";
	render::text(x - 8, y + h - height - 6, render::fonts::watermark_font_other, s.str(), false, color::white(220));

}

void c_player_esp::draw_box(color c, int index, float x, float y, float w, float h) {
	render::draw_rect(x - 1, y - 1, w + 2, h + 2, color(10, 10, 10, 220));
	render::draw_rect(x + 1, y + 1, w - 2, h - 2, color(10, 10, 10, 220));
	render::draw_rect(x, y, w, h, c);
}

void c_player_esp::weapon_name(weapon_t* weapon, player_t* player, color c, float x, float y, float w, float h) {
	auto info = weapon->get_weapon_data();
	if (!info)
		return;

	const char* name = info->szHudName;
	std::wstring localised_name = interfaces::localize->find(name);

	std::string print(localised_name.begin(), localised_name.end());
	std::transform(print.begin(), print.end(), print.begin(), toupper);

	render::text(x + w * 0.5f, y + h + 2 + g_ctx.globals.offset, render::fonts::watermark_font_other, name, false, c);
}

void c_player_esp::ammo_bar(weapon_t* weapon, player_t* player, color c, float x, float y, float w, float h, int clip) {
	auto info = weapon->get_weapon_data();
	if (!info)
		return;

	//	setup for drawing
	const auto ammo = clip;
	const auto max_ammo = info->iMaxClip1;
	auto width = w;

	if (info->nWeaponType != WEAPONTYPE_KNIFE && info->nWeaponType != WEAPONTYPE_C4 && info->nWeaponType != WEAPONTYPE_GRENADE && info->nWeaponType != WEAPONTYPE_PLACEHOLDER) {
		width *= ammo;
		width /= max_ammo;
	}

	// background
	render::draw_filled_rect(x - 1, y + h + 2 + g_ctx.globals.offset, w + 2, 4, color(10, 10, 10, 220));
	
	// ammo bar
	color to_color = color(c.r - 50, c.g - 50, c.b - 50, c.a);
	render::gradient(x, y + h + 3 + g_ctx.globals.offset, width, 2, c, to_color, render::GradientType::GRADIENT_HORIZONTAL);

	if (ammo < 5 && ammo != 0) {
		std::stringstream s;
		s << ammo;
		render::text(x + width, y + h, render::fonts::watermark_font_other, s.str(), false, c);
	}

	g_ctx.globals.offset += 5;
}

void c_player_esp::flash_bar(player_t* player, color c, float x, float y, float w, float h) {
	const auto max_alpha = 255.f;

	float flash_alpha = player->flash_duration();
	if (flash_alpha < 1.f)
		return;

	float width = w;

	//	calculate width
	width *= (flash_alpha / 255.f);

	// background
	render::draw_filled_rect(x - 1, y + h + 2, w + 2, 4, color(10, 10, 10, 220));

	// flash bar
	render::draw_filled_rect(x, y + h + 3, width, 2, c);

	g_ctx.globals.offset += 5;
}

void c_player_esp::draw_flags(player_t* player, color c, float x, float y, float w, float h) {
	std::string armor_flag;
	if (player->has_helmet())
		armor_flag += "H";

	if (player->armor() > 0)
		armor_flag += "K";

	if (!armor_flag.empty()) {
		render::text(x + w + 3, y + g_ctx.globals.flag_count++ * 9, render::fonts::watermark_font_other, armor_flag, false, c);
	}

	if (player->flash_duration() > 140.f) {
		render::text(x + w + 3, y + g_ctx.globals.flag_count++ * 9, render::fonts::watermark_font_other, "F", false, c);
	}
	if (player->is_scoped()) {
		render::text(x + w + 3, y + g_ctx.globals.flag_count++ * 9, render::fonts::watermark_font_other, "+", false, c);
	}
	if (combat::ragebot::isEnabled) {
		if (g_ragebot.m_last_target == player) {
			render::text(x + w + 3, y + g_ctx.globals.flag_count++ * 9, render::fonts::watermark_font_other, "TARGET", false, c);
		}
	}
}
