#pragma once
#include "../../features.hpp"

/*
class chams : public feature {

public:
	bool isenabled = is_enabled;
	bool teammates = false;
	bool enemies = true;
	bool hands = true;
	int material = 1;

	void onRender_chams(void* _this, int edx, void* ctx, void* state, model_render_info_t& info, matrix3x4_t* customBoneToWorld, hooks::draw_model_execute::fn orig, entity_t* ent);
};
*/

class c_chams : public singleton<c_chams> {

public:
	bool enemies = false;
	bool teammates = false;
	bool hands = false;
	bool weapons = false;
	int material = 1;
	int category = Category::RENDER;
	std::unordered_map<int, std::string> color_map = { {0, "Red"}, {1, "Orange"}, {2, "Yellow"}, {3, "Green"}, {4, "Cyan"}, {5, "Blue"}, {6, "Purple"}, {7, "Pink"},  {8, "White"}, {9, "Black"} };
	std::unordered_map<int, std::string> material_map = { {0, "Normal"}, {1, "Flat"}, {2, "Platinum"}, {3, "Glass"}, {4, "Crystal"}, {5, "Chrome"}, {6, "Silver"}, {7, "Gold"},  {8, "Plastic"}, {9, "Metallic"}, {10, "Pearlescent"}, {11, "Glow"}};

	std::string enemy_material;
	std::string team_material;
	std::string arms_material;
	std::string weapons_material;

	std::string enemy_color;
	std::string team_color;
	std::string arms_color;
	std::string weapons_color;

	std::string enemy_color_occluded;
	std::string team_color_occluded;
	std::string weapons_color_occluded;

public:
	void onChams(void* _this, int edx, void* ctx, void* state, model_render_info_t& info, matrix3x4_t* customBoneToWorld, hooks::draw_model_execute::fn orig, entity_t* ent);


private:
	typedef struct Mat{
		i_material* material;
		std::string m_name;

		Mat(i_material* material, std::string name) {
			this->material = material;
			this->m_name = name;
		}
	} mat;

	i_material* enemy_mat;
	i_material* team_mat;
	i_material* arms_mat;
	i_material* weapons_mat;

	c_chams::mat* get_mat(std::string mat);

	void Override(bool ignoreZ, bool wireframe, bool glass, const color c, mat* mat);

	Mat* normal;
	Mat* flat;
	Mat* platinum;
	Mat* glass;
	Mat* crystal;
	Mat* chrome;
	Mat* silver;
	Mat* gold;
	Mat* plastic;
	Mat* metallic;
	Mat* pearlescent;
	Mat* glow;

	
	void init_materials();

	bool initialized = false;

};

//extern c_chams g_chams;