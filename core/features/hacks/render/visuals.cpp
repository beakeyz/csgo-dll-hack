#include "visuals.h"

void c_visuals::run_visuals() {


	for (int i = 1; i <= interfaces::entity_list->get_highest_index(); i++) {
		auto entity = static_cast<entity_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity)
			continue;

		this->nightmode(entity);
	}

}

void c_visuals::remove_smoke()
{
	static DWORD smoke_count;
	static uint8_t* offset;

	static auto last_value = false;

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

	if (!offset)
		offset = utilities::pattern_scan("client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
	
	if (!smoke_count)
		smoke_count = *reinterpret_cast<DWORD*>(offset + 0x8);

	if (this->m_edit_smoke)
		*reinterpret_cast<int*>(smoke_count) = 0;

	if (last_value != this->m_edit_smoke) {
		last_value = this->m_edit_smoke;

		for (const auto& name : smoke_materials)
		{
			auto material = interfaces::material_system->find_material(name, TEXTURE_GROUP_CLIENT_EFFECTS);
			if (material)
				material->set_material_var_flag(material_var_wireframe, last_value);
		}
	}
}

void c_visuals::nightmode(entity_t* ent, float override_brightness) {
	if (!interfaces::engine->is_connected() || !csgo::local_player/* || !ent */)
		return;

	auto client_class = ent->client_class();
	if (!client_class || client_class->class_id != cenvtonemapcontroller)
		return;

	float brightness = this->m_nightmode / 100.f;
	if (!this->m_nightmode)
		brightness = 0.001f;

	if (override_brightness > 0.f)
		brightness = override_brightness / 100.f;

	// will not work if post processing is disabled.
	auto tonemap_controller = reinterpret_cast<c_env_tonemap_controller*>(ent);

	tonemap_controller->use_custom_exposure_min() = 1;
	tonemap_controller->use_custom_exposure_max() = 1;

	tonemap_controller->custom_exposure_min() = brightness;
	tonemap_controller->custom_exposure_max() = brightness;
}