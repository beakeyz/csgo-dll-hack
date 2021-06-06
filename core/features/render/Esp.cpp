#include "../features.hpp"
#include "../../menu/variables.hpp"

void Render::Esp::onEsp()
{
	if (!Render::Esp::isEnabled)
		return;

	if (interfaces::engine->is_in_game())
	{
		if (!csgo::local_player)
			return;

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
			if (interfaces::debug_overlay->world_to_screen(entityHead, positionHead)) {
				if (interfaces::debug_overlay->world_to_screen(*entityOrigin, positionFeet)) {
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

	}
}
