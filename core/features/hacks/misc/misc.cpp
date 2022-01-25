#include "../../features.hpp"
#include "../../../menu/variables.hpp"
#include "./misc.h"

void c_misc::third_person() {

	if (!csgo::local_player)
		return;

	interfaces::input->camera_in_third_person = this->m_third_person && csgo::local_player->is_alive();
	if (!interfaces::input->camera_in_third_person)
		return;

	const auto& weapon = csgo::local_player->active_weapon();

	const auto weapon_type = weapon->get_weapon_data()->nWeaponType;

	if (weapon_type == WEAPONTYPE_GRENADE)
		interfaces::input->camera_in_third_person = false;

	if ((weapon_type == WEAPONTYPE_PISTOL || weapon_type == WEAPONTYPE_MACHINEGUN || weapon_type == WEAPONTYPE_RIFLE || weapon_type == WEAPONTYPE_SHOTGUN || weapon_type == WEAPONTYPE_SNIPER_RIFLE || weapon_type == WEAPONTYPE_SUBMACHINEGUN) && this->m_third_person_disable_on_weapon)
		interfaces::input->camera_in_third_person = false;

	vec3_t angles;
	interfaces::engine->get_view_angles(angles);

	vec3_t backward(angles.x, angles.y + 180.f, angles.z);
	backward.clamp();
	backward.normalize();

	vec3_t range;
	math::angle_to_vector(backward, range);
	range *= 8192.f;

	const auto start = csgo::local_player->get_eye_pos();

	trace_filter filter;
	filter.skip = csgo::local_player;

	ray_t ray;
	ray.initialize(start, static_cast<vec3_t>(start) + range);

	trace_t tr;
	interfaces::trace_ray->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

	angles.z = std::min<int>(start.DistTo(tr.end), 150); // 150 is better distance

	interfaces::input->camera_offset = angles;

	csgo::local_player->update_client_side_animations();
}