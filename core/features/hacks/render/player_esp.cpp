#include "../../features.hpp"
#include "../../../menu/variables.hpp"
#include "player_esp.h"

c_player_esp g_player_esp;

void c_player_esp::on_draw()
{
	if (!g_player_esp.is_enabled)
		return;

	if (interfaces::engine->is_in_game())
	{
		if (!csgo::local_player)
			return;

		for (int i = 1; i <= interfaces::globals->max_clients; ++i) {
			auto entity = static_cast<entity_t*>(interfaces::entity_list->get_client_entity(i));

			if (!entity || entity->dormant() || entity == csgo::local_player || entity->health() == 0)
				continue;

			Box b;

			if (entity != nullptr) {

				matrix_t bone_matrices[128];
				if (entity->setup_bones(bone_matrices, 128, 256, 0.0f)) {
					if (entity->team() == csgo::local_player->team()) {
						g_player_esp.draw_skeleton((player_t*)entity, g_player_esp.teammate_color, bone_matrices);
					}
					else {
						g_player_esp.draw_skeleton((player_t*)entity, g_player_esp.enemy_color, bone_matrices);
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

void c_player_esp::draw_2d_box(entity_t* e, const Box& b) {

	color outline_color
	{
		0,
		0,
		0,
		255
	};

	color inside_color
	{
		255,
		255,
		255,
		255
	};


	render::draw_rect(b.x - 1, b.y - 1, b.w + 2, b.h + 2, outline_color);
	render::draw_rect(b.x, b.y, b.w, b.h, inside_color);
	render::draw_rect(b.x + 1, b.y + 1, b.w - 2, b.h - 2, outline_color);
}

void c_player_esp::draw_name(entity_t* e, const Box& b) {

}


void c_player_esp::draw_skeleton(player_t* e, color color, matrix_t matrix[MAXSTUDIOBONES])
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



