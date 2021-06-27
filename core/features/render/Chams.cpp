#include "Chams.h"
#include "../../menu/variables.hpp"



void Override(bool ignoreZ, bool wireframe, bool glass, const color c) {
	i_material* pMat = nullptr;

	switch (Render::Chams::material) {
	case 1:
		pMat = interfaces::material_system->find_material("models/player/ct_fbi/ct_fbi_glass", TEXTURE_GROUP_OTHER);
		break;
	case 2:
		pMat = interfaces::material_system->find_material("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER);
		break;
	case 3:
		pMat = interfaces::material_system->find_material("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_OTHER);
		break;
	case 4:
		pMat = interfaces::material_system->find_material("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER);
		break;
	case 5:
		pMat = interfaces::material_system->find_material("models/gibs/glass/glass", TEXTURE_GROUP_OTHER);
		break;
	case 6:
		pMat = interfaces::material_system->find_material("models/inventory_items/trophy_majors/velvet", TEXTURE_GROUP_OTHER);
		break;
	default:
		pMat = interfaces::material_system->find_material("debug/debugambientcube", TEXTURE_GROUP_OTHER);
		break;
	}

	//if (Render::Chams::t.has_time_elapsed(500, true)) {
	//	Render::Chams::_switch = !Render::Chams::_switch;
	//}

	pMat->set_material_var_flag(material_var_ignorez, ignoreZ);

	if (glass) {
		pMat->alpha_modulate(0.35F);
	}
	else
		pMat->alpha_modulate(1.0F);

	pMat->set_material_var_flag(material_var_wireframe, wireframe);

	pMat->color_modulate(c.r, c.g, c.b);
	if (pMat != nullptr)
		interfaces::model_render->override_material(pMat);
}

void Render::Chams::onChams(void* _this, int edx, void* ctx, void* state, model_render_info_t& info, matrix3x4_t* customBoneToWorld, hooks::draw_model_execute::fn orig, entity_t* ent)
{
	if (!interfaces::engine->is_connected() || interfaces::engine->is_taking_screenshot() || csgo::local_player == nullptr)
		return;

	if (info.model) {

		const char* model_name = interfaces::model_info->get_model_name(info.model);

		if (strstr(model_name, "models/player"))
		{
			if (interfaces::engine->is_connected()) {
				if (ent && !ent->dormant() && ent->health() != 0) {
					if (ent->team() == csgo::local_player->team() && Render::Chams::teammates) {
						Override(true, false, false, Render::Chams::team_color_invisible);
						orig(_this, edx, ctx, state, info, customBoneToWorld);
						Override(false, false, false, Render::Chams::team_color_visible);
					}
					else if (ent->team() != csgo::local_player->team() && Render::Chams::enemies) {
						Override(true, false, false, Render::Chams::enemy_color_invisible);
						orig(_this, edx, ctx, state, info, customBoneToWorld);
						Override(false, false, false, Render::Chams::enemy_color_visible);
					}
				}
			}
		}
		if (strstr(model_name, "arms") && Render::Chams::hands)
		{
			if (csgo::local_player->health() != 0 && !csgo::local_player->dormant() && interfaces::engine->is_connected()) {
				if (csgo::local_player && csgo::local_player->health() != 0 && !csgo::local_player->dormant() && interfaces::engine->is_connected()) {
					Override(false, false, false, Render::Chams::hands_color);
					orig(_this, edx, ctx, state, info, customBoneToWorld);
				}
			}
		}
	}
}

void Render::Chams::onEnable() {
	//Render::Chams::t.reset();

}