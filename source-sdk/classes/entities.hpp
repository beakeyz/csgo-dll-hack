#pragma once
#include "../../dependencies/math/math.hpp"
#include <array>
#include "collideable.hpp"
#include "client_class.hpp"
#include "../structs/animstate.hpp"
#include "../../dependencies/utilities/netvars/netvars.hpp"

enum data_update_type_t {
	DATA_UPDATE_CREATED = 0,
	DATA_UPDATE_DATATABLE_CHANGED,
};

enum cs_weapon_type {
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_PLACEHOLDER,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN
};

enum client_frame_stage_t {
	FRAME_UNDEFINED = -1,			// (haven't run any frames yet)
	FRAME_START,

	// A network packet is being recieved
	FRAME_NET_UPDATE_START,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END,

	// We're about to start rendering the scene
	FRAME_RENDER_START,
	// We've finished rendering the scene.
	FRAME_RENDER_END
};

enum move_type {
	movetype_none = 0,
	movetype_isometric,
	movetype_walk,
	movetype_step,
	movetype_fly,
	movetype_flygravity,
	movetype_vphysics,
	movetype_push,
	movetype_noclip,
	movetype_ladder,
	movetype_observer,
	movetype_custom,
	movetype_last = movetype_custom,
	movetype_max_bits = 4,
	max_movetype
};

enum entity_flags {
	fl_onground = (1 << 0),
	fl_ducking = (1 << 1),
	fl_waterjump = (1 << 2),
	fl_ontrain = (1 << 3),
	fl_inrain = (1 << 4),
	fl_frozen = (1 << 5),
	fl_atcontrols = (1 << 6),
	fl_client = (1 << 7),
	fl_fakeclient = (1 << 8),
	fl_inwater = (1 << 9),
	fl_fly = (1 << 10),
	fl_swim = (1 << 11),
	fl_conveyor = (1 << 12),
	fl_npc = (1 << 13),
	fl_godmode = (1 << 14),
	fl_notarget = (1 << 15),
	fl_aimtarget = (1 << 16),
	fl_partialground = (1 << 17),
	fl_staticprop = (1 << 18),
	fl_graphed = (1 << 19),
	fl_grenade = (1 << 20),
	fl_stepmovement = (1 << 21),
	fl_donttouch = (1 << 22),
	fl_basevelocity = (1 << 23),
	fl_worldbrush = (1 << 24),
	fl_object = (1 << 25),
	fl_killme = (1 << 26),
	fl_onfire = (1 << 27),
	fl_dissolving = (1 << 28),
	fl_transragdoll = (1 << 29),
	fl_unblockable_by_player = (1 << 30)
};
enum item_definition_indexes {
	WEAPON_NONE = 0,
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SHIELD,
	WEAPON_SCAR20,
	WEAPON_SG556,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL,
	WEAPON_BUMPMINE,
	WEAPON_BAYONET = 500,
	WEAPON_KNIFE_CSS = 503,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_HYDRA = 5035
};

class entity_t {
public:
	void set_m_bUseCustomBloomScale(byte value)
	{
		*reinterpret_cast<byte*>(uintptr_t(this) + (int)netvar_manager::get_net_var(fnv::hash("CEnvTonemapController"), fnv::hash("m_bUseCustomBloomScale"))) = value;
	}

	void set_m_flCustomBloomScale(float value)
	{
		*reinterpret_cast<float*>(uintptr_t(this) + (int)netvar_manager::get_net_var(fnv::hash("CEnvTonemapController"), fnv::hash("m_flCustomBloomScale"))) = value;
	}

	void set_m_bUseCustomAutoExposureMin(byte value)
	{
		*reinterpret_cast<byte*>(uintptr_t(this) + (int)netvar_manager::get_net_var(fnv::hash("CEnvTonemapController"), fnv::hash("m_bUseCustomAutoExposureMin"))) = value;
	}

	void set_m_flCustomAutoExposureMin(float value)
	{
		*reinterpret_cast<float*>(uintptr_t(this) + (int)netvar_manager::get_net_var(fnv::hash("CEnvTonemapController"), fnv::hash("m_flCustomAutoExposureMin"))) = value;
	}

	void set_m_bUseCustomAutoExposureMax(byte value)
	{
		*reinterpret_cast<byte*>(uintptr_t(this) + (int)netvar_manager::get_net_var(fnv::hash("CEnvTonemapController"), fnv::hash("m_bUseCustomAutoExposureMax"))) = value;
	}

	void set_m_flCustomAutoExposureMax(float value)
	{
		*reinterpret_cast<float*>(uintptr_t(this) + (int)netvar_manager::get_net_var(fnv::hash("CEnvTonemapController"), fnv::hash("m_flCustomAutoExposureMax"))) = value;
	}

    matrix_t& coord_frame() {
        const static auto m_CollisionGroup = netvar_manager::get_net_var(fnv::hash("DT_BaseEntity"), fnv::hash("m_CollisionGroup"));

        auto m_rgflCoordinateFrame = m_CollisionGroup - 0x30;

        return *reinterpret_cast<matrix_t*>(reinterpret_cast<uintptr_t>(this) + m_rgflCoordinateFrame);
    }
	
	void* animating() {
		return reinterpret_cast<void*>(uintptr_t(this) + 0x4);
	}
	void* networkable() {
		return reinterpret_cast<void*>(uintptr_t(this) + 0x8);
	}
	collideable_t* collideable() {
		using original_fn = collideable_t * (__thiscall*)(void*);
		return (*(original_fn**)this)[3](this);
	}
	c_client_class* client_class() {
		using original_fn = c_client_class * (__thiscall*)(void*);
		return (*(original_fn**)networkable())[2](networkable());
	}

	int index() {
		using original_fn = int(__thiscall*)(void*);
		return (*(original_fn**)networkable())[10](networkable());
	}
	bool is_player() {
		using original_fn = bool(__thiscall*)(entity_t*);
		return (*(original_fn**)this)[158](this);
	}
	bool is_weapon() {
		using original_fn = bool(__thiscall*)(entity_t*);
		return (*(original_fn**)this)[166](this);
	}
	bool setup_bones(matrix_t* out, int max_bones, int mask, float time) {
		if (!this)
			return false;

		using original_fn = bool(__thiscall*)(void*, matrix_t*, int, int, float);
		return (*(original_fn**)animating())[13](animating(), out, max_bones, mask, time);
	}

	vec3_t getBonePos(int i) {
		matrix_t out[256];
		if (setup_bones(out, 256, 256, 0)) {
			return out[i].getOrigin();
		}
		else {
			return { 0, 0, 0 };
		}
	}
	model_t* model() {
		using original_fn = model_t * (__thiscall*)(void*);
		return (*(original_fn**)animating())[8](animating());
	}
	void update() {
		using original_fn = void(__thiscall*)(entity_t*);
		(*(original_fn**)this)[218](this);
	}
	int draw_model(int flags, uint8_t alpha) {
		using original_fn = int(__thiscall*)(void*, int, uint8_t);
		return (*(original_fn**)animating())[9](animating(), flags, alpha);
	}
	void set_angles(vec3_t angles) {
		using original_fn = void(__thiscall*)(void*, const vec3_t&);
		static original_fn set_angles_fn = (original_fn)((DWORD)utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));
		set_angles_fn(this, angles);
	}
	void set_position(vec3_t position) {
		using original_fn = void(__thiscall*)(void*, const vec3_t&);
		static original_fn set_position_fn = (original_fn)((DWORD)utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"));
		set_position_fn(this, position);
	}

	void set_model_index(int index) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)this)[75](this, index);
	}

	void net_pre_data_update(int update_type) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)networkable())[6](networkable(), update_type);
	}

	void net_release() {
		using original_fn = void(__thiscall*)(void*);
		return (*(original_fn**)networkable())[1](networkable());
	}

	int net_set_destroyed_on_recreate_entities() {
		using original_fn = int(__thiscall*)(void*);
		return (*(original_fn**)networkable())[13](networkable());
	}

	bool dormant() {
		using original_fn = bool(__thiscall*)(void*);
		return (*static_cast<original_fn**>(networkable()))[9](networkable());
	}

	vec3_t get_eye_pos() {
		return origin() + view_offset();
	}

    NETVAR("DT_CSPlayer", "m_fFlags", flags, int)
    NETVAR("DT_BaseEntity", "m_hOwnerEntity", owner_handle, unsigned long)
    NETVAR("DT_CSPlayer", "m_flSimulationTime", simulation_time, float)
    NETVAR("DT_BasePlayer", "m_vecOrigin", origin, vec3_t)
    NETVAR("DT_BasePlayer", "m_vecViewOffset[0]", view_offset, vec3_t)
    NETVAR("DT_CSPlayer", "m_iTeamNum", team, int)
    NETVAR("DT_BaseEntity", "m_bSpotted", spotted, bool)
    NETVAR("DT_CSPlayer", "m_nSurvivalTeam", survival_team, int)
    NETVAR("DT_CSPlayer", "m_flHealthShotBoostExpirationTime", health_boost_time, float)
    NETVAR("DT_CSPlayer", "m_dwBoneMatrix", bone_matrix, uintptr_t*)
    NETVAR("DT_CSPlayer", "m_iHealth", health, int)
    NETVAR("DT_CSPlayer", "m_iGlowIndex", glowIndex, int)
    NETVAR("DT_BaseEntity", "m_vecMins", mins, vec3_t)
    NETVAR("DT_BaseEntity", "m_vecMaxs", maxs, vec3_t)
    NETVAR("DT_BaseEntity", "m_CollisionGroup", m_CollisionGroup, matrix3x4_t)

};

class econ_view_item_t {
public:
	NETVAR("DT_ScriptCreatedItem", "m_bInitialized", is_initialized, bool)
		NETVAR("DT_ScriptCreatedItem", "m_iEntityLevel", entity_level, int)
		NETVAR("DT_ScriptCreatedItem", "m_iAccountID", account_id, int)
		NETVAR("DT_ScriptCreatedItem", "m_iItemIDLow", item_id_low, int)
};

class base_view_model_t : public entity_t {
public:
	NETVAR("DT_BaseViewModel", "m_nModelIndex", model_index, int)
		NETVAR("DT_BaseViewModel", "m_nViewModelIndex", view_model_index, int)
		NETVAR("DT_BaseViewModel", "m_hWeapon", weapon, int)
		NETVAR("DT_BaseViewModel", "m_hOwner", owner, int)
};

class weapon_t : public entity_t {
public:
	NETVAR("DT_BaseCombatWeapon", "m_flNextPrimaryAttack", next_primary_attack, float)
		NETVAR("DT_BaseCombatWeapon", "m_flNextSecondaryAttack", next_secondary_attack, float)
		NETVAR("DT_BaseCombatWeapon", "m_iClip1", clip1_count, int)
		NETVAR("DT_BaseCombatWeapon", "m_iClip2", clip2_count, int)
		NETVAR("DT_BaseCombatWeapon", "m_iPrimaryReserveAmmoCount", primary_reserve_ammo_acount, int)
		NETVAR("DT_WeaponCSBase", "m_flRecoilIndex", recoil_index, float)
		NETVAR("DT_WeaponCSBaseGun", "m_zoomLevel", zoom_level, float)
		NETVAR("DT_BaseAttributableItem", "m_iItemDefinitionIndex", item_definition_index, short)
		NETVAR("DT_BaseCombatWeapon", "m_iEntityQuality", entity_quality, int)

		float inaccuracy() {
		using original_fn = float(__thiscall*)(void*);
		return (*(original_fn**)this)[483](this);
	}

	float get_spread() {
		using original_fn = float(__thiscall*)(void*);
		return (*(original_fn**)this)[453](this);
	}

	void update_accuracy_penalty() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[484](this);
	}

	weapon_info_t* get_weapon_data() {
		using original_fn = weapon_info_t*(__thiscall*)(void*);
		return (*(original_fn**)this)[461](this);
		//return interfaces::weapon_system->get_weapon_data(this->item_definition_index());
	}
};

//class AnimationLayer;

class player_t : public entity_t {
private:
	template <typename T>
	T& read(uintptr_t offset) {
		return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

	template <typename T>
	void write(uintptr_t offset, T data) {
		*reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset) = data;
	}

public:
	NETVAR("DT_BasePlayer", "m_hViewModel[0]", view_model, int)
	NETVAR("DT_CSPlayer", "m_bHasDefuser", has_defuser, bool)
	NETVAR("DT_CSPlayer", "m_bGunGameImmunity", has_gun_game_immunity, bool)
	NETVAR("DT_CSPlayer", "m_iShotsFired", shots_fired, int)
	NETVAR("DT_CSPlayer", "m_angEyeAngles", eye_angles, vec3_t)
	NETVAR("DT_CSPlayer", "m_ArmorValue", armor, int)
	NETVAR("DT_CSPlayer", "m_bHasHelmet", has_helmet, bool)
	NETVAR("DT_CSPlayer", "m_bIsScoped", is_scoped, bool)
	NETVAR("DT_CSPlayer", "m_bIsDefusing", is_defusing, bool)
	NETVAR("DT_CSPlayer", "m_iAccount", money, int)
	NETVAR("DT_CSPlayer", "m_flLowerBodyYawTarget", lower_body_yaw, float)
	NETVAR("DT_CSPlayer", "m_flNextAttack", next_attack, float)
	NETVAR("DT_CSPlayer", "m_flFlashDuration", flash_duration, float)
	NETVAR("DT_CSPlayer", "m_flFlashMaxAlpha", flash_alpha, float)
	NETVAR("DT_CSPlayer", "m_bHasNightVision", has_night_vision, bool)
	NETVAR("DT_CSPlayer", "m_bNightVisionOn", night_vision_on, bool)
	NETVAR("DT_CSPlayer", "m_iHealth", health, int)
	NETVAR("DT_CSPlayer", "m_lifeState", life_state, int)
	NETVAR("DT_CSPlayer", "m_fFlags", flags, int)
	NETVAR("DT_BasePlayer", "m_viewPunchAngle", punch_angle, vec3_t)
	NETVAR("DT_BasePlayer", "m_aimPunchAngle", aim_punch_angle, vec3_t)
	NETVAR("DT_BasePlayer", "m_vecVelocity[0]", velocity, vec3_t)
	NETVAR("DT_BasePlayer", "m_flMaxspeed", max_speed, float)
	NETVAR("DT_BaseEntity", "m_flShadowCastDistance", fov_time, float)
	NETVAR("DT_BasePlayer", "m_hObserverTarget", observer_target, unsigned long)
	NETVAR("DT_BasePlayer", "m_nHitboxSet", hitbox_set, int)
	NETVAR("DT_CSPlayer", "m_flDuckAmount", duck_amount, float)
	NETVAR("DT_CSPlayer", "m_bHasHeavyArmor", has_heavy_armor, bool)
	NETVAR("DT_SmokeGrenadeProjectile", "m_nSmokeEffectTickBegin", smoke_grenade_tick_begin, int)
	NETVAR("DT_CSPlayer", "m_nTickBase", get_tick_base, int)

	weapon_t* active_weapon() {
		auto active_weapon = read<uintptr_t>(netvar_manager::get_net_var(fnv::hash("DT_CSPlayer"), fnv::hash("m_hActiveWeapon"))) & 0xFFF;
		return reinterpret_cast<weapon_t*>(interfaces::entity_list->get_client_entity(active_weapon));
	}

	UINT* get_wearables() {
		return (UINT*)((uintptr_t)this + (netvar_manager::get_net_var(fnv::hash("DT_CSPlayer"), fnv::hash("m_hMyWearables"))));
	}

	UINT* get_weapons() {
		return (UINT*)((uintptr_t)this + (netvar_manager::get_net_var(fnv::hash("DT_CSPlayer"), fnv::hash("m_hMyWeapons"))));
	}

	vec3_t get_eye_pos() {
		return origin() + view_offset();
	}

	anim_state* get_anim_state() {
		return *reinterpret_cast<anim_state**>(this + 0x3914);
	}

	bool can_see_player_pos(player_t* player, const vec3_t& pos) {
		trace_t tr;
		ray_t ray;
		trace_filter filter;
		filter.skip = this;

		auto start = get_eye_pos();
		auto dir = (pos - start).normalized();

		ray.initialize(start, pos);
		interfaces::trace_ray->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

		return tr.entity == player || tr.flFraction > 0.97f;
	}

	vec3_t get_bone_position(int bone) {
		matrix_t bone_matrices[128];
		if (setup_bones(bone_matrices, 128, 256, 0.0f))
			return vec3_t{ bone_matrices[bone][0][3], bone_matrices[bone][1][3], bone_matrices[bone][2][3] };
		else
			return vec3_t{ };
	}

	vec3_t get_hitbox_position(int hitbox_id) {
		matrix_t bone_matrix[MAXSTUDIOBONES];

		if (setup_bones(bone_matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
			auto studio_model = interfaces::model_info->get_studio_model(model());

			if (studio_model) {
				auto hitbox = studio_model->hitbox_set(0)->hitbox(hitbox_id);

				if (hitbox) {
					auto min = vec3_t{}, max = vec3_t{};

					math::transform_vector(hitbox->mins, bone_matrix[hitbox->bone], min);
					math::transform_vector(hitbox->maxs, bone_matrix[hitbox->bone], max);

					return vec3_t((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f, (min.z + max.z) * 0.5f);
				}
			}
		}
		return vec3_t{};
	}

	bool is_alive() {
		if (!this) return false;
		return this->health() > 0;
	}

	bool is_moving() {
		if (!this) return false;
		return this->velocity().length() > 0.1f;
	}

	bool is_in_air() {
		if (!this) return false;
		return !(this->flags() & fl_onground);
	}

	bool is_flashed() {
		if (!this) return false;
		return this->flash_duration() > 0.0f;
	}

	void update_client_side_animations() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[224](this);
	}

	vec3_t& abs_origin() {
		using original_fn = vec3_t & (__thiscall*)(void*);
		return (*(original_fn**)this)[10](this);;
	}
	vec3_t& abs_angles() {
		using original_fn = vec3_t & (__thiscall*)(void*);
		return (*(original_fn**)this)[11](this);;
	}

	int move_type() {
		static int type = netvar_manager::get_net_var(fnv::hash("DT_BaseEntity"), fnv::hash("m_nRenderMode")) + 1;
		return read<int>(type);
	}
};

/*
class AnimationLayer
{
public:
    bool m_bClientBlend;		 //0x0000
    float m_flBlendIn;			 //0x0004
    void* m_pStudioHdr;			 //0x0008
    int m_nDispatchSequence;     //0x000C
    int m_nDispatchSequence_2;   //0x0010
    uint32_t m_nOrder;           //0x0014
    uint32_t m_nSequence;        //0x0018
    float_t m_flPrevCycle;       //0x001C
    float_t m_flWeight;          //0x0020
    float_t m_flWeightDeltaRate; //0x0024
    float_t m_flPlaybackRate;    //0x0028
    float_t m_flCycle;           //0x002C
    void* m_pOwner;              //0x0030
    char pad_0038[4];            //0x0034
};

class c_baseplayeranimationstate
{
public:
    char pad[3];
    char bUnknown; //0x4
    char pad2[87];
    weapon_t* m_pLastBoneSetupWeapon; //0x5C
    player_t* m_pBaseEntity; //0x60
    weapon_t* m_pActiveWeapon; //0x64
    weapon_t* m_pLastActiveWeapon; //0x68
    float m_flLastClientSideAnimationUpdateTime; //0x6C
    int m_iLastClientSideAnimationUpdateFramecount; //0x70
    float m_flUpdateTimeDelta; //0x74
    float m_flEyeYaw; //0x78
    float m_flPitch; //0x7C
    float m_flGoalFeetYaw; //0x80
    float m_flCurrentFeetYaw; //0x84
    float m_flCurrentTorsoYaw; //0x88
    float m_flUnknownVelocityLean; //0x8C //changes when moving/jumping/hitting ground
    float m_flLeanAmount; //0x90
    char pad4[4]; //NaN
    float m_flFeetCycle; //0x98 0 to 1
    float m_flFeetYawRate; //0x9C 0 to 1
    float m_fUnknown2;
    float m_fDuckAmount; //0xA4
    float m_fLandingDuckAdditiveSomething; //0xA8
    float m_fUnknown3; //0xAC
    vec3_t m_vOrigin; //0xB0, 0xB4, 0xB8
    vec3_t m_vLastOrigin; //0xBC, 0xC0, 0xC4
    float m_vVelocityX; //0xC8
    float m_vVelocityY; //0xCC
    char pad5[4];
    float m_flUnknownFloat1; //0xD4 Affected by movement and direction
    char pad6[8];
    float m_flUnknownFloat2; //0xE0 //from -1 to 1 when moving and affected by direction
    float m_flUnknownFloat3; //0xE4 //from -1 to 1 when moving and affected by direction
    float m_unknown; //0xE8
    float m_velocity; //0xEC
    float flUpVelocity; //0xF0
    float m_flSpeedNormalized; //0xF4 //from 0 to 1
    float m_flFeetSpeedForwardsOrSideWays; //0xF8 //from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
    float m_flFeetSpeedUnknownForwardOrSideways; //0xFC //from 0 to 3. something
    float m_flTimeSinceStartedMoving; //0x100
    float m_flTimeSinceStoppedMoving; //0x104
    bool m_bOnGround; //0x108
    bool m_bInHitGroundAnimation; //0x109
    char pad7[10];
    float m_flLastOriginZ; //0x114
    float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x118 from 0 to 1, is 1 when standing
    float m_flStopToFullRunningFraction; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
    char pad8[4]; //NaN
    float m_flMovingFraction; //0x124 affected while jumping and running, or when just jumping, 0 to 1
    char pad9[4]; //NaN
    float m_flUnknown3;
    char pad10[528];

    float& time_since_in_air()
    {
        return *(float*)((uintptr_t)this + 0x110);
    }

    float& yaw_desync_adjustment()
    {
        return *(float*)((uintptr_t)this + 0x334);
    }
};

enum Activity
{
    ACT_RESET,
    ACT_IDLE,
    ACT_TRANSITION,
    ACT_COVER,
    ACT_COVER_MED,
    ACT_COVER_LOW,
    ACT_WALK,
    ACT_WALK_AIM,
    ACT_WALK_CROUCH,
    ACT_WALK_CROUCH_AIM,
    ACT_RUN,
    ACT_RUN_AIM,
    ACT_RUN_CROUCH,
    ACT_RUN_CROUCH_AIM,
    ACT_RUN_PROTECTED,
    ACT_SCRIPT_CUSTOM_MOVE,
    ACT_RANGE_ATTACK1,
    ACT_RANGE_ATTACK2,
    ACT_RANGE_ATTACK1_LOW,
    ACT_RANGE_ATTACK2_LOW,
    ACT_DIESIMPLE,
    ACT_DIEBACKWARD,
    ACT_DIEFORWARD,
    ACT_DIEVIOLENT,
    ACT_DIERAGDOLL,
    ACT_FLY,
    ACT_HOVER,
    ACT_GLIDE,
    ACT_SWIM,
    ACT_JUMP,
    ACT_HOP,
    ACT_LEAP,
    ACT_LAND,
    ACT_CLIMB_UP,
    ACT_CLIMB_DOWN,
    ACT_CLIMB_DISMOUNT,
    ACT_SHIPLADDER_UP,
    ACT_SHIPLADDER_DOWN,
    ACT_STRAFE_LEFT,
    ACT_STRAFE_RIGHT,
    ACT_ROLL_LEFT,
    ACT_ROLL_RIGHT,
    ACT_TURN_LEFT,
    ACT_TURN_RIGHT,
    ACT_CROUCH,
    ACT_CROUCHIDLE,
    ACT_STAND,
    ACT_USE,
    ACT_ALIEN_BURROW_IDLE,
    ACT_ALIEN_BURROW_OUT,
    ACT_SIGNAL1,
    ACT_SIGNAL2,
    ACT_SIGNAL3,
    ACT_SIGNAL_ADVANCE,
    ACT_SIGNAL_FORWARD,
    ACT_SIGNAL_GROUP,
    ACT_SIGNAL_HALT,
    ACT_SIGNAL_LEFT,
    ACT_SIGNAL_RIGHT,
    ACT_SIGNAL_TAKECOVER,
    ACT_LOOKBACK_RIGHT,
    ACT_LOOKBACK_LEFT,
    ACT_COWER,
    ACT_SMALL_FLINCH,
    ACT_BIG_FLINCH,
    ACT_MELEE_ATTACK1,
    ACT_MELEE_ATTACK2,
    ACT_RELOAD,
    ACT_RELOAD_START,
    ACT_RELOAD_FINISH,
    ACT_RELOAD_LOW,
    ACT_ARM,
    ACT_DISARM,
    ACT_DROP_WEAPON,
    ACT_DROP_WEAPON_SHOTGUN,
    ACT_PICKUP_GROUND,
    ACT_PICKUP_RACK,
    ACT_IDLE_ANGRY,
    ACT_IDLE_RELAXED,
    ACT_IDLE_STIMULATED,
    ACT_IDLE_AGITATED,
    ACT_IDLE_STEALTH,
    ACT_IDLE_HURT,
    ACT_WALK_RELAXED,
    ACT_WALK_STIMULATED,
    ACT_WALK_AGITATED,
    ACT_WALK_STEALTH,
    ACT_RUN_RELAXED,
    ACT_RUN_STIMULATED,
    ACT_RUN_AGITATED,
    ACT_RUN_STEALTH,
    ACT_IDLE_AIM_RELAXED,
    ACT_IDLE_AIM_STIMULATED,
    ACT_IDLE_AIM_AGITATED,
    ACT_IDLE_AIM_STEALTH,
    ACT_WALK_AIM_RELAXED,
    ACT_WALK_AIM_STIMULATED,
    ACT_WALK_AIM_AGITATED,
    ACT_WALK_AIM_STEALTH,
    ACT_RUN_AIM_RELAXED,
    ACT_RUN_AIM_STIMULATED,
    ACT_RUN_AIM_AGITATED,
    ACT_RUN_AIM_STEALTH,
    ACT_CROUCHIDLE_STIMULATED,
    ACT_CROUCHIDLE_AIM_STIMULATED,
    ACT_CROUCHIDLE_AGITATED,
    ACT_WALK_HURT,
    ACT_RUN_HURT,
    ACT_SPECIAL_ATTACK1,
    ACT_SPECIAL_ATTACK2,
    ACT_COMBAT_IDLE,
    ACT_WALK_SCARED,
    ACT_RUN_SCARED,
    ACT_VICTORY_DANCE,
    ACT_DIE_HEADSHOT,
    ACT_DIE_CHESTSHOT,
    ACT_DIE_GUTSHOT,
    ACT_DIE_BACKSHOT,
    ACT_FLINCH_HEAD,
    ACT_FLINCH_CHEST,
    ACT_FLINCH_STOMACH,
    ACT_FLINCH_LEFTARM,
    ACT_FLINCH_RIGHTARM,
    ACT_FLINCH_LEFTLEG,
    ACT_FLINCH_RIGHTLEG,
    ACT_FLINCH_PHYSICS,
    ACT_FLINCH_HEAD_BACK,
    ACT_FLINCH_HEAD_LEFT,
    ACT_FLINCH_HEAD_RIGHT,
    ACT_FLINCH_CHEST_BACK,
    ACT_FLINCH_STOMACH_BACK,
    ACT_FLINCH_CROUCH_FRONT,
    ACT_FLINCH_CROUCH_BACK,
    ACT_FLINCH_CROUCH_LEFT,
    ACT_FLINCH_CROUCH_RIGHT,
    ACT_IDLE_ON_FIRE,
    ACT_WALK_ON_FIRE,
    ACT_RUN_ON_FIRE,
    ACT_RAPPEL_LOOP,
    ACT_180_LEFT,
    ACT_180_RIGHT,
    ACT_90_LEFT,
    ACT_90_RIGHT,
    ACT_STEP_LEFT,
    ACT_STEP_RIGHT,
    ACT_STEP_BACK,
    ACT_STEP_FORE,
    ACT_GESTURE_RANGE_ATTACK1,
    ACT_GESTURE_RANGE_ATTACK2,
    ACT_GESTURE_MELEE_ATTACK1,
    ACT_GESTURE_MELEE_ATTACK2,
    ACT_GESTURE_RANGE_ATTACK1_LOW,
    ACT_GESTURE_RANGE_ATTACK2_LOW,
    ACT_MELEE_ATTACK_SWING_GESTURE,
    ACT_GESTURE_SMALL_FLINCH,
    ACT_GESTURE_BIG_FLINCH,
    ACT_GESTURE_FLINCH_BLAST,
    ACT_GESTURE_FLINCH_BLAST_SHOTGUN,
    ACT_GESTURE_FLINCH_BLAST_DAMAGED,
    ACT_GESTURE_FLINCH_BLAST_DAMAGED_SHOTGUN,
    ACT_GESTURE_FLINCH_HEAD,
    ACT_GESTURE_FLINCH_CHEST,
    ACT_GESTURE_FLINCH_STOMACH,
    ACT_GESTURE_FLINCH_LEFTARM,
    ACT_GESTURE_FLINCH_RIGHTARM,
    ACT_GESTURE_FLINCH_LEFTLEG,
    ACT_GESTURE_FLINCH_RIGHTLEG,
    ACT_GESTURE_TURN_LEFT,
    ACT_GESTURE_TURN_RIGHT,
    ACT_GESTURE_TURN_LEFT45,
    ACT_GESTURE_TURN_RIGHT45,
    ACT_GESTURE_TURN_LEFT90,
    ACT_GESTURE_TURN_RIGHT90,
    ACT_GESTURE_TURN_LEFT45_FLAT,
    ACT_GESTURE_TURN_RIGHT45_FLAT,
    ACT_GESTURE_TURN_LEFT90_FLAT,
    ACT_GESTURE_TURN_RIGHT90_FLAT,
    ACT_BARNACLE_HIT,
    ACT_BARNACLE_PULL,
    ACT_BARNACLE_CHOMP,
    ACT_BARNACLE_CHEW,
    ACT_DO_NOT_DISTURB,
    ACT_SPECIFIC_SEQUENCE,
    ACT_VM_DRAW,
    ACT_VM_HOLSTER,
    ACT_VM_IDLE,
    ACT_VM_FIDGET,
    ACT_VM_PULLBACK,
    ACT_VM_PULLBACK_HIGH,
    ACT_VM_PULLBACK_LOW,
    ACT_VM_THROW,
    ACT_VM_PULLPIN,
    ACT_VM_PRIMARYATTACK,
    ACT_VM_SECONDARYATTACK,
    ACT_VM_RELOAD,
    ACT_VM_DRYFIRE,
    ACT_VM_HITLEFT,
    ACT_VM_HITLEFT2,
    ACT_VM_HITRIGHT,
    ACT_VM_HITRIGHT2,
    ACT_VM_HITCENTER,
    ACT_VM_HITCENTER2,
    ACT_VM_MISSLEFT,
    ACT_VM_MISSLEFT2,
    ACT_VM_MISSRIGHT,
    ACT_VM_MISSRIGHT2,
    ACT_VM_MISSCENTER,
    ACT_VM_MISSCENTER2,
    ACT_VM_HAULBACK,
    ACT_VM_SWINGHARD,
    ACT_VM_SWINGMISS,
    ACT_VM_SWINGHIT,
    ACT_VM_IDLE_TO_LOWERED,
    ACT_VM_IDLE_LOWERED,
    ACT_VM_LOWERED_TO_IDLE,
    ACT_VM_RECOIL1,
    ACT_VM_RECOIL2,
    ACT_VM_RECOIL3,
    ACT_VM_PICKUP,
    ACT_VM_RELEASE,
    ACT_VM_ATTACH_SILENCER,
    ACT_VM_DETACH_SILENCER,
    ACT_VM_EMPTY_FIRE,
    ACT_VM_EMPTY_RELOAD,
    ACT_VM_EMPTY_DRAW,
    ACT_VM_EMPTY_IDLE,
    ACT_SLAM_STICKWALL_IDLE,
    ACT_SLAM_STICKWALL_ND_IDLE,
    ACT_SLAM_STICKWALL_ATTACH,
    ACT_SLAM_STICKWALL_ATTACH2,
    ACT_SLAM_STICKWALL_ND_ATTACH,
    ACT_SLAM_STICKWALL_ND_ATTACH2,
    ACT_SLAM_STICKWALL_DETONATE,
    ACT_SLAM_STICKWALL_DETONATOR_HOLSTER,
    ACT_SLAM_STICKWALL_DRAW,
    ACT_SLAM_STICKWALL_ND_DRAW,
    ACT_SLAM_STICKWALL_TO_THROW,
    ACT_SLAM_STICKWALL_TO_THROW_ND,
    ACT_SLAM_STICKWALL_TO_TRIPMINE_ND,
    ACT_SLAM_THROW_IDLE,
    ACT_SLAM_THROW_ND_IDLE,
    ACT_SLAM_THROW_THROW,
    ACT_SLAM_THROW_THROW2,
    ACT_SLAM_THROW_THROW_ND,
    ACT_SLAM_THROW_THROW_ND2,
    ACT_SLAM_THROW_DRAW,
    ACT_SLAM_THROW_ND_DRAW,
    ACT_SLAM_THROW_TO_STICKWALL,
    ACT_SLAM_THROW_TO_STICKWALL_ND,
    ACT_SLAM_THROW_DETONATE,
    ACT_SLAM_THROW_DETONATOR_HOLSTER,
    ACT_SLAM_THROW_TO_TRIPMINE_ND,
    ACT_SLAM_TRIPMINE_IDLE,
    ACT_SLAM_TRIPMINE_DRAW,
    ACT_SLAM_TRIPMINE_ATTACH,
    ACT_SLAM_TRIPMINE_ATTACH2,
    ACT_SLAM_TRIPMINE_TO_STICKWALL_ND,
    ACT_SLAM_TRIPMINE_TO_THROW_ND,
    ACT_SLAM_DETONATOR_IDLE,
    ACT_SLAM_DETONATOR_DRAW,
    ACT_SLAM_DETONATOR_DETONATE,
    ACT_SLAM_DETONATOR_HOLSTER,
    ACT_SLAM_DETONATOR_STICKWALL_DRAW,
    ACT_SLAM_DETONATOR_THROW_DRAW,
    ACT_SHOTGUN_RELOAD_START,
    ACT_SHOTGUN_RELOAD_FINISH,
    ACT_SHOTGUN_PUMP,
    ACT_SMG2_IDLE2,
    ACT_SMG2_FIRE2,
    ACT_SMG2_DRAW2,
    ACT_SMG2_RELOAD2,
    ACT_SMG2_DRYFIRE2,
    ACT_SMG2_TOAUTO,
    ACT_SMG2_TOBURST,
    ACT_PHYSCANNON_UPGRADE,
    ACT_RANGE_ATTACK_AR1,
    ACT_RANGE_ATTACK_AR2,
    ACT_RANGE_ATTACK_AR2_LOW,
    ACT_RANGE_ATTACK_AR2_GRENADE,
    ACT_RANGE_ATTACK_HMG1,
    ACT_RANGE_ATTACK_ML,
    ACT_RANGE_ATTACK_SMG1,
    ACT_RANGE_ATTACK_SMG1_LOW,
    ACT_RANGE_ATTACK_SMG2,
    ACT_RANGE_ATTACK_SHOTGUN,
    ACT_RANGE_ATTACK_SHOTGUN_LOW,
    ACT_RANGE_ATTACK_PISTOL,
    ACT_RANGE_ATTACK_PISTOL_LOW,
    ACT_RANGE_ATTACK_SLAM,
    ACT_RANGE_ATTACK_TRIPWIRE,
    ACT_RANGE_ATTACK_THROW,
    ACT_RANGE_ATTACK_SNIPER_RIFLE,
    ACT_RANGE_ATTACK_RPG,
    ACT_MELEE_ATTACK_SWING,
    ACT_RANGE_AIM_LOW,
    ACT_RANGE_AIM_SMG1_LOW,
    ACT_RANGE_AIM_PISTOL_LOW,
    ACT_RANGE_AIM_AR2_LOW,
    ACT_COVER_PISTOL_LOW,
    ACT_COVER_SMG1_LOW,
    ACT_GESTURE_RANGE_ATTACK_AR1,
    ACT_GESTURE_RANGE_ATTACK_AR2,
    ACT_GESTURE_RANGE_ATTACK_AR2_GRENADE,
    ACT_GESTURE_RANGE_ATTACK_HMG1,
    ACT_GESTURE_RANGE_ATTACK_ML,
    ACT_GESTURE_RANGE_ATTACK_SMG1,
    ACT_GESTURE_RANGE_ATTACK_SMG1_LOW,
    ACT_GESTURE_RANGE_ATTACK_SMG2,
    ACT_GESTURE_RANGE_ATTACK_SHOTGUN,
    ACT_GESTURE_RANGE_ATTACK_PISTOL,
    ACT_GESTURE_RANGE_ATTACK_PISTOL_LOW,
    ACT_GESTURE_RANGE_ATTACK_SLAM,
    ACT_GESTURE_RANGE_ATTACK_TRIPWIRE,
    ACT_GESTURE_RANGE_ATTACK_THROW,
    ACT_GESTURE_RANGE_ATTACK_SNIPER_RIFLE,
    ACT_GESTURE_MELEE_ATTACK_SWING,
    ACT_IDLE_RIFLE,
    ACT_IDLE_SMG1,
    ACT_IDLE_ANGRY_SMG1,
    ACT_IDLE_PISTOL,
    ACT_IDLE_ANGRY_PISTOL,
    ACT_IDLE_ANGRY_SHOTGUN,
    ACT_IDLE_STEALTH_PISTOL,
    ACT_IDLE_PACKAGE,
    ACT_WALK_PACKAGE,
    ACT_IDLE_SUITCASE,
    ACT_WALK_SUITCASE,
    ACT_IDLE_SMG1_RELAXED,
    ACT_IDLE_SMG1_STIMULATED,
    ACT_WALK_RIFLE_RELAXED,
    ACT_RUN_RIFLE_RELAXED,
    ACT_WALK_RIFLE_STIMULATED,
    ACT_RUN_RIFLE_STIMULATED,
    ACT_IDLE_AIM_RIFLE_STIMULATED,
    ACT_WALK_AIM_RIFLE_STIMULATED,
    ACT_RUN_AIM_RIFLE_STIMULATED,
    ACT_IDLE_SHOTGUN_RELAXED,
    ACT_IDLE_SHOTGUN_STIMULATED,
    ACT_IDLE_SHOTGUN_AGITATED,
    ACT_WALK_ANGRY,
    ACT_POLICE_HARASS1,
    ACT_POLICE_HARASS2,
    ACT_IDLE_MANNEDGUN,
    ACT_IDLE_MELEE,
    ACT_IDLE_ANGRY_MELEE,
    ACT_IDLE_RPG_RELAXED,
    ACT_IDLE_RPG,
    ACT_IDLE_ANGRY_RPG,
    ACT_COVER_LOW_RPG,
    ACT_WALK_RPG,
    ACT_RUN_RPG,
    ACT_WALK_CROUCH_RPG,
    ACT_RUN_CROUCH_RPG,
    ACT_WALK_RPG_RELAXED,
    ACT_RUN_RPG_RELAXED,
    ACT_WALK_RIFLE,
    ACT_WALK_AIM_RIFLE,
    ACT_WALK_CROUCH_RIFLE,
    ACT_WALK_CROUCH_AIM_RIFLE,
    ACT_RUN_RIFLE,
    ACT_RUN_AIM_RIFLE,
    ACT_RUN_CROUCH_RIFLE,
    ACT_RUN_CROUCH_AIM_RIFLE,
    ACT_RUN_STEALTH_PISTOL,
    ACT_WALK_AIM_SHOTGUN,
    ACT_RUN_AIM_SHOTGUN,
    ACT_WALK_PISTOL,
    ACT_RUN_PISTOL,
    ACT_WALK_AIM_PISTOL,
    ACT_RUN_AIM_PISTOL,
    ACT_WALK_STEALTH_PISTOL,
    ACT_WALK_AIM_STEALTH_PISTOL,
    ACT_RUN_AIM_STEALTH_PISTOL,
    ACT_RELOAD_PISTOL,
    ACT_RELOAD_PISTOL_LOW,
    ACT_RELOAD_SMG1,
    ACT_RELOAD_SMG1_LOW,
    ACT_RELOAD_SHOTGUN,
    ACT_RELOAD_SHOTGUN_LOW,
    ACT_GESTURE_RELOAD,
    ACT_GESTURE_RELOAD_PISTOL,
    ACT_GESTURE_RELOAD_SMG1,
    ACT_GESTURE_RELOAD_SHOTGUN,
    ACT_BUSY_LEAN_LEFT,
    ACT_BUSY_LEAN_LEFT_ENTRY,
    ACT_BUSY_LEAN_LEFT_EXIT,
    ACT_BUSY_LEAN_BACK,
    ACT_BUSY_LEAN_BACK_ENTRY,
    ACT_BUSY_LEAN_BACK_EXIT,
    ACT_BUSY_SIT_GROUND,
    ACT_BUSY_SIT_GROUND_ENTRY,
    ACT_BUSY_SIT_GROUND_EXIT,
    ACT_BUSY_SIT_CHAIR,
    ACT_BUSY_SIT_CHAIR_ENTRY,
    ACT_BUSY_SIT_CHAIR_EXIT,
    ACT_BUSY_STAND,
    ACT_BUSY_QUEUE,
    ACT_DUCK_DODGE,
    ACT_DIE_BARNACLE_SWALLOW,
    ACT_GESTURE_BARNACLE_STRANGLE,
    ACT_PHYSCANNON_DETACH,
    ACT_PHYSCANNON_ANIMATE,
    ACT_PHYSCANNON_ANIMATE_PRE,
    ACT_PHYSCANNON_ANIMATE_POST,
    ACT_DIE_FRONTSIDE,
    ACT_DIE_RIGHTSIDE,
    ACT_DIE_BACKSIDE,
    ACT_DIE_LEFTSIDE,
    ACT_DIE_CROUCH_FRONTSIDE,
    ACT_DIE_CROUCH_RIGHTSIDE,
    ACT_DIE_CROUCH_BACKSIDE,
    ACT_DIE_CROUCH_LEFTSIDE,
    ACT_OPEN_DOOR,
    ACT_DI_ALYX_ZOMBIE_MELEE,
    ACT_DI_ALYX_ZOMBIE_TORSO_MELEE,
    ACT_DI_ALYX_HEADCRAB_MELEE,
    ACT_DI_ALYX_ANTLION,
    ACT_DI_ALYX_ZOMBIE_SHOTGUN64,
    ACT_DI_ALYX_ZOMBIE_SHOTGUN26,
    ACT_READINESS_RELAXED_TO_STIMULATED,
    ACT_READINESS_RELAXED_TO_STIMULATED_WALK,
    ACT_READINESS_AGITATED_TO_STIMULATED,
    ACT_READINESS_STIMULATED_TO_RELAXED,
    ACT_READINESS_PISTOL_RELAXED_TO_STIMULATED,
    ACT_READINESS_PISTOL_RELAXED_TO_STIMULATED_WALK,
    ACT_READINESS_PISTOL_AGITATED_TO_STIMULATED,
    ACT_READINESS_PISTOL_STIMULATED_TO_RELAXED,
    ACT_IDLE_CARRY,
    ACT_WALK_CARRY,
    ACT_STARTDYING,
    ACT_DYINGLOOP,
    ACT_DYINGTODEAD,
    ACT_RIDE_MANNED_GUN,
    ACT_VM_SPRINT_ENTER,
    ACT_VM_SPRINT_IDLE,
    ACT_VM_SPRINT_LEAVE,
    ACT_FIRE_START,
    ACT_FIRE_LOOP,
    ACT_FIRE_END,
    ACT_CROUCHING_GRENADEIDLE,
    ACT_CROUCHING_GRENADEREADY,
    ACT_CROUCHING_PRIMARYATTACK,
    ACT_OVERLAY_GRENADEIDLE,
    ACT_OVERLAY_GRENADEREADY,
    ACT_OVERLAY_PRIMARYATTACK,
    ACT_OVERLAY_SHIELD_UP,
    ACT_OVERLAY_SHIELD_DOWN,
    ACT_OVERLAY_SHIELD_UP_IDLE,
    ACT_OVERLAY_SHIELD_ATTACK,
    ACT_OVERLAY_SHIELD_KNOCKBACK,
    ACT_SHIELD_UP,
    ACT_SHIELD_DOWN,
    ACT_SHIELD_UP_IDLE,
    ACT_SHIELD_ATTACK,
    ACT_SHIELD_KNOCKBACK,
    ACT_CROUCHING_SHIELD_UP,
    ACT_CROUCHING_SHIELD_DOWN,
    ACT_CROUCHING_SHIELD_UP_IDLE,
    ACT_CROUCHING_SHIELD_ATTACK,
    ACT_CROUCHING_SHIELD_KNOCKBACK,
    ACT_TURNRIGHT45,
    ACT_TURNLEFT45,
    ACT_TURN,
    ACT_OBJ_ASSEMBLING,
    ACT_OBJ_DISMANTLING,
    ACT_OBJ_STARTUP,
    ACT_OBJ_RUNNING,
    ACT_OBJ_IDLE,
    ACT_OBJ_PLACING,
    ACT_OBJ_DETERIORATING,
    ACT_OBJ_UPGRADING,
    ACT_DEPLOY,
    ACT_DEPLOY_IDLE,
    ACT_UNDEPLOY,
    ACT_CROSSBOW_DRAW_UNLOADED,
    ACT_GAUSS_SPINUP,
    ACT_GAUSS_SPINCYCLE,
    ACT_VM_PRIMARYATTACK_SILENCED,
    ACT_VM_RELOAD_SILENCED,
    ACT_VM_DRYFIRE_SILENCED,
    ACT_VM_IDLE_SILENCED,
    ACT_VM_DRAW_SILENCED,
    ACT_VM_IDLE_EMPTY_LEFT,
    ACT_VM_DRYFIRE_LEFT,
    ACT_VM_IS_DRAW,
    ACT_VM_IS_HOLSTER,
    ACT_VM_IS_IDLE,
    ACT_VM_IS_PRIMARYATTACK,
    ACT_PLAYER_IDLE_FIRE,
    ACT_PLAYER_CROUCH_FIRE,
    ACT_PLAYER_CROUCH_WALK_FIRE,
    ACT_PLAYER_WALK_FIRE,
    ACT_PLAYER_RUN_FIRE,
    ACT_IDLETORUN,
    ACT_RUNTOIDLE,
    ACT_VM_DRAW_DEPLOYED,
    ACT_HL2MP_IDLE_MELEE,
    ACT_HL2MP_RUN_MELEE,
    ACT_HL2MP_IDLE_CROUCH_MELEE,
    ACT_HL2MP_WALK_CROUCH_MELEE,
    ACT_HL2MP_GESTURE_RANGE_ATTACK_MELEE,
    ACT_HL2MP_GESTURE_RELOAD_MELEE,
    ACT_HL2MP_JUMP_MELEE,
    ACT_VM_FIZZLE,
    ACT_MP_STAND_IDLE,
    ACT_MP_CROUCH_IDLE,
    ACT_MP_CROUCH_DEPLOYED_IDLE,
    ACT_MP_CROUCH_DEPLOYED,
    ACT_MP_DEPLOYED_IDLE,
    ACT_MP_RUN,
    ACT_MP_WALK,
    ACT_MP_AIRWALK,
    ACT_MP_CROUCHWALK,
    ACT_MP_SPRINT,
    ACT_MP_JUMP,
    ACT_MP_JUMP_START,
    ACT_MP_JUMP_FLOAT,
    ACT_MP_JUMP_LAND,
    ACT_MP_JUMP_IMPACT_N,
    ACT_MP_JUMP_IMPACT_E,
    ACT_MP_JUMP_IMPACT_W,
    ACT_MP_JUMP_IMPACT_S,
    ACT_MP_JUMP_IMPACT_TOP,
    ACT_MP_DOUBLEJUMP,
    ACT_MP_SWIM,
    ACT_MP_DEPLOYED,
    ACT_MP_SWIM_DEPLOYED,
    ACT_MP_VCD,
    ACT_MP_ATTACK_STAND_PRIMARYFIRE,
    ACT_MP_ATTACK_STAND_PRIMARYFIRE_DEPLOYED,
    ACT_MP_ATTACK_STAND_SECONDARYFIRE,
    ACT_MP_ATTACK_STAND_GRENADE,
    ACT_MP_ATTACK_CROUCH_PRIMARYFIRE,
    ACT_MP_ATTACK_CROUCH_PRIMARYFIRE_DEPLOYED,
    ACT_MP_ATTACK_CROUCH_SECONDARYFIRE,
    ACT_MP_ATTACK_CROUCH_GRENADE,
    ACT_MP_ATTACK_SWIM_PRIMARYFIRE,
    ACT_MP_ATTACK_SWIM_SECONDARYFIRE,
    ACT_MP_ATTACK_SWIM_GRENADE,
    ACT_MP_ATTACK_AIRWALK_PRIMARYFIRE,
    ACT_MP_ATTACK_AIRWALK_SECONDARYFIRE,
    ACT_MP_ATTACK_AIRWALK_GRENADE,
    ACT_MP_RELOAD_STAND,
    ACT_MP_RELOAD_STAND_LOOP,
    ACT_MP_RELOAD_STAND_END,
    ACT_MP_RELOAD_CROUCH,
    ACT_MP_RELOAD_CROUCH_LOOP,
    ACT_MP_RELOAD_CROUCH_END,
    ACT_MP_RELOAD_SWIM,
    ACT_MP_RELOAD_SWIM_LOOP,
    ACT_MP_RELOAD_SWIM_END,
    ACT_MP_RELOAD_AIRWALK,
    ACT_MP_RELOAD_AIRWALK_LOOP,
    ACT_MP_RELOAD_AIRWALK_END,
    ACT_MP_ATTACK_STAND_PREFIRE,
    ACT_MP_ATTACK_STAND_POSTFIRE,
    ACT_MP_ATTACK_STAND_STARTFIRE,
    ACT_MP_ATTACK_CROUCH_PREFIRE,
    ACT_MP_ATTACK_CROUCH_POSTFIRE,
    ACT_MP_ATTACK_SWIM_PREFIRE,
    ACT_MP_ATTACK_SWIM_POSTFIRE,
    ACT_MP_STAND_PRIMARY,
    ACT_MP_CROUCH_PRIMARY,
    ACT_MP_RUN_PRIMARY,
    ACT_MP_WALK_PRIMARY,
    ACT_MP_AIRWALK_PRIMARY,
    ACT_MP_CROUCHWALK_PRIMARY,
    ACT_MP_JUMP_PRIMARY,
    ACT_MP_JUMP_START_PRIMARY,
    ACT_MP_JUMP_FLOAT_PRIMARY,
    ACT_MP_JUMP_LAND_PRIMARY,
    ACT_MP_SWIM_PRIMARY,
    ACT_MP_DEPLOYED_PRIMARY,
    ACT_MP_SWIM_DEPLOYED_PRIMARY,
    ACT_MP_ATTACK_STAND_PRIMARY,
    ACT_MP_ATTACK_STAND_PRIMARY_DEPLOYED,
    ACT_MP_ATTACK_CROUCH_PRIMARY,
    ACT_MP_ATTACK_CROUCH_PRIMARY_DEPLOYED,
    ACT_MP_ATTACK_SWIM_PRIMARY,
    ACT_MP_ATTACK_AIRWALK_PRIMARY,
    ACT_MP_RELOAD_STAND_PRIMARY,
    ACT_MP_RELOAD_STAND_PRIMARY_LOOP,
    ACT_MP_RELOAD_STAND_PRIMARY_END,
    ACT_MP_RELOAD_CROUCH_PRIMARY,
    ACT_MP_RELOAD_CROUCH_PRIMARY_LOOP,
    ACT_MP_RELOAD_CROUCH_PRIMARY_END,
    ACT_MP_RELOAD_SWIM_PRIMARY,
    ACT_MP_RELOAD_SWIM_PRIMARY_LOOP,
    ACT_MP_RELOAD_SWIM_PRIMARY_END,
    ACT_MP_RELOAD_AIRWALK_PRIMARY,
    ACT_MP_RELOAD_AIRWALK_PRIMARY_LOOP,
    ACT_MP_RELOAD_AIRWALK_PRIMARY_END,
    ACT_MP_ATTACK_STAND_GRENADE_PRIMARY,
    ACT_MP_ATTACK_CROUCH_GRENADE_PRIMARY,
    ACT_MP_ATTACK_SWIM_GRENADE_PRIMARY,
    ACT_MP_ATTACK_AIRWALK_GRENADE_PRIMARY,
    ACT_MP_STAND_SECONDARY,
    ACT_MP_CROUCH_SECONDARY,
    ACT_MP_RUN_SECONDARY,
    ACT_MP_WALK_SECONDARY,
    ACT_MP_AIRWALK_SECONDARY,
    ACT_MP_CROUCHWALK_SECONDARY,
    ACT_MP_JUMP_SECONDARY,
    ACT_MP_JUMP_START_SECONDARY,
    ACT_MP_JUMP_FLOAT_SECONDARY,
    ACT_MP_JUMP_LAND_SECONDARY,
    ACT_MP_SWIM_SECONDARY,
    ACT_MP_ATTACK_STAND_SECONDARY,
    ACT_MP_ATTACK_CROUCH_SECONDARY,
    ACT_MP_ATTACK_SWIM_SECONDARY,
    ACT_MP_ATTACK_AIRWALK_SECONDARY,
    ACT_MP_RELOAD_STAND_SECONDARY,
    ACT_MP_RELOAD_STAND_SECONDARY_LOOP,
    ACT_MP_RELOAD_STAND_SECONDARY_END,
    ACT_MP_RELOAD_CROUCH_SECONDARY,
    ACT_MP_RELOAD_CROUCH_SECONDARY_LOOP,
    ACT_MP_RELOAD_CROUCH_SECONDARY_END,
    ACT_MP_RELOAD_SWIM_SECONDARY,
    ACT_MP_RELOAD_SWIM_SECONDARY_LOOP,
    ACT_MP_RELOAD_SWIM_SECONDARY_END,
    ACT_MP_RELOAD_AIRWALK_SECONDARY,
    ACT_MP_RELOAD_AIRWALK_SECONDARY_LOOP,
    ACT_MP_RELOAD_AIRWALK_SECONDARY_END,
    ACT_MP_ATTACK_STAND_GRENADE_SECONDARY,
    ACT_MP_ATTACK_CROUCH_GRENADE_SECONDARY,
    ACT_MP_ATTACK_SWIM_GRENADE_SECONDARY,
    ACT_MP_ATTACK_AIRWALK_GRENADE_SECONDARY,
    ACT_MP_STAND_MELEE,
    ACT_MP_CROUCH_MELEE,
    ACT_MP_RUN_MELEE,
    ACT_MP_WALK_MELEE,
    ACT_MP_AIRWALK_MELEE,
    ACT_MP_CROUCHWALK_MELEE,
    ACT_MP_JUMP_MELEE,
    ACT_MP_JUMP_START_MELEE,
    ACT_MP_JUMP_FLOAT_MELEE,
    ACT_MP_JUMP_LAND_MELEE,
    ACT_MP_SWIM_MELEE,
    ACT_MP_ATTACK_STAND_MELEE,
    ACT_MP_ATTACK_STAND_MELEE_SECONDARY,
    ACT_MP_ATTACK_CROUCH_MELEE,
    ACT_MP_ATTACK_CROUCH_MELEE_SECONDARY,
    ACT_MP_ATTACK_SWIM_MELEE,
    ACT_MP_ATTACK_AIRWALK_MELEE,
    ACT_MP_ATTACK_STAND_GRENADE_MELEE,
    ACT_MP_ATTACK_CROUCH_GRENADE_MELEE,
    ACT_MP_ATTACK_SWIM_GRENADE_MELEE,
    ACT_MP_ATTACK_AIRWALK_GRENADE_MELEE,
    ACT_MP_STAND_ITEM1,
    ACT_MP_CROUCH_ITEM1,
    ACT_MP_RUN_ITEM1,
    ACT_MP_WALK_ITEM1,
    ACT_MP_AIRWALK_ITEM1,
    ACT_MP_CROUCHWALK_ITEM1,
    ACT_MP_JUMP_ITEM1,
    ACT_MP_JUMP_START_ITEM1,
    ACT_MP_JUMP_FLOAT_ITEM1,
    ACT_MP_JUMP_LAND_ITEM1,
    ACT_MP_SWIM_ITEM1,
    ACT_MP_ATTACK_STAND_ITEM1,
    ACT_MP_ATTACK_STAND_ITEM1_SECONDARY,
    ACT_MP_ATTACK_CROUCH_ITEM1,
    ACT_MP_ATTACK_CROUCH_ITEM1_SECONDARY,
    ACT_MP_ATTACK_SWIM_ITEM1,
    ACT_MP_ATTACK_AIRWALK_ITEM1,
    ACT_MP_STAND_ITEM2,
    ACT_MP_CROUCH_ITEM2,
    ACT_MP_RUN_ITEM2,
    ACT_MP_WALK_ITEM2,
    ACT_MP_AIRWALK_ITEM2,
    ACT_MP_CROUCHWALK_ITEM2,
    ACT_MP_JUMP_ITEM2,
    ACT_MP_JUMP_START_ITEM2,
    ACT_MP_JUMP_FLOAT_ITEM2,
    ACT_MP_JUMP_LAND_ITEM2,
    ACT_MP_SWIM_ITEM2,
    ACT_MP_ATTACK_STAND_ITEM2,
    ACT_MP_ATTACK_STAND_ITEM2_SECONDARY,
    ACT_MP_ATTACK_CROUCH_ITEM2,
    ACT_MP_ATTACK_CROUCH_ITEM2_SECONDARY,
    ACT_MP_ATTACK_SWIM_ITEM2,
    ACT_MP_ATTACK_AIRWALK_ITEM2,
    ACT_MP_GESTURE_FLINCH,
    ACT_MP_GESTURE_FLINCH_PRIMARY,
    ACT_MP_GESTURE_FLINCH_SECONDARY,
    ACT_MP_GESTURE_FLINCH_MELEE,
    ACT_MP_GESTURE_FLINCH_ITEM1,
    ACT_MP_GESTURE_FLINCH_ITEM2,
    ACT_MP_GESTURE_FLINCH_HEAD,
    ACT_MP_GESTURE_FLINCH_CHEST,
    ACT_MP_GESTURE_FLINCH_STOMACH,
    ACT_MP_GESTURE_FLINCH_LEFTARM,
    ACT_MP_GESTURE_FLINCH_RIGHTARM,
    ACT_MP_GESTURE_FLINCH_LEFTLEG,
    ACT_MP_GESTURE_FLINCH_RIGHTLEG,
    ACT_MP_GRENADE1_DRAW,
    ACT_MP_GRENADE1_IDLE,
    ACT_MP_GRENADE1_ATTACK,
    ACT_MP_GRENADE2_DRAW,
    ACT_MP_GRENADE2_IDLE,
    ACT_MP_GRENADE2_ATTACK,
    ACT_MP_PRIMARY_GRENADE1_DRAW,
    ACT_MP_PRIMARY_GRENADE1_IDLE,
    ACT_MP_PRIMARY_GRENADE1_ATTACK,
    ACT_MP_PRIMARY_GRENADE2_DRAW,
    ACT_MP_PRIMARY_GRENADE2_IDLE,
    ACT_MP_PRIMARY_GRENADE2_ATTACK,
    ACT_MP_SECONDARY_GRENADE1_DRAW,
    ACT_MP_SECONDARY_GRENADE1_IDLE,
    ACT_MP_SECONDARY_GRENADE1_ATTACK,
    ACT_MP_SECONDARY_GRENADE2_DRAW,
    ACT_MP_SECONDARY_GRENADE2_IDLE,
    ACT_MP_SECONDARY_GRENADE2_ATTACK,
    ACT_MP_MELEE_GRENADE1_DRAW,
    ACT_MP_MELEE_GRENADE1_IDLE,
    ACT_MP_MELEE_GRENADE1_ATTACK,
    ACT_MP_MELEE_GRENADE2_DRAW,
    ACT_MP_MELEE_GRENADE2_IDLE,
    ACT_MP_MELEE_GRENADE2_ATTACK,
    ACT_MP_ITEM1_GRENADE1_DRAW,
    ACT_MP_ITEM1_GRENADE1_IDLE,
    ACT_MP_ITEM1_GRENADE1_ATTACK,
    ACT_MP_ITEM1_GRENADE2_DRAW,
    ACT_MP_ITEM1_GRENADE2_IDLE,
    ACT_MP_ITEM1_GRENADE2_ATTACK,
    ACT_MP_ITEM2_GRENADE1_DRAW,
    ACT_MP_ITEM2_GRENADE1_IDLE,
    ACT_MP_ITEM2_GRENADE1_ATTACK,
    ACT_MP_ITEM2_GRENADE2_DRAW,
    ACT_MP_ITEM2_GRENADE2_IDLE,
    ACT_MP_ITEM2_GRENADE2_ATTACK,
    ACT_MP_STAND_BUILDING,
    ACT_MP_CROUCH_BUILDING,
    ACT_MP_RUN_BUILDING,
    ACT_MP_WALK_BUILDING,
    ACT_MP_AIRWALK_BUILDING,
    ACT_MP_CROUCHWALK_BUILDING,
    ACT_MP_JUMP_BUILDING,
    ACT_MP_JUMP_START_BUILDING,
    ACT_MP_JUMP_FLOAT_BUILDING,
    ACT_MP_JUMP_LAND_BUILDING,
    ACT_MP_SWIM_BUILDING,
    ACT_MP_ATTACK_STAND_BUILDING,
    ACT_MP_ATTACK_CROUCH_BUILDING,
    ACT_MP_ATTACK_SWIM_BUILDING,
    ACT_MP_ATTACK_AIRWALK_BUILDING,
    ACT_MP_ATTACK_STAND_GRENADE_BUILDING,
    ACT_MP_ATTACK_CROUCH_GRENADE_BUILDING,
    ACT_MP_ATTACK_SWIM_GRENADE_BUILDING,
    ACT_MP_ATTACK_AIRWALK_GRENADE_BUILDING,
    ACT_MP_STAND_PDA,
    ACT_MP_CROUCH_PDA,
    ACT_MP_RUN_PDA,
    ACT_MP_WALK_PDA,
    ACT_MP_AIRWALK_PDA,
    ACT_MP_CROUCHWALK_PDA,
    ACT_MP_JUMP_PDA,
    ACT_MP_JUMP_START_PDA,
    ACT_MP_JUMP_FLOAT_PDA,
    ACT_MP_JUMP_LAND_PDA,
    ACT_MP_SWIM_PDA,
    ACT_MP_ATTACK_STAND_PDA,
    ACT_MP_ATTACK_SWIM_PDA,
    ACT_MP_GESTURE_VC_HANDMOUTH,
    ACT_MP_GESTURE_VC_FINGERPOINT,
    ACT_MP_GESTURE_VC_FISTPUMP,
    ACT_MP_GESTURE_VC_THUMBSUP,
    ACT_MP_GESTURE_VC_NODYES,
    ACT_MP_GESTURE_VC_NODNO,
    ACT_MP_GESTURE_VC_HANDMOUTH_PRIMARY,
    ACT_MP_GESTURE_VC_FINGERPOINT_PRIMARY,
    ACT_MP_GESTURE_VC_FISTPUMP_PRIMARY,
    ACT_MP_GESTURE_VC_THUMBSUP_PRIMARY,
    ACT_MP_GESTURE_VC_NODYES_PRIMARY,
    ACT_MP_GESTURE_VC_NODNO_PRIMARY,
    ACT_MP_GESTURE_VC_HANDMOUTH_SECONDARY,
    ACT_MP_GESTURE_VC_FINGERPOINT_SECONDARY,
    ACT_MP_GESTURE_VC_FISTPUMP_SECONDARY,
    ACT_MP_GESTURE_VC_THUMBSUP_SECONDARY,
    ACT_MP_GESTURE_VC_NODYES_SECONDARY,
    ACT_MP_GESTURE_VC_NODNO_SECONDARY,
    ACT_MP_GESTURE_VC_HANDMOUTH_MELEE,
    ACT_MP_GESTURE_VC_FINGERPOINT_MELEE,
    ACT_MP_GESTURE_VC_FISTPUMP_MELEE,
    ACT_MP_GESTURE_VC_THUMBSUP_MELEE,
    ACT_MP_GESTURE_VC_NODYES_MELEE,
    ACT_MP_GESTURE_VC_NODNO_MELEE,
    ACT_MP_GESTURE_VC_HANDMOUTH_ITEM1,
    ACT_MP_GESTURE_VC_FINGERPOINT_ITEM1,
    ACT_MP_GESTURE_VC_FISTPUMP_ITEM1,
    ACT_MP_GESTURE_VC_THUMBSUP_ITEM1,
    ACT_MP_GESTURE_VC_NODYES_ITEM1,
    ACT_MP_GESTURE_VC_NODNO_ITEM1,
    ACT_MP_GESTURE_VC_HANDMOUTH_ITEM2,
    ACT_MP_GESTURE_VC_FINGERPOINT_ITEM2,
    ACT_MP_GESTURE_VC_FISTPUMP_ITEM2,
    ACT_MP_GESTURE_VC_THUMBSUP_ITEM2,
    ACT_MP_GESTURE_VC_NODYES_ITEM2,
    ACT_MP_GESTURE_VC_NODNO_ITEM2,
    ACT_MP_GESTURE_VC_HANDMOUTH_BUILDING,
    ACT_MP_GESTURE_VC_FINGERPOINT_BUILDING,
    ACT_MP_GESTURE_VC_FISTPUMP_BUILDING,
    ACT_MP_GESTURE_VC_THUMBSUP_BUILDING,
    ACT_MP_GESTURE_VC_NODYES_BUILDING,
    ACT_MP_GESTURE_VC_NODNO_BUILDING,
    ACT_MP_GESTURE_VC_HANDMOUTH_PDA,
    ACT_MP_GESTURE_VC_FINGERPOINT_PDA,
    ACT_MP_GESTURE_VC_FISTPUMP_PDA,
    ACT_MP_GESTURE_VC_THUMBSUP_PDA,
    ACT_MP_GESTURE_VC_NODYES_PDA,
    ACT_MP_GESTURE_VC_NODNO_PDA,
    ACT_VM_UNUSABLE,
    ACT_VM_UNUSABLE_TO_USABLE,
    ACT_VM_USABLE_TO_UNUSABLE,
    ACT_PRIMARY_VM_DRAW,
    ACT_PRIMARY_VM_HOLSTER,
    ACT_PRIMARY_VM_IDLE,
    ACT_PRIMARY_VM_PULLBACK,
    ACT_PRIMARY_VM_PRIMARYATTACK,
    ACT_PRIMARY_VM_SECONDARYATTACK,
    ACT_PRIMARY_VM_RELOAD,
    ACT_PRIMARY_VM_DRYFIRE,
    ACT_PRIMARY_VM_IDLE_TO_LOWERED,
    ACT_PRIMARY_VM_IDLE_LOWERED,
    ACT_PRIMARY_VM_LOWERED_TO_IDLE,
    ACT_SECONDARY_VM_DRAW,
    ACT_SECONDARY_VM_HOLSTER,
    ACT_SECONDARY_VM_IDLE,
    ACT_SECONDARY_VM_PULLBACK,
    ACT_SECONDARY_VM_PRIMARYATTACK,
    ACT_SECONDARY_VM_SECONDARYATTACK,
    ACT_SECONDARY_VM_RELOAD,
    ACT_SECONDARY_VM_DRYFIRE,
    ACT_SECONDARY_VM_IDLE_TO_LOWERED,
    ACT_SECONDARY_VM_IDLE_LOWERED,
    ACT_SECONDARY_VM_LOWERED_TO_IDLE,
    ACT_MELEE_VM_DRAW,
    ACT_MELEE_VM_HOLSTER,
    ACT_MELEE_VM_IDLE,
    ACT_MELEE_VM_PULLBACK,
    ACT_MELEE_VM_PRIMARYATTACK,
    ACT_MELEE_VM_SECONDARYATTACK,
    ACT_MELEE_VM_RELOAD,
    ACT_MELEE_VM_DRYFIRE,
    ACT_MELEE_VM_IDLE_TO_LOWERED,
    ACT_MELEE_VM_IDLE_LOWERED,
    ACT_MELEE_VM_LOWERED_TO_IDLE,
    ACT_PDA_VM_DRAW,
    ACT_PDA_VM_HOLSTER,
    ACT_PDA_VM_IDLE,
    ACT_PDA_VM_PULLBACK,
    ACT_PDA_VM_PRIMARYATTACK,
    ACT_PDA_VM_SECONDARYATTACK,
    ACT_PDA_VM_RELOAD,
    ACT_PDA_VM_DRYFIRE,
    ACT_PDA_VM_IDLE_TO_LOWERED,
    ACT_PDA_VM_IDLE_LOWERED,
    ACT_PDA_VM_LOWERED_TO_IDLE,
    ACT_ITEM1_VM_DRAW,
    ACT_ITEM1_VM_HOLSTER,
    ACT_ITEM1_VM_IDLE,
    ACT_ITEM1_VM_PULLBACK,
    ACT_ITEM1_VM_PRIMARYATTACK,
    ACT_ITEM1_VM_SECONDARYATTACK,
    ACT_ITEM1_VM_RELOAD,
    ACT_ITEM1_VM_DRYFIRE,
    ACT_ITEM1_VM_IDLE_TO_LOWERED,
    ACT_ITEM1_VM_IDLE_LOWERED,
    ACT_ITEM1_VM_LOWERED_TO_IDLE,
    ACT_ITEM2_VM_DRAW,
    ACT_ITEM2_VM_HOLSTER,
    ACT_ITEM2_VM_IDLE,
    ACT_ITEM2_VM_PULLBACK,
    ACT_ITEM2_VM_PRIMARYATTACK,
    ACT_ITEM2_VM_SECONDARYATTACK,
    ACT_ITEM2_VM_RELOAD,
    ACT_ITEM2_VM_DRYFIRE,
    ACT_ITEM2_VM_IDLE_TO_LOWERED,
    ACT_ITEM2_VM_IDLE_LOWERED,
    ACT_ITEM2_VM_LOWERED_TO_IDLE,
    ACT_RELOAD_SUCCEED,
    ACT_RELOAD_FAIL,
    ACT_WALK_AIM_AUTOGUN,
    ACT_RUN_AIM_AUTOGUN,
    ACT_IDLE_AUTOGUN,
    ACT_IDLE_AIM_AUTOGUN,
    ACT_RELOAD_AUTOGUN,
    ACT_CROUCH_IDLE_AUTOGUN,
    ACT_RANGE_ATTACK_AUTOGUN,
    ACT_JUMP_AUTOGUN,
    ACT_IDLE_AIM_PISTOL,
    ACT_WALK_AIM_DUAL,
    ACT_RUN_AIM_DUAL,
    ACT_IDLE_DUAL,
    ACT_IDLE_AIM_DUAL,
    ACT_RELOAD_DUAL,
    ACT_CROUCH_IDLE_DUAL,
    ACT_RANGE_ATTACK_DUAL,
    ACT_JUMP_DUAL,
    ACT_IDLE_SHOTGUN,
    ACT_IDLE_AIM_SHOTGUN,
    ACT_CROUCH_IDLE_SHOTGUN,
    ACT_JUMP_SHOTGUN,
    ACT_IDLE_AIM_RIFLE,
    ACT_RELOAD_RIFLE,
    ACT_CROUCH_IDLE_RIFLE,
    ACT_RANGE_ATTACK_RIFLE,
    ACT_JUMP_RIFLE,
    ACT_SLEEP,
    ACT_WAKE,
    ACT_FLICK_LEFT,
    ACT_FLICK_LEFT_MIDDLE,
    ACT_FLICK_RIGHT_MIDDLE,
    ACT_FLICK_RIGHT,
    ACT_SPINAROUND,
    ACT_PREP_TO_FIRE,
    ACT_FIRE,
    ACT_FIRE_RECOVER,
    ACT_SPRAY,
    ACT_PREP_EXPLODE,
    ACT_EXPLODE,
    ACT_DOTA_IDLE,
    ACT_DOTA_RUN,
    ACT_DOTA_ATTACK,
    ACT_DOTA_ATTACK_EVENT,
    ACT_DOTA_DIE,
    ACT_DOTA_FLINCH,
    ACT_DOTA_DISABLED,
    ACT_DOTA_CAST_ABILITY_1,
    ACT_DOTA_CAST_ABILITY_2,
    ACT_DOTA_CAST_ABILITY_3,
    ACT_DOTA_CAST_ABILITY_4,
    ACT_DOTA_OVERRIDE_ABILITY_1,
    ACT_DOTA_OVERRIDE_ABILITY_2,
    ACT_DOTA_OVERRIDE_ABILITY_3,
    ACT_DOTA_OVERRIDE_ABILITY_4,
    ACT_DOTA_CHANNEL_ABILITY_1,
    ACT_DOTA_CHANNEL_ABILITY_2,
    ACT_DOTA_CHANNEL_ABILITY_3,
    ACT_DOTA_CHANNEL_ABILITY_4,
    ACT_DOTA_CHANNEL_END_ABILITY_1,
    ACT_DOTA_CHANNEL_END_ABILITY_2,
    ACT_DOTA_CHANNEL_END_ABILITY_3,
    ACT_DOTA_CHANNEL_END_ABILITY_4,
    ACT_MP_RUN_SPEEDPAINT,
    ACT_MP_LONG_FALL,
    ACT_MP_TRACTORBEAM_FLOAT,
    ACT_MP_DEATH_CRUSH,
    ACT_MP_RUN_SPEEDPAINT_PRIMARY,
    ACT_MP_DROWNING_PRIMARY,
    ACT_MP_LONG_FALL_PRIMARY,
    ACT_MP_TRACTORBEAM_FLOAT_PRIMARY,
    ACT_MP_DEATH_CRUSH_PRIMARY,
    ACT_DIE_STAND,
    ACT_DIE_STAND_HEADSHOT,
    ACT_DIE_CROUCH,
    ACT_DIE_CROUCH_HEADSHOT,
    ACT_CSGO_NULL,
    ACT_CSGO_DEFUSE,
    ACT_CSGO_DEFUSE_WITH_KIT,
    ACT_CSGO_FLASHBANG_REACTION,
    ACT_CSGO_FIRE_PRIMARY,
    ACT_CSGO_FIRE_PRIMARY_OPT_1,
    ACT_CSGO_FIRE_PRIMARY_OPT_2,
    ACT_CSGO_FIRE_SECONDARY,
    ACT_CSGO_FIRE_SECONDARY_OPT_1,
    ACT_CSGO_FIRE_SECONDARY_OPT_2,
    ACT_CSGO_RELOAD,
    ACT_CSGO_RELOAD_START,
    ACT_CSGO_RELOAD_LOOP,
    ACT_CSGO_RELOAD_END,
    ACT_CSGO_OPERATE,
    ACT_CSGO_DEPLOY,
    ACT_CSGO_CATCH,
    ACT_CSGO_SILENCER_DETACH,
    ACT_CSGO_SILENCER_ATTACH,
    ACT_CSGO_TWITCH,
    ACT_CSGO_TWITCH_BUYZONE,
    ACT_CSGO_PLANT_BOMB,
    ACT_CSGO_IDLE_TURN_BALANCEADJUST,
    ACT_CSGO_IDLE_ADJUST_STOPPEDMOVING,
    ACT_CSGO_ALIVE_LOOP,
    ACT_CSGO_FLINCH,
    ACT_CSGO_FLINCH_HEAD,
    ACT_CSGO_FLINCH_MOLOTOV,
    ACT_CSGO_JUMP,
    ACT_CSGO_FALL,
    ACT_CSGO_CLIMB_LADDER,
    ACT_CSGO_LAND_LIGHT,
    ACT_CSGO_LAND_HEAVY,
    ACT_CSGO_EXIT_LADDER_TOP,
    ACT_CSGO_EXIT_LADDER_BOTTOM,
};
*/