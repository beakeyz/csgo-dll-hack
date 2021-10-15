#include "./autowall.h"

#define HITGROUP_GENERIC	0
#define HITGROUP_HEAD		1
#define HITGROUP_CHEST		2
#define HITGROUP_STOMACH	3
#define HITGROUP_LEFTARM	4
#define HITGROUP_RIGHTARM	5
#define HITGROUP_LEFTLEG	6
#define HITGROUP_RIGHTLEG	7
#define HITGROUP_GEAR		10

c_autowall g_autowall;

void c_autowall::ScaleDamage(int hitgroup, player_t* enemy, float weapon_armor_ratio, float& current_damage)
{
	int armor = enemy->armor();
	float ratio;

	switch (hitgroup)
	{
	case hitgroup_head:
		current_damage *= 4.f;
		break;
	case hitgroup_stomach:
		current_damage *= 1.25f;
		break;
	case hitgroup_leftleg:
	case hitgroup_rightleg:
		current_damage *= 0.75f;
		break;
	}

	if (armor > 0)
	{
		switch (hitgroup)
		{
		case hitgroup_head:
			if (enemy->has_helmet())
			{
				ratio = (weapon_armor_ratio * 0.5) * current_damage;
				if (((current_damage - ratio) * 0.5) > armor)
				{
					ratio = current_damage - (armor * 2.0);
				}
				current_damage = ratio;
			}
			break;
		case hitgroup_generic:
		case hitgroup_chest:
		case hitgroup_stomach:
		case hitgroup_leftarm:
		case hitgroup_rightarm:
			ratio = (weapon_armor_ratio * 0.5) * current_damage;
			if (((current_damage - ratio) * 0.5) > armor)
			{
				ratio = current_damage - (armor * 2.0);
			}
			current_damage = ratio;
			break;
		}
	}


}

void c_autowall::scale_damage(const int& hit_group, const float& weapon_armor_ratio, const bool& has_helmet, const float& armor_value, float& current_damage)
{
	switch (hit_group)
	{
	case HITGROUP_HEAD:
		current_damage *= 4.f;
		break;
	case HITGROUP_STOMACH:
		current_damage *= 1.25f;
		break;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		current_damage *= 0.75f;
		break;
	}

	const auto armor = armor_value;
	if (armor > 0)
	{
		float ratio = 0.f;
		switch (hit_group)
		{
		case HITGROUP_HEAD:
			if (has_helmet)
			{
				ratio = (weapon_armor_ratio * 0.5) * current_damage;
				if (((current_damage - ratio) * 0.5) > armor)
				{
					ratio = current_damage - (armor * 2.0);
				}
				current_damage = ratio;
			}

			break;
		case HITGROUP_GENERIC:
		case HITGROUP_CHEST:
		case HITGROUP_STOMACH:
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:
			ratio = (weapon_armor_ratio * 0.5) * current_damage;
			if (((current_damage - ratio) * 0.5) > armor)
			{
				ratio = current_damage - (armor * 2.0);
			}
			current_damage = ratio;
			break;
		}
	}
}

void c_autowall::scale_damage(int hit_group, player_t* enemy, float weapon_armor_ratio, float& current_damage)
{
	scale_damage(hit_group, weapon_armor_ratio, enemy->has_helmet(), enemy->armor(), current_damage);
}

bool c_autowall::trace_to_exit(vec3_t& end, trace_t* enter_trace, vec3_t start, vec3_t dir, trace_t* exit_trace)
{
	float distance = 0.0f;

	while (distance <= 90.0f)
	{
		distance += 4.0f;
		end = start + dir * distance;

		auto point_contents = interfaces::trace_ray->get_point_contents(end, MASK_SHOT_HULL | CONTENTS_HITBOX, NULL);

		if (point_contents & MASK_SHOT_HULL && !(point_contents & CONTENTS_HITBOX))
			continue;

		auto new_end = end - (dir * 4.0f);

		ray_t ray;
		ray.initialize(end, new_end);
		interfaces::trace_ray->trace_ray(ray, MASK_SHOT, 0, exit_trace);

		if (exit_trace->startSolid && exit_trace->surface.flags & SURF_HITBOX)
		{
			ray.initialize(end, start);

			trace_filter filter;
			filter.skip = exit_trace->entity;

			interfaces::trace_ray->trace_ray(ray, MASK_SHOT | CONTENTS_HITBOX , &filter, exit_trace);

			if ((exit_trace->flFraction < 1.0f || exit_trace->allsolid) && !exit_trace->startSolid)
			{
				end = exit_trace->end;
				return true;
			}

			continue;
		}

		if (!(exit_trace->flFraction < 1.0 || exit_trace->allsolid || exit_trace->startSolid) || exit_trace->startSolid)
		{
			if (exit_trace->entity)
			{
				if (enter_trace->entity && enter_trace->entity == csgo::local_player)
					return true;
			}

			continue;
		}

		if (exit_trace->surface.flags >> 7 & 1 && !(enter_trace->surface.flags >> 7 & 1))
			continue;

		if (exit_trace->plane.normal.dot(dir) <= 1.0f)
		{
			auto fraction = exit_trace->flFraction * 4.0f;
			end = end - dir * fraction;

			return true;
		}
	}

	return false;
}

bool c_autowall::handle_bullet_penetration(weapon_info_t* weapon_data, fire_bullet_data& data)
{
	surface_data_t* enter_surface_data = interfaces::physics_surface->get_surface_data(data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = enter_surface_data->game.penetrationmodifier;

	data.trace_length += data.enter_trace.flFraction * data.trace_length_remaining;
	data.current_damage *= powf(weapon_data->flRangeModifier, data.trace_length * 0.002f);

	if (data.trace_length > 3000.f || enter_surf_penetration_mod < 0.1f)
		data.penetrate_count = 0;

	if (data.penetrate_count <= 0)
		return false;

	vec3_t dummy;
	trace_t trace_exit;

	if (!trace_to_exit(dummy, &data.enter_trace, data.enter_trace.end, data.direction, &trace_exit))
		return false;

	surface_data_t* exit_surface_data = interfaces::physics_surface->get_surface_data(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;

	float exit_surf_penetration_mod = *(float*)(((uint8_t*)exit_surface_data) + 76);
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;

	if ((data.enter_trace.contents & CONTENTS_GRATE) != 0 || enter_material == 89 || enter_material == 71)
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else
		combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;

	if (enter_material == exit_material)
	{
		if (exit_material == 87 || exit_material == 85)
			combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)
			combined_penetration_modifier = 2.0f;
	}

	float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / weapon_data->flPenetration) * 1.25f);
	float thickness = std::sqrt((trace_exit.end - data.enter_trace.end).length_sqr());

	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;

	float lost_damage = fmaxf(0.0f, v35 + thickness);

	if (lost_damage > data.current_damage)
		return false;

	if (lost_damage >= 0.0f)
		data.current_damage -= lost_damage;

	if (data.current_damage < 1.0f)
		return false;

	data.src = trace_exit.end;
	data.penetrate_count--;

	return true;
}

void c_autowall::trace_line(const vec3_t& start, const vec3_t& end, unsigned int mask, player_t* ignore, trace_t* ptr)
{
	ray_t ray;
	ray.initialize(start, end);
	trace_filter filter;
	filter.skip = ignore;

	interfaces::trace_ray->trace_ray(ray, mask, &filter, ptr);
}

bool c_autowall::simulate_fire_bullet(player_t* ignore, weapon_t* weapon, fire_bullet_data& data)
{
	auto weapon_data = weapon->get_weapon_data();

	data.penetrate_count = 4;
	data.trace_length = 0.0f;
	data.current_damage = float(weapon_data->iDamage);


	while (data.penetrate_count > 0 && data.current_damage >= 1.0f)
	{
		data.trace_length_remaining = weapon_data->flRange - data.trace_length;
		vec3_t end = data.src + data.direction * data.trace_length_remaining;

		//trace_line(data.src, end, MASK_SHOT, ignore, &data.enter_trace); //was commented

		//ray_t ray; //was commented
		//ray.initialize(data.src, end + data.direction * 40.f); //was commented

		//interfaces::trace_ray->trace_ray(ray, MASK_SHOT, &data.filter, &data.enter_trace); //was commented

		trace_line(data.src, end + data.direction * 40.f, MASK_SHOT_HULL | CONTENTS_HITBOX, ignore, &data.enter_trace);

		if (data.enter_trace.flFraction == 1.0f) {
			interfaces::console->console_printf("fraction = 1.0f\n");
			break;
		}

		if (!data.enter_trace.did_hit()) {
			interfaces::console->console_printf("hit nothing!\n");
		}

		if (data.enter_trace.hitGroup != HITGROUP_GEAR && data.enter_trace.hitGroup != HITGROUP_GENERIC)
		{
			auto player = (player_t*)data.enter_trace.entity;
			if (!player || (player->team() == ignore->team() && !combat::ragebot::teammates))
				break;

			data.trace_length += data.enter_trace.flFraction * data.trace_length_remaining;
			data.current_damage *= powf(weapon_data->flRangeModifier, data.trace_length * 0.002f);

			scale_damage(data.enter_trace.hitGroup, player, weapon_data->flArmorRatio, data.current_damage);

			return true;
		}

		if (!handle_bullet_penetration(weapon_data, data)) {
			break;
		}
	}

	return false;
}

float c_autowall::get_damage(const vec3_t& point)
{
	return get_damage(csgo::local_player, point);
}

float c_autowall::get_damage(player_t* player, const vec3_t& point)
{
	float damage = 0.f;

	auto weapon = player->active_weapon();
	if (!weapon || !weapon->is_weapon())
		return -1.0f;

	fire_bullet_data data;

	data.src = player->get_eye_pos();
	data.filter.skip = player;

	data.direction = point - data.src;
	data.direction = data.direction.normalized();

	if (simulate_fire_bullet(player, weapon, data)) {
		//std::ostringstream stream;
		//stream << data.current_damage << "\n";
		//console::log(stream.str().c_str());
		damage = data.current_damage;
	}

	return damage;
}
