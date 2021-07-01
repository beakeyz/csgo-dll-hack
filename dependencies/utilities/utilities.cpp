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
	matrix_t& m_rgflCoordinateFrame = e->m_rgflCoordinateFrame();

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

	for (auto i = 0; i < 8; i++)
		math::transform_vector(points[i], m_rgflCoordinateFrame, pointsTransformed[i]);

	vec3_t pos = e->get_eye_pos();
	vec2_t flb;
	vec2_t brt;
	vec2_t blb;
	vec2_t frt;
	vec2_t frb;
	vec2_t brb;
	vec2_t blt;
	vec2_t flt;

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

	vec2_t arr[8] =
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
