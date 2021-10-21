#include "Chams.h"
#include "../../../menu/variables.hpp"
#include "../features.h"

//c_chams g_chams;


void c_chams::init_materials() {

	std::ofstream("./../../chamsAnimated.vmt") << R"#("VertexLitGeneric" {
	"$envmap" "env_cubemap"
    "$envmapcontrast" "0.5"
    "$envmaptint" "[.3 .7 .7]"
    "$basetexture" "dev/zone_warning"
	"proxies"
     {
		"texturescroll"
        {
            "texturescrollvar" "$basetexturetransform"
            "texturescrollrate" "0.6"
            "texturescrollangle" "90"
        }
    }
 
	})#";

	std::ofstream("./../../chrome.vmt") << R"#("VertexLitGeneric" {
	"$envmap" "env_cubemap"
 
	})#";

	std::ofstream("./../../pearlescent.vmt") << R"#("VertexLitGeneric" {
	"$ambientonly" "1" 
	"$phong" "1"
	"$pearlescent" "3" 
	"$basemapalphaphongmask" "1"
	})#";


	std::ofstream("./../../metallic.vmt") << R"#("VertexLitGeneric" { 
	"$basetexture" "white"
	"$ignorez" "0"
	"$envmap" "env_cubemap"
	"$normalmapalphaenvmapmask" "1"
	"$envmapcontrast" "1"
	"$nofog" "1"
	"$model" "1"
	"$nocull" "0"
	"$selfillum" "1"
	"$halfambert" "1"
	"$znearer" "0"
	"$flat" "1"
 }
)#";

	std::ofstream("./../../flat.vmt") << R"#("UnlitGeneric" {
 }
)#";

	std::ofstream("./../../normal.vmt") << R"#("VertexLitGeneric" {
 }
)#";

	std::ofstream("./../../test.vmt") << R"#("VertexLitGeneric" {
	"$additive" "1"
	"$envmap" "models/effects/cube"
	"$envmapfresnel" "1"
	"$alpha" ".3" 
})#";

	normal = new Mat(interfaces::material_system->find_material("../../../../normal", TEXTURE_GROUP_OTHER), "Normal");
	
	flat = new Mat(interfaces::material_system->find_material("../../../../flat", TEXTURE_GROUP_MODEL), "Flat");

	platinum = new Mat(interfaces::material_system->find_material("models/player/ct_fbi/ct_fbi_glass", TEXTURE_GROUP_OTHER), "Platinum");

	glass = new Mat(interfaces::material_system->find_material("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER), "Glass");

	crystal = new Mat(interfaces::material_system->find_material("../../../../crystal", TEXTURE_GROUP_OTHER), "Crystal");

	chrome = new Mat(interfaces::material_system->find_material("../../../../chrome", TEXTURE_GROUP_OTHER), "Chrome");
	
	silver = new Mat(interfaces::material_system->find_material("../../../../test", TEXTURE_GROUP_OTHER), "Silver");
	
	gold = new Mat(interfaces::material_system->find_material("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER), "Gold");
	
	plastic = new Mat(interfaces::material_system->find_material("debug/debugambientcube", TEXTURE_GROUP_OTHER), "Plastic");
	
	metallic = new Mat(interfaces::material_system->find_material("../../../../metallic", TEXTURE_GROUP_OTHER), "Metallic");
	
	pearlescent = new Mat(interfaces::material_system->find_material("../../../../pearlescent", TEXTURE_GROUP_OTHER), "Pearlescent");
	
	glow = new Mat(interfaces::material_system->find_material("../../../../chamsAnimated", TEXTURE_GROUP_OTHER), "Glow");

	initialized = true;
}

c_chams::Mat* c_chams::get_mat(std::string mat) {
	static std::vector<Mat*> materials = { normal, flat, platinum, glass, crystal, chrome, silver, gold, plastic, metallic, pearlescent, glow };

	for (c_chams::Mat* type : materials) {
		if (type->m_name == mat) {
			return type;
		}
	}
	return nullptr;
}

void c_chams::Override(bool ignoreZ, bool wireframe, bool flat, const color c, c_chams::Mat* mat) {

	i_material* pMat = mat->material;

	if (pMat == nullptr)
		return;

	pMat->set_material_var_flag(material_var_ignorez, ignoreZ);
	pMat->set_material_var_flag(material_var_wireframe, wireframe);
	pMat->set_material_var_flag(material_var_flat, flat);

	pMat->alpha_modulate(c.a / 255.0F);

	pMat->color_modulate(c.r / 255.0F, c.g / 255.0F, c.b / 255.0F);
	
	interfaces::model_render->override_material(pMat);
	
}

void c_chams::onChams(void* _this, int edx, void* ctx, void* state, model_render_info_t& info, matrix3x4_t* customBoneToWorld, hooks::draw_model_execute::fn orig, entity_t* ent)
{
	if (!initialized)
		this->init_materials();

	if (!interfaces::engine->is_connected() || interfaces::engine->is_taking_screenshot() || csgo::local_player == nullptr)
		return;

	if (info.model) {

		const char* model_name = interfaces::model_info->get_model_name(info.model);

		if (strstr(model_name, "models/player"))
		{
			if (interfaces::engine->is_connected()) {
				if (ent && !ent->dormant() && ent->health() != 0) {
					if (ent->team() == csgo::local_player->team() && teammates) {
						Override(true, false, false, c_skeleton_esp::get_ptr()->get_color(this->color_map, this->team_color_occluded), this->get_mat(this->team_material));
						orig(_this, edx, ctx, state, info, customBoneToWorld);
						Override(false, false, false, c_skeleton_esp::get_ptr()->get_color(this->color_map, this->team_color), this->get_mat(this->team_material));
					}
					else if (ent->team() != csgo::local_player->team() && enemies) {
						Override(true, false, false, c_skeleton_esp::get_ptr()->get_color(this->color_map, this->enemy_color_occluded), this->get_mat(this->enemy_material));
						orig(_this, edx, ctx, state, info, customBoneToWorld);
						Override(false, false, false, c_skeleton_esp::get_ptr()->get_color(this->color_map, this->enemy_color), this->get_mat(this->enemy_material));

					}
				}
			}
		}
		if (strstr(model_name, "arms") && hands)
		{
			if (csgo::local_player->health() != 0 && !csgo::local_player->dormant() && interfaces::engine->is_connected()) {
				if (csgo::local_player && csgo::local_player->health() != 0 && !csgo::local_player->dormant() && interfaces::engine->is_connected()) {
					Override(false, false, false, c_skeleton_esp::get_ptr()->get_color(this->color_map, this->arms_color), this->get_mat(this->arms_material));
					orig(_this, edx, ctx, state, info, customBoneToWorld);
				}
			}
		}

		if (weapons) {
			if (strstr(model_name, "weapons/v")) //Weapons in hand.
			{
				if (!strstr(model_name, "arms") && !strstr(model_name, "ied_dropped")) //ied is c4 on ground/on back.
				{
					if (csgo::local_player->health() != 0 && !csgo::local_player->dormant() && interfaces::engine->is_connected()) {
						if (csgo::local_player && csgo::local_player->health() != 0 && !csgo::local_player->dormant() && interfaces::engine->is_connected()) {
							Override(false, false, false, c_skeleton_esp::get_ptr()->get_color(this->color_map, this->weapons_color), this->get_mat(this->weapons_material));
							orig(_this, edx, ctx, state, info, customBoneToWorld);

						}
					}
				}
			}
			if (strstr(model_name, "_dropped.mdl") && strstr(model_name, "weapons/w")) //Weapons on back.
			{
				if (!strstr(model_name, "arms") && !strstr(model_name, "ied_dropped")) //ied is c4 on ground/on back.
				{
					if (csgo::local_player->health() != 0 && !csgo::local_player->dormant() && interfaces::engine->is_connected()) {
						if (csgo::local_player && csgo::local_player->health() != 0 && !csgo::local_player->dormant() && interfaces::engine->is_connected()) {
							Override(false, false, false, c_skeleton_esp::get_ptr()->get_color(this->color_map, this->weapons_color_occluded), this->get_mat(this->weapons_material));
							orig(_this, edx, ctx, state, info, customBoneToWorld);
							Override(true, false, false, c_skeleton_esp::get_ptr()->get_color(this->color_map, this->weapons_color), this->get_mat(this->weapons_material));

						}
					}
				}
			}
		}
	}
}