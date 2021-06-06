#pragma once
#include "../../dependencies/interfaces/FrameStage.h"

namespace hooks {
	bool initialize();
	void release();

	inline unsigned int get_virtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*static_cast<int**>(_class))[index]); }

	namespace create_move {
		using fn = bool(__stdcall*)(float, c_usercmd*);
		bool __stdcall hook(float input_sample_frametime, c_usercmd* cmd);
	};
	namespace doPostScreenEffects {
		using fn = int(__fastcall*)(void*, int, int);
		int __fastcall hook(void* _this, int edx, int params);
	};

	namespace paint_traverse {
		using fn = void(__thiscall*)(i_panel*, unsigned int, bool, bool);
		void __stdcall hook(unsigned int panel, bool force_repaint, bool allow_force);
	};

	namespace draw_model_execute {
		using fn = void(__fastcall*)(void*, int, void*, void*, model_render_info_t&, matrix3x4_t*);
		void __fastcall hook(void* _this, int edx, void* ctx, void* state, model_render_info_t& info, matrix3x4_t* customBoneToWorld);
	};
	namespace frame_stage_notify {
		using fn = void(__fastcall*)(void*, int, FrameStage);
		void __fastcall hook(void* _this, int edx, FrameStage stage);
	};
	namespace SceneEnd {
		using fn = void(__fastcall*)(void*, void*);
		void __fastcall hook(void* _this, void* edx);
	};
	namespace EndScene {
		using fn = long(__stdcall*)(IDirect3DDevice9* pDevice);
		long __stdcall hook(IDirect3DDevice9* pDevice);
	}

}
