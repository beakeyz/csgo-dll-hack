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
	bool enemies = true;
	bool teammates = false;
	bool hands = true;
	int material = 1;
	int category = Category::RENDER;
	bool isEnabled = false;
	color team_color_visible = { 0, 255, 0 };
	color enemy_color_visible = { 255, 0, 0 };
	color team_color_invisible = { 0, 255, 255 };
	color enemy_color_invisible = { 255, 100, 200 };
	color hands_color = { 255, 255, 255 };

public:
	void onChams(void* _this, int edx, void* ctx, void* state, model_render_info_t& info, matrix3x4_t* customBoneToWorld, hooks::draw_model_execute::fn orig, entity_t* ent);

private:
	void Override(bool ignoreZ, bool wireframe, bool glass, const color c);

};

//extern c_chams g_chams;