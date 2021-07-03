#include "../../features.hpp"
#include "../../../menu/variables.hpp"
#include "player_esp.h"

c_player_esp g_player_esp;

void c_player_esp::on_draw()
{
	//if (!Render::Esp::isEnabled)
	//	return;

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
				color inside_color
				{
					255,
					255,
					255,
					255
				};

				matrix_t bone_matrices[128];
				if (entity->setup_bones(bone_matrices, 128, 256, 0.0f)) {
					g_player_esp.draw_skeleton((player_t*)entity, inside_color, bone_matrices);
				}

				if (utilities::get_bbox(entity, b, true)) {


					//render::draw_rect(b.x, b.y, 10, 10, inside_color);
					//draw_2d_box(entity, b);
					//draw_name(entity, b);
				}
			}
		}


		/*

		for (int i = 1; i <= interfaces::globals->max_clients; ++i) {
			auto entity = static_cast<entity_t*>(interfaces::entity_list->get_client_entity(i));

			if (!entity || entity->dormant() || entity == csgo::local_player || entity->health() == 0)
				continue;

			int width = GetSystemMetrics(SM_CXSCREEN);
			int height = GetSystemMetrics(SM_CYSCREEN);

			vec3_t entityHead = entity->getBonePos(8);
			vec3_t* entityOrigin = &entity->origin();
			vec3_t positionHead;
			vec3_t positionFeet;

			entityHead.z += 8;
			if (math::world_to_screen(entityHead, positionHead)) {
				if (math::world_to_screen(*entityOrigin, positionFeet)) {
					positionHead.x = positionFeet.x;

					//visuals::Draw2DBox(positionHead, positionFeet, 255, 255, 255);
					int height = abs(positionHead.y - positionFeet.y);
					vec2_t tl, tr;
					tl.x = positionHead.x - height / 3.5;
					tr.x = positionHead.x + height / 3.5;
					tl.y = positionHead.y;
					tr.y = positionHead.y;

					vec2_t bl, br;
					bl.x = positionFeet.x - height / 3.5;
					br.x = positionFeet.x + height / 3.5;
					bl.y = positionFeet.y;
					br.y = positionFeet.y;

					interfaces::surface->set_drawing_color(Render::Esp::R, Render::Esp::G, Render::Esp::B);
					interfaces::surface->draw_line(tl.x, tl.y, tr.x, tr.y);
					interfaces::surface->draw_line(bl.x, bl.y, br.x, br.y);
					interfaces::surface->draw_line(tl.x, tl.y, bl.x, bl.y);
					interfaces::surface->draw_line(tr.x, tr.y, br.x, br.y);

					//interfaces::surface->set_drawing_color(255, 255, 255);
					//interfaces::surface->draw_filled_rectangle(positionFeet.x - w * 0.5F, positionHead.y, 1, (positionFeet.y - positionHead.y) + 1);
					//interfaces::surface->draw_filled_rectangle(positionFeet.x + w * 0.5F, positionHead.y, 1, (positionFeet.y - positionHead.y) + 2);
					//interfaces::surface->draw_filled_rectangle(positionFeet.x - w * 0.5F, positionHead.y, (positionFeet.x + w * 0.5F) - (positionFeet.x - w * 0.5F) + 1, 1);
					//interfaces::surface->draw_filled_rectangle(positionFeet.x - w * 0.5F, positionFeet.y, (positionFeet.x + w * 0.5F) - (positionFeet.x - w * 0.5F) + 2, 1);

				}
			}
		}

		*/

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



