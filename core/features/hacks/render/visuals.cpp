#include "visuals.h"

void c_visuals::full_bright() {

	if (!csgo::local_player || !interfaces::engine->is_in_game())
		return;

	static auto mat_fullbright = interfaces::console->get_convar("mat_fullbright");

	if (mat_fullbright->float_value != this->m_full_bright)
		mat_fullbright->set_value(this->m_full_bright);
}

void c_visuals::remove_smoke()
{
	if (csgo::local_player && this->m_edit_smoke)
	{
		static auto smoke_count = *reinterpret_cast<uint32_t**>(utilities::pattern_scan("client.dll", "A3 ? ? ? ? 57 8B CB") + 0x1);
		*(int*)smoke_count = 0;
	}

	if (g_ctx.globals.should_remove_smoke == interfaces::engine->is_in_game() && c_visuals::get().m_edit_smoke)
		return;

	g_ctx.globals.should_remove_smoke = interfaces::engine->is_in_game() && c_visuals::get().m_edit_smoke;

	static std::vector <const char*> smoke_materials =
	{
		"effects/overlaysmoke",
		"particle/beam_smoke_01",
		"particle/particle_smokegrenade",
		"particle/particle_smokegrenade1",
		"particle/particle_smokegrenade2",
		"particle/particle_smokegrenade3",
		"particle/particle_smokegrenade_sc",
		"particle/smoke1/smoke1",
		"particle/smoke1/smoke1_ash",
		"particle/smoke1/smoke1_nearcull",
		"particle/smoke1/smoke1_nearcull2",
		"particle/smoke1/smoke1_snow",
		"particle/smokesprites_0001",
		"particle/smokestack",
		"particle/vistasmokev1/vistasmokev1",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_nearcull",
		"particle/vistasmokev1/vistasmokev1_nearcull_fog",
		"particle/vistasmokev1/vistasmokev1_nearcull_nodepth",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev4_emods_nocull",
		"particle/vistasmokev1/vistasmokev4_nearcull",
		"particle/vistasmokev1/vistasmokev4_nocull"
	};

	for (auto material_name : smoke_materials)
	{
		auto material = interfaces::material_system->find_material(material_name, nullptr);

		if (!material)
			continue;

		material->set_material_var_flag(material_var_no_draw, this->m_edit_smoke);
		material->set_material_var_flag(material_var_wireframe, this->smoke_wireframe);
	}
}