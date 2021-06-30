#include "../../menu/variables.hpp"
#include "../features.hpp"

void Render::Glow::EndGlow() {

	for (auto i = 0; i < interfaces::glow_manager->size; i++) {
		auto& glowObject = interfaces::glow_manager->objects[i];
		auto entity = reinterpret_cast<entity_t*>(glowObject.entity);

		if (glowObject.unused())
			continue;

		if (!entity || entity->dormant())
			continue;

		glowObject.alpha = 0.0f;
	}
}

void Render::Glow::RunGlow() {

	if (!Render::Glow::isEnabled)
		return;

	if (!interfaces::engine->is_in_game())
		return;

	for (auto i = 0; i < interfaces::glow_manager->size; i++) {
		auto& glowObject = interfaces::glow_manager->objects[i];
		auto entity = reinterpret_cast<entity_t*>(glowObject.entity);

		if (glowObject.unused())
			continue;

		if (!entity || entity->dormant())
			continue;

		//auto color = Color{};

		if (entity->client_class()->class_id == ccsplayer)
		{
			if (entity->health() <= 0)
				continue;



			//if (Vars.glow_visible_only && !entity->IsVisible())
			//	continue;
			/*
			Color team_color = entity->IsVisible()
				? Color(Vars.color_glow_team_visible[0],
					Vars.color_glow_team_visible[1],
					Vars.color_glow_team_visible[2],
					Vars.color_glow_team_visible[3])
				:
				Color(Vars.color_glow_team_hidden[0],
					Vars.color_glow_team_hidden[1],
					Vars.color_glow_team_hidden[2],
					Vars.color_glow_team_hidden[3]);


			Color enemy_color = entity->IsVisible()
				? Color(Vars.color_glow_enemy_visible[0],
					Vars.color_glow_enemy_visible[1],
					Vars.color_glow_enemy_visible[2],
					Vars.color_glow_enemy_visible[3])
				:
				Color(Vars.color_glow_enemy_hidden[0],
					Vars.color_glow_enemy_hidden[1],
					Vars.color_glow_enemy_hidden[2],
					Vars.color_glow_enemy_hidden[3]);

			color = entity->IsTeammate() ? team_color : enemy_color;
			*/
		}
		else {
			continue;
		}

		glowObject.color = vec3_t{ 255, 0, 0 };//color.r() / 255.0f;
		//glowObject.m_flGreen = color.g() / 255.0f;
		//glowObject.m_flBlue = color.b() / 255.0f;
		glowObject.alpha = 255.0f;
		glowObject.render_when_occluded = true;
		glowObject.render_when_unoccluded = false;
	}

}