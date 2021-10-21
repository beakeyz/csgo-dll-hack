#include "../../features.hpp"
#include "../../../menu/variables.hpp"
#include "player_esp.h"

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
						
						this->draw_skeleton((player_t*)entity, this->get_color(this->color_map, this->team_color), bone_matrices);
					}
					else if(this->enemies){

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



