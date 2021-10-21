#include "../../dependencies/utilities/csgo.hpp"
#include "../features/features.hpp"
#include "../features/hacks/misc/engine_prediction.hpp"
#include "../menu/menu.hpp"
#include "../../dependencies/interfaces/Framestage.h"
#include "../features/hacks/features.h"

/*
make sure unused hookfunctions are commented out!
otherwise game go boom
*/

//hooks::doPostScreenEffects::fn do_post_screen_original = nullptr;
//hooks::EndScene::fn EndScene_original = nullptr;

hooks::create_move::fn create_move_original = nullptr;
hooks::paint_traverse::fn paint_traverse_original = nullptr;

hooks::draw_model_execute::fn draw_model_execute_original = nullptr;
hooks::frame_stage_notify::fn frame_stage_notify_original = nullptr;
hooks::SceneEnd::fn SceneEnd_original = nullptr;

vec3_t flb_aim_punch;
vec3_t flb_view_punch;

vec3_t* aim_punch;
vec3_t* view_punch;

bool hooks::initialize() {
	const auto create_move_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 24));
	const auto paint_traverse_target = reinterpret_cast<void*>(get_virtual(interfaces::panel, 41));
	const auto do_post_screen_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 44));
	const auto draw_model_execute_target = reinterpret_cast<void*>(get_virtual(interfaces::model_render, 21));
	const auto frame_stage_notify_target = reinterpret_cast<void*>(get_virtual(interfaces::client, 37));
	const auto SceneEnd_target = reinterpret_cast<void*>(get_virtual(interfaces::render_view, 9));
	const auto EndScene_target = reinterpret_cast<void*>(get_virtual(interfaces::directx, 42));

	if (MH_Initialize() != MH_OK)
		throw std::runtime_error("failed to initialize MH_Initialize.");
	else if (MH_CreateHook(create_move_target, &create_move::hook, reinterpret_cast<void**>(&create_move_original)) != MH_OK)
		throw std::runtime_error("failed to initialize create_move. (outdated index?)");

	else if (MH_CreateHook(paint_traverse_target, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse_original)) != MH_OK)
		throw std::runtime_error("failed to initialize paint_traverse. (outdated index?)");

	//else if (MH_CreateHook(do_post_screen_target, &doPostScreenEffects::hook, reinterpret_cast<void**>(&do_post_screen_original)) != MH_OK)
	//	throw std::runtime_error("failed to initialize post_screen. (outdated index?)");
	//else if (MH_CreateHook(EndScene_target, &EndScene::hook, reinterpret_cast<void**>(&EndScene_original)) != MH_OK)
	//	throw std::runtime_error("failed to initialize EndScene. (outdated index?)");

	else if (MH_CreateHook(draw_model_execute_target, &draw_model_execute::hook, reinterpret_cast<void**>(&draw_model_execute_original)) != MH_OK)
		throw std::runtime_error("failed to initialize draw_model_execute. (outdated index?)");

	else if (MH_CreateHook(frame_stage_notify_target, &frame_stage_notify::hook, reinterpret_cast<void**>(&frame_stage_notify_original)) != MH_OK)
		throw std::runtime_error("failed to initialize frame_stage_notify. (outdated index?)");

	else if (MH_CreateHook(SceneEnd_target, &SceneEnd::hook, reinterpret_cast<void**>(&SceneEnd_original)) != MH_OK)
		throw std::runtime_error("failed to initialize SceneEnd. (outdated index?)");

	

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		throw std::runtime_error("failed to enable hooks.");

	console::log("[setup] hooks initialized!\n");
	

	return true;

}

void hooks::release() {

	Render::Glow::EndGlow();

	MH_Uninitialize();

	MH_DisableHook(MH_ALL_HOOKS);

}

void __fastcall hooks::frame_stage_notify::hook(void* _this, int edx, FrameStage stage) {

	aim_punch = nullptr;
	view_punch = nullptr;

	flb_aim_punch = { 0.0f, 0.0f, 0.0f };
	flb_view_punch = { 0.0f, 0.0f, 0.0f };

	if (stage == FrameStage::RENDER_START)
	{
		c_visuals::get().remove_smoke();

		if (c_visuals::get().visual_recoil && interfaces::engine->is_in_game())
		{
			aim_punch = &csgo::local_player->aim_punch_angle();
			view_punch = &csgo::local_player->punch_angle();

			flb_aim_punch = *aim_punch;
			flb_view_punch = *view_punch;

			(*aim_punch) = { 0.0f, 0.0f, 0.0f };
			(*view_punch) = { 0.0f, 0.0f, 0.0f };
		}
	}

	if (stage == FrameStage::RENDER_END)
	{
		c_visuals::get().full_bright();
	}

	if (stage == FrameStage::NET_UPDATE_POSTDATAUPDATE_START)
	{
		combat::resolver::resolver();
		//visuals::color_world();
	}

	frame_stage_notify_original(interfaces::client, edx, stage);

	if (csgo::local_player && c_visuals::get().visual_recoil && csgo::local_player->is_alive() && aim_punch && view_punch)
	{
		*aim_punch = flb_aim_punch;
		*view_punch = flb_view_punch;
	}
}

void __fastcall hooks::draw_model_execute::hook(void* _this, int edx, void* ctx, void* state, model_render_info_t& info, matrix3x4_t* customBoneToWorld) {
	if (interfaces::engine->is_in_game() && interfaces::engine->is_connected()) {

		if (ctx && customBoneToWorld) {
			//TODO: this might be broken, check and capitalize
			//original: if (c_chams::get_ptr()->isEnabled)
			if (c_chams::get_ptr()->teammates || c_chams::get_ptr()->enemies || c_chams::get_ptr()->weapons || c_chams::get_ptr()->hands) {
				c_chams::get_ptr()->onChams(_this, edx, ctx, state, info, customBoneToWorld, draw_model_execute_original, static_cast<entity_t*>(interfaces::entity_list->get_client_entity(info.entity_index)));

				draw_model_execute_original(_this, edx, ctx, state, info, customBoneToWorld);
				interfaces::model_render->override_material(nullptr);
			}
		}
	}
	//TODO: this might be broken, check and capitalize
	//original: if (!c_chams::get_ptr()->isEnabled)
	if (!c_chams::get_ptr()->teammates && !c_chams::get_ptr()->enemies && !c_chams::get_ptr()->weapons && !c_chams::get_ptr()->hands) {
		draw_model_execute_original(_this, edx, ctx, state, info, customBoneToWorld);
	}
}

void __fastcall hooks::SceneEnd::hook(void* _this, void* edx) {
	SceneEnd_original(_this, edx);


	Render::Glow::RunGlow();
}

/*
long __stdcall hooks::EndScene::hook(IDirect3DDevice9* pDevice)
{
	return EndScene_original(pDevice);
}
*/

bool __stdcall hooks::create_move::hook(float input_sample_frametime, c_usercmd* cmd) {
	create_move_original(input_sample_frametime, cmd);

	if (!cmd || !cmd->command_number)
		return false;

	csgo::local_player = static_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	auto old_viewangles = cmd->viewangles;
	auto old_forwardmove = cmd->forwardmove;
	auto old_sidemove = cmd->sidemove;
	g_ctx.globals.wish_angle = cmd->viewangles;

	//where all the hacks go
	movement::bunny_hop::bunny_hop(cmd);
	//misc::movement::fastStop(cmd);
	//misc::movement::air_strafe(cmd);

	//misc::no_flash(cmd);

	//variables::bot->aim_bot(cmd);
	combat::aimbot::aim_bot(cmd);
	combat::triggerbot::triggerbot(cmd);

	g_ragebot.unpredicted_vel = csgo::local_player->velocity();

	prediction::start(cmd); {

		g_ragebot.work(cmd);
		
		movement::auto_strafe::auto_strafe(cmd);

		combat::antiaim::run(cmd, g_ctx.globals.send_packet);

		c_fakelag::get_ptr()->think(cmd, g_ctx.globals.send_packet);

	} prediction::end();

	//math::correct_movement(old_viewangles, cmd, old_forwardmove, old_sidemove);

	math::CorrectMovement(old_viewangles, cmd, old_forwardmove, old_sidemove);

	//cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
	//cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
	//cmd->upmove = std::clamp(cmd->upmove, -320.0f, 320.0f);

	cmd->viewangles.normalize();
	cmd->viewangles.x = std::clamp(cmd->viewangles.x, -89.0f, 89.0f);
	cmd->viewangles.y = std::clamp(cmd->viewangles.y, -180.0f, 180.0f);
	cmd->viewangles.z = 0.0f;
	
	uintptr_t* frame_pointer;
	__asm mov frame_pointer, ebp;
	*reinterpret_cast<bool*>(*frame_pointer - 0x1C) = g_ctx.globals.send_packet;

	return false;
}

/*
int __fastcall hooks::doPostScreenEffects::hook(void* _this, int edx, int param) {

	//visuals::glow();

	return do_post_screen_original(interfaces::clientmode, edx, param);
}
*/

void __stdcall hooks::paint_traverse::hook(unsigned int panel, bool force_repaint, bool allow_force) {
	auto panel_to_draw = fnv::hash(interfaces::panel->get_panel_name(panel));
	static const std::string watermark = "BeakonCS - V2";

	switch (panel_to_draw) {
	case fnv::hash("MatSystemTopPanel"):
		
		//all the component go out of scope here, so they might get reverted to nullptrs

		render::draw_filled_rect(5, 15, render::get_text_size(render::fonts::font_bigboi, watermark).x + 10, 25, color::black(245));
		
		render::draw_line(5, 40, render::get_text_size(render::fonts::font_bigboi, watermark).x + 14, 40, color::white(255));
		render::draw_line(5, 41, render::get_text_size(render::fonts::font_bigboi, watermark).x + 14, 41, color::white(255));


		render::text(10, 16, render::fonts::font_bigboi, watermark, false, color::white(255));
		//visuals::twoD_box();
		//visuals::snap_lines();
		c_skeleton_esp::get_ptr()->on_draw();

		//menu::toggle();
		c_menu::get_ptr()->toggle();

		c_menu::get_ptr()->render();
		//menu::render();

		break;

	case fnv::hash("FocusOverlayPanel"):
		interfaces::panel->set_keyboard_input_enabled(panel, c_menu::get_ptr()->is_open);
		interfaces::panel->set_mouse_input_enabled(panel, c_menu::get_ptr()->is_open);
		break;
	}

	paint_traverse_original(interfaces::panel, panel, force_repaint, allow_force);
}
