#include "utilities.hpp"
#include "../utilities/csgo.hpp"
#include <psapi.h>

std::uint8_t* utilities::pattern_scan(const char* module_name, const char* signature) noexcept {
    const auto module_handle = GetModuleHandleA(module_name);

    if (!module_handle)
        return nullptr;

    static auto pattern_to_byte = [](const char* pattern) {
        auto bytes = std::vector<int>{};
        auto start = const_cast<char*>(pattern);
        auto end = const_cast<char*>(pattern) + std::strlen(pattern);

        for (auto current = start; current < end; ++current) {
            if (*current == '?') {
                ++current;

                if (*current == '?')
                    ++current;

                bytes.push_back(-1);
            }
            else {
                bytes.push_back(std::strtoul(current, &current, 16));
            }
        }
        return bytes;
    };

    auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
    auto nt_headers =
        reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module_handle) + dos_header->e_lfanew);

    auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
    auto pattern_bytes = pattern_to_byte(signature);
    auto scan_bytes = reinterpret_cast<std::uint8_t*>(module_handle);

    auto s = pattern_bytes.size();
    auto d = pattern_bytes.data();

    for (auto i = 0ul; i < size_of_image - s; ++i) {
        bool found = true;

        for (auto j = 0ul; j < s; ++j) {
            if (scan_bytes[i + j] != d[j] && d[j] != -1) {
                found = false;
                break;
            }
        }
        if (found)
            return &scan_bytes[i];
    }

    throw std::runtime_error(std::string("Wrong signature: ") + signature);
}

bool utilities::get_bbox(entity_t* e, Box& b, bool player_esp)
{
	auto collideable = e->collideable();
	auto& m_rgflCoordinateFrame = e->m_rgflCoordinateFrame();

	auto min = collideable->mins();
	auto max = collideable->maxs();

	vec3_t points[8] =
	{
		vec3_t(min.x, min.y, min.z),
		vec3_t(min.x, max.y, min.z),
		vec3_t(max.x, max.y, min.z),
		vec3_t(max.x, min.y, min.z),
		vec3_t(max.x, max.y, max.z),
		vec3_t(min.x, max.y, max.z),
		vec3_t(min.x, min.y, max.z),
		vec3_t(max.x, min.y, max.z)
	};

	vec3_t pointsTransformed[8];

	for (auto i = 0; i < 8; i++) {
		math::vector_transform(points[i], m_rgflCoordinateFrame, pointsTransformed[i]);
		
	}


	vec3_t pos = e->origin();
	vec3_t flb;
	vec3_t brt;
	vec3_t blb;
	vec3_t frt;
	vec3_t frb;
	vec3_t brb;
	vec3_t blt;
	vec3_t flt;

	auto bFlb = math::world_to_screen(pointsTransformed[3], flb);
	auto bBrt = math::world_to_screen(pointsTransformed[5], brt);
	auto bBlb = math::world_to_screen(pointsTransformed[0], blb);
	auto bFrt = math::world_to_screen(pointsTransformed[4], frt);
	auto bFrb = math::world_to_screen(pointsTransformed[2], frb);
	auto bBrb = math::world_to_screen(pointsTransformed[1], brb);
	auto bBlt = math::world_to_screen(pointsTransformed[6], blt);
	auto bFlt = math::world_to_screen(pointsTransformed[7], flt);

	if (!bFlb && !bBrt && !bBlb && !bFrt && !bFrb && !bBrb && !bBlt && !bFlt)
		return false;

	vec3_t arr[8] =
	{
		flb,
		brt,
		blb,
		frt,
		frb,
		brb,
		blt,
		flt
	};

	auto left = flb.x;
	auto top = flb.y;
	auto right = flb.x;
	auto bottom = flb.y;

	for (auto i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (top < arr[i].y)
			top = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (bottom > arr[i].y)
			bottom = arr[i].y;
	}

	b.x = left;
	b.y = bottom;
	b.w = right - left;
	b.h = top - bottom;

	return true;
}

void utilities::movement_fix(c_usercmd* m_pcmd)
{
	vec3_t view_fwd, view_right, view_up, g_cmd_fwd, g_cmd_right, g_cmd_up;
	auto viewangles = m_pcmd->viewangles;
	viewangles.normalized();

	math::angle_vectors(g_ctx.globals.wish_angle, &view_fwd, &view_right, &view_up);
	math::angle_vectors(viewangles, &g_cmd_fwd, &g_cmd_right, &g_cmd_up);

	const auto v8 = sqrtf((view_fwd.x * view_fwd.x) + (view_fwd.y * view_fwd.y));
	const auto v10 = sqrtf((view_right.x * view_right.x) + (view_right.y * view_right.y));
	const auto v12 = sqrtf(view_up.z * view_up.z);

	const vec3_t norm_view_fwd((1.f / v8) * view_fwd.x, (1.f / v8) * view_fwd.y, 0.f);
	const vec3_t norm_view_right((1.f / v10) * view_right.x, (1.f / v10) * view_right.y, 0.f);
	const vec3_t norm_view_up(0.f, 0.f, (1.f / v12) * view_up.z);

	const auto v14 = sqrtf((g_cmd_fwd.x * g_cmd_fwd.x) + (g_cmd_fwd.y * g_cmd_fwd.y));
	const auto v16 = sqrtf((g_cmd_right.x * g_cmd_right.x) + (g_cmd_right.y * g_cmd_right.y));
	const auto v18 = sqrtf(g_cmd_up.z * g_cmd_up.z);

	const vec3_t norm_g_cmd_fwd((1.f / v14) * g_cmd_fwd.x, (1.f / v14) * g_cmd_fwd.y, 0.f);
	const vec3_t norm_g_cmd_right((1.f / v16) * g_cmd_right.x, (1.f / v16) * g_cmd_right.y, 0.f);
	const vec3_t norm_g_cmd_up(0.f, 0.f, (1.f / v18) * g_cmd_up.z);

	const auto v22 = norm_view_fwd.x * m_pcmd->forwardmove;
	const auto v26 = norm_view_fwd.y * m_pcmd->forwardmove;
	const auto v28 = norm_view_fwd.z * m_pcmd->forwardmove;
	const auto v24 = norm_view_right.x * m_pcmd->sidemove;
	const auto v23 = norm_view_right.y * m_pcmd->sidemove;
	const auto v25 = norm_view_right.z * m_pcmd->sidemove;
	const auto v30 = norm_view_up.x * m_pcmd->upmove;
	const auto v27 = norm_view_up.z * m_pcmd->upmove;
	const auto v29 = norm_view_up.y * m_pcmd->upmove;

	m_pcmd->forwardmove = ((((norm_g_cmd_fwd.x * v24) + (norm_g_cmd_fwd.y * v23)) + (norm_g_cmd_fwd.z * v25))
		+ (((norm_g_cmd_fwd.x * v22) + (norm_g_cmd_fwd.y * v26)) + (norm_g_cmd_fwd.z * v28)))
		+ (((norm_g_cmd_fwd.y * v30) + (norm_g_cmd_fwd.x * v29)) + (norm_g_cmd_fwd.z * v27));
	m_pcmd->sidemove = ((((norm_g_cmd_right.x * v24) + (norm_g_cmd_right.y * v23)) + (norm_g_cmd_right.z * v25))
		+ (((norm_g_cmd_right.x * v22) + (norm_g_cmd_right.y * v26)) + (norm_g_cmd_right.z * v28)))
		+ (((norm_g_cmd_right.x * v29) + (norm_g_cmd_right.y * v30)) + (norm_g_cmd_right.z * v27));
	m_pcmd->upmove = ((((norm_g_cmd_up.x * v23) + (norm_g_cmd_up.y * v24)) + (norm_g_cmd_up.z * v25))
		+ (((norm_g_cmd_up.x * v26) + (norm_g_cmd_up.y * v22)) + (norm_g_cmd_up.z * v28)))
		+ (((norm_g_cmd_up.x * v30) + (norm_g_cmd_up.y * v29)) + (norm_g_cmd_up.z * v27));

	g_ctx.globals.wish_angle = viewangles;

	static auto cl_forwardspeed = interfaces::console->get_convar("cl_forwardspeed");
	static auto cl_sidespeed = interfaces::console->get_convar("cl_sidespeed");
	static auto cl_upspeed = interfaces::console->get_convar("cl_upspeed");

	m_pcmd->forwardmove = math::clamp(m_pcmd->forwardmove, -cl_forwardspeed->float_value, cl_forwardspeed->float_value);
	m_pcmd->sidemove = math::clamp(m_pcmd->sidemove, -cl_sidespeed->float_value, cl_sidespeed->float_value);
	m_pcmd->upmove = math::clamp(m_pcmd->upmove, -cl_upspeed->float_value, cl_upspeed->float_value);
}

void utilities::rotate_movement(c_usercmd* cmd, float yaw)
{
	vec3_t viewangles;
	interfaces::engine->get_view_angles(viewangles);

	float rotation = DEG2RAD(viewangles.y - yaw);

	float cos_rot = cos(rotation);
	float sin_rot = sin(rotation);

	float new_forwardmove = cos_rot * cmd->forwardmove - sin_rot * cmd->sidemove;
	float new_sidemove = sin_rot * cmd->forwardmove + cos_rot * cmd->sidemove;

	cmd->forwardmove = new_forwardmove;
	cmd->forwardmove = new_sidemove;
}

