#include "../../features.hpp"
#include "../../../menu/variables.hpp"
#include "player_esp.h"
#include "./../features.h"

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
		return color(255, 100, 0, 255); //orange
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
		return color(0, 0, 0, 255); //black
	}
	return color(0, 0, 0, 255);
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

				if (entity->team() == csgo::local_player->team() && (this->type[0].boxes || this->type[0].names)) {
					c_player_esp::get_ptr()->player((player_t*)entity);
						
				}
				else if (entity->team() != csgo::local_player->team() && (this->type[1].boxes || this->type[1].names)) {
					c_player_esp::get_ptr()->player((player_t*)entity);
				}
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

	/*
	const auto name_color = OSHColor::FromARGB(g_vars.visuals.name_color, m_alpha.at(index));
	const auto box_color = OSHColor::FromARGB(g_vars.visuals.box_color, m_alpha.at(index));
	const auto ammo_bar_color = OSHColor::FromARGB(g_vars.visuals.ammo_bar_color, m_alpha.at(index));
	const auto weapon_color = OSHColor::FromARGB(g_vars.visuals.weapon_color, m_alpha.at(index));
	const auto flag_color = OSHColor::FromARGB(g_vars.visuals.flag_color, m_alpha.at(index));
	const auto money_color = OSHColor::FromARGB(g_vars.visuals.money_color, m_alpha.at(index));
	const auto flashed_color = OSHColor::FromARGB(g_vars.visuals.flash_bar_color, m_alpha.at(index));
	*/
	//m_ctx.flag_count = 0;
	//m_ctx.offset = 0;

	bool is_team = (entity->team() == csgo::local_player->team());

	if ((this->type[0].boxes && is_team) || (this->type[1].boxes && !is_team)){
		

		draw_box(is_team ? color::white() : color::red(), index, box.x, box.y, box.w, box.h);
	}
	
	if ((this->type[0].names && is_team) || (this->type[1].names && !is_team)) {
		player_info_t info;
		interfaces::engine->get_player_info(index, &info);

		std::string name = info.name;

		if (name.length() > 17)
			name = name.substr(0, 17) + "...";

		//std::transform( name.begin( ), name.end( ), name.begin( ), tolower );
		render::text(box.x - render::get_text_size(render::fonts::watermark_font_other, name).x / 2 + box.w * 0.5, box.y - render::get_text_size(render::fonts::watermark_font_other, name).y, render::fonts::watermark_font_other, name, false, color::white());
	}
	/*
	if (g_vars.visuals.healthbar)
		draw_healthbar(entity, box.x, box.y, box.w, box.h);

	if (g_vars.visuals.flash_bar)
		flash_bar(entity, flashed_color, box.x, box.y, box.w, box.h);

	if (g_vars.visuals.ammo_bar)
		ammo_bar(weapon, entity, ammo_bar_color, box.x, box.y, box.w, box.h, weapon->clip());

	if (g_vars.visuals.weapon)
		weapon_name(weapon, entity, weapon_color, box.x, box.y, box.w, box.h);

	if (g_vars.visuals.money)
		g_renderer.ansi_text(g_renderer.m_fonts.at(FONT_04B03_6PX),
			money_color,
			OSHColor::FromARGB(220, 10, 10, 10),
			box.x + box.w + 3, box.y + m_ctx.flag_count++ * 8, OUTLINED, "$%i", entity->money());

	if (g_vars.visuals.flags)
		draw_flags(entity, flag_color, box.x, box.y, box.w, box.h);
		*/
}

void c_player_esp::draw_box(color c, int index, float x, float y, float w, float h) {
	render::draw_rect(x - 1, y - 1, w + 2, h + 2, color(10, 10, 10, 220));
	render::draw_rect(x + 1, y + 1, w - 2, h - 2, color(10, 10, 10, 220));
	render::draw_rect(x, y, w, h, c);
}
