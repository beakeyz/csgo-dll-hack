#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "./../menu/utils/timer.h"

namespace Category
{
	constexpr auto COMBAT = -4;
	constexpr auto MOVEMENT = -3;
	constexpr auto MISC = -2;
	constexpr auto RENDER = -1;
};

namespace Render {
	namespace Chams {
		


		/*
		inline int TeamRedVisible = 255;
		inline int TeamGreenVisible = 255;
		inline int TeamBlueVisible = 255;
		inline int TeamRedInVisible = 255;
		inline int TeamGreenInVisible = 255;
		inline int TeamBlueInVisible = 255;
		inline int EnemyRedVisible = 255;
		inline int EnemyGreenVisible = 255;
		inline int EnemyBlueVisible = 255;
		inline int EnemyRedInVisible = 255;
		inline int EnemyGreenInVisible = 255;
		inline int EnemyBlueInVisible = 255;
		inline int armsColor = 200;
		*/
		//void onChams(void* _this, int edx, void* ctx, void* state, model_render_info_t& info, matrix3x4_t* customBoneToWorld, hooks::draw_model_execute::fn orig, entity_t* ent);
		//void onEnable();
	};
	namespace Esp {
		inline int category = Category::RENDER;
		inline bool isEnabled = false;
		inline int R = 255;
		inline int G = 255;
		inline int B = 255;

		//void onEsp();

	}
};
namespace movement {
	namespace bunny_hop {
		inline int category = Category::MOVEMENT;
		inline bool isEnabled = false;
		void bunny_hop(c_usercmd* cmd);
	}
	namespace auto_strafe {
		inline int category = Category::MOVEMENT;
		inline bool isEnabled = false;
		void auto_strafe(c_usercmd* cmd);
	}
};
namespace misc {


};
namespace combat {
	namespace antiaim {
		inline int category = Category::COMBAT;
		inline bool isEnabled = false;
		void run(c_usercmd* cmd, bool& bSendPacket);
		void handle_yaw(c_usercmd* cmd, bool fake);
		void handle_pitch(c_usercmd* cmd);

		inline bool desync = true;
		inline bool backwards = true;
		inline bool spinbot = false;
		inline int spinspeed = 10;
		inline bool jitter = true;
		inline int jitter_amount = 5;
		inline int yaw = 0;
		inline int pitch = 90;
	}

	namespace triggerbot{
		inline int category = Category::COMBAT;
		inline bool enabled = false;
		inline int delay = 50;
		void triggerbot(c_usercmd* cmd);
	}
	namespace aimbot {
		inline int category = Category::COMBAT;
		inline bool isEnabled = false;
		void aim_bot(c_usercmd* cmd);
	};
	namespace ragebot {
		inline bool isEnabled = false;
		inline bool auto_fire = true;
		inline bool silent = true;
		inline bool teammates = false;
		inline bool quickstop = true;
		inline int hit_chance = 90; /* min:1, max:100 */
		inline int min_dmg = 70; /*min:1, max:100*/
		inline bool dynamic_hitbox = true;
		inline bool head = true;
		inline bool pelvis = true;
		inline bool arms = false;
		inline bool legs = false;

		void onEnable();
	}
};