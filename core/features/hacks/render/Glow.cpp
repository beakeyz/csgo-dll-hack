#include "../../../menu/variables.hpp"
#include "../../features.hpp"
#include "./glow.h"
#include "./../features.h"

void c_glow::endGlow() {

	for (auto i = 0; i < interfaces::glow_manager->size; i++) {
		auto& glowObject = interfaces::glow_manager->objects[i];
		auto entity = reinterpret_cast<entity_t*>(glowObject.m_pEntity);

		if (glowObject.unused())
			continue;

		glowObject.m_flAlpha = 0.0f;
	}
}

void c_glow::runGlow() {

	if (!this->teammates && !this->enemies)
		return;

	if (!interfaces::engine->is_in_game())
		return;

	for (auto i = 0; i < interfaces::glow_manager->size; i++) {
		auto& glowObject = interfaces::glow_manager->objects[i];
		auto entity = reinterpret_cast<entity_t*>(glowObject.m_pEntity);

		if (glowObject.unused())
			continue;

		if (!entity || entity->dormant())
			continue;

		//auto color = Color{};
		color c;

		if (entity->client_class()->class_id == class_ids::ccsplayer)
		{
			if (entity->health() <= 0)
				continue;

			if (entity->team() == csgo::local_player->team() && this->teammates)
				c = c_skeleton_esp::get_ptr()->get_color(this->color_map, this->team_color);
			else if (entity->team() != csgo::local_player->team() && this->enemies)
				c = c_skeleton_esp::get_ptr()->get_color(this->color_map, this->enemy_color);
			else
				continue;
		}
		else {
			continue;
		}

		glowObject.m_nGlowStyle = 0;
		glowObject.m_vecClr = vec3_t{ c.r * 1.0F, c.g * 1.0F, c.b * 1.0F };
		glowObject.m_flAlpha = c.a * 1.0F;
		glowObject.m_bRenderWhenUnoccluded = false;
		glowObject.m_bRenderWhenOccluded = true;
	}
}