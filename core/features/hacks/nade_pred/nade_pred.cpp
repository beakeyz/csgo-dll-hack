#include "nade_pred.h"
#include "./../features.h"
#include "./../../../menu/menu.hpp"

#include <mutex>

void CCSGrenadeHint::Tick(int buttons)
{
	if (!c_visuals::get().projectile_prediction)
		return;

	bool _in_attack = (buttons & in_attack);
	bool _in_attack2 = (buttons & in_attack2);

	act = (_in_attack && _in_attack2) ? ACT_DROP :
		(_in_attack2) ? ACT_THROW :
		(_in_attack) ? ACT_LOB :
		ACT_NONE;
}

void CCSGrenadeHint::View()
{
	if (!c_visuals::get().projectile_prediction)
		return;

	if (!csgo::local_player)
		return;

	auto weapon = csgo::local_player->active_weapon();
	if (!weapon)
		return;

	if ((weapon->client_class()->class_id == class_ids::cbasecsgrenade) && act != ACT_NONE)
	{
		vec3_t Angles;
		interfaces::engine->get_view_angles(Angles);

		c_client_class* pWeaponClass = weapon->client_class();
		if (!pWeaponClass)
		{
			type = -1;
			Simulate(Angles, csgo::local_player);
		}
		else
		{
			type = pWeaponClass->class_id;
			Simulate(Angles, csgo::local_player);
		}
	}
	else
	{
		type = -1;
	}
}

inline float CSGO_Armor(float flDamage, int ArmorValue) {
	float flArmorRatio = 0.5f;
	float flArmorBonus = 0.5f;
	if (ArmorValue > 0) {
		float flNew = flDamage * flArmorRatio;
		float flArmor = (flDamage - flNew) * flArmorBonus;

		if (flArmor > static_cast<float>(ArmorValue)) {
			flArmor = static_cast<float>(ArmorValue) * (1.f / flArmorBonus);
			flNew = flDamage - flArmor;
		}

		flDamage = flNew;
	}
	return flDamage;
}

void CCSGrenadeHint::Paint()
{
	if (!c_visuals::get().projectile_prediction)
		return;

	if (!csgo::local_player)
		return;

	auto weapon = csgo::local_player->active_weapon();
	if (!weapon)
		return;

	if ((type) && path.size() > 1 && act != ACT_NONE && (weapon->client_class()->class_id == class_ids::cbasecsgrenade))
	{
		vec3_t ab, cd;
		vec3_t prev = path[0];
		for (auto it = path.begin(), end = path.end(); it != end; ++it)
		{
			if (math::world_to_screen(prev, ab) && math::world_to_screen(*it, cd))
			{
				interfaces::surface->set_drawing_color(TracerColor.r, TracerColor.g, TracerColor.b);
				interfaces::surface->draw_line(ab[0], ab[1], cd[0], cd[1]);
			}
			prev = *it;
		}

		for (auto it = OtherCollisions.begin(), end = OtherCollisions.end(); it != end; ++it)
		{
			render::draw_rect(it->first.x, it->first.y, 2.f, 2.f, color(0, 255, 0, 200));
		}

		//Visuals::Draw3DCube(2.f, OtherCollisions.rbegin()->second, OtherCollisions.rbegin()->first, Color(255, 0, 0, 200));

		std::string EntName;
		auto bestdmg = 0;
		static color redcol = { 255, 0, 0, 255 };
		static color greencol = { 25, 255, 25, 255 };
		static color yellowgreencol = { 177, 253, 2, 255 };
		static color yellowcol = { 255, 255, 0, 255 };
		static color orangecol = { 255, 128, 0, 255 };
		color* BestColor = &redcol;

		vec3_t endpos = path[path.size() - 1];
		vec3_t absendpos = endpos;

		float totaladded = 0.0f;

		while (totaladded < 30.0f) {
			if (interfaces::trace_ray->get_point_contents(endpos) == CONTENTS_EMPTY)
				break;

			totaladded += 2.0f;
			endpos.z += 2.0f;
		}

		weapon_t* pWeapon = csgo::local_player->active_weapon();
		int weap_id = pWeapon->index();

		if (pWeapon &&
			weap_id == WEAPON_HEGRENADE ||
			weap_id == WEAPON_MOLOTOV ||
			weap_id == WEAPON_INCGRENADE) {
			for (int i = 1; i < 64; i++) {
				player_t* pEntity = static_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

				if (!pEntity || pEntity->team() == csgo::local_player->team())
					continue;

				float dist = (pEntity->origin() - endpos).length();

				if (dist < 350.0f) {
					trace_filter filter;
					filter.skip = csgo::local_player;
					ray_t ray;
					vec3_t NadeScreen;
					math::world_to_screen(endpos, NadeScreen);

					vec3_t vPelvis = pEntity->getBonePos(3);
					ray.initialize(endpos, vPelvis);
					trace_t ptr;
					interfaces::trace_ray->trace_ray(ray, MASK_SHOT, &filter, &ptr);

					if (ptr.entity == pEntity) {
						vec3_t PelvisScreen;

						math::world_to_screen(vPelvis, PelvisScreen);

						static float a = 105.0f;
						static float b = 25.0f;
						static float c = 140.0f;

						float d = ((((pEntity->origin()) - prev).length() - b) / c);
						float flDamage = a * exp(-d * d);
						auto dmg = max(static_cast<int>(ceilf(CSGO_Armor(flDamage, pEntity->armor()))), 0);

						color* destcolor = dmg >= 65 ? &redcol : dmg >= 40.0f ? &orangecol : dmg >= 20.0f ? &yellowgreencol : &greencol;

						if (dmg > bestdmg) {
							EntName = "hareld";
							BestColor = destcolor;
							bestdmg = dmg;
						}
					}
				}
			}
		}

		/*
		if (bestdmg > 0.f) {
			if (weap_id != WEAPON_HEGRENADE)
			{
				if (math::world_to_screen(prev, cd))
					Visuals::DrawString(Visuals::ui_font, cd[0], cd[1] - 10, *BestColor, FONT_CENTER, firegrenade_didnt_hit ? "No collisions" : (EntName + " will be burnt.").c_str());
			}
			else
			{
				if (math::world_to_screen(*path.begin(), cd))
					Visuals::DrawString(Visuals::ui_font, cd[0], cd[1] - 10, *BestColor, FONT_CENTER, ("Most damage dealt to: " + EntName + " -" + std::to_string(bestdmg)).c_str());
			}
		}
		*/
	}
}

void CCSGrenadeHint::Setup(player_t* pl, vec3_t& vecSrc, vec3_t& vecThrow, const vec3_t& angEyeAngles)
{
	vec3_t angThrow = angEyeAngles;
	float pitch = angThrow.x;

	if (pitch <= 90.0f)
	{
		if (pitch < -90.0f)
		{
			pitch += 360.0f;
		}
	}
	else
	{
		pitch -= 360.0f;
	}
	float a = pitch - (90.0f - fabs(pitch)) * 10.0f / 90.0f;
	angThrow.x = a;

	// Gets ThrowVelocity from weapon files
	// Clamped to [15,750]
	float flVel = 750.0f * 0.9f;

	// Do magic on member of grenade object [esi+9E4h]
	// m1=1  m1+m2=0.5  m2=0
	static const float power[] = { 1.0f, 1.0f, 0.5f, 0.0f };
	float b = power[act];
	// Clamped to [0,1]
	b = b * 0.7f;
	b = b + 0.3f;
	flVel *= b;

	vec3_t vForward, vRight, vUp;
	math::angle_vectors(angThrow, vForward, vRight, vUp);

	vecSrc = pl->origin();
	vecSrc += pl->view_offset();
	float off = (power[act] * 12.0f) - 12.0f;
	vecSrc.z += off;

	// Game calls UTIL_TraceHull here with hull and assigns vecSrc tr.endpos
	trace_t tr;
	vec3_t vecDest = vecSrc;
	vecDest.MulAdd(vecDest, vForward, 22.0f);
	TraceHull(vecSrc, vecDest, tr);

	// After the hull trace it moves 6 units back along vForward
	// vecSrc = tr.endpos - vForward * 6
	vec3_t vecBack = vForward; vecBack *= 6.0f;
	vecSrc = tr.end;
	vecSrc -= vecBack;

	// Finally calculate velocity
	vecThrow = pl->velocity(); vecThrow *= 1.25f;
	vecThrow.MulAdd(vecThrow, vForward, flVel);
}

void CCSGrenadeHint::Simulate(vec3_t& Angles, player_t* pLocal)
{
	vec3_t vecSrc, vecThrow;
	Setup(pLocal, vecSrc, vecThrow, Angles);

	float interval = interfaces::globals->interval_per_tick;

	// Log positions 20 times per sec
	int logstep = static_cast<int>(0.05f / interval);
	int logtimer = 0;

	path.clear();
	OtherCollisions.clear();
	TracerColor = color(255, 255, 0, 255);
	for (unsigned int i = 0; i < path.max_size() - 1; ++i)
	{
		if (!logtimer)
			path.push_back(vecSrc);

		int s = Step(vecSrc, vecThrow, i, interval);
		if ((s & 1) || vecThrow == vec3_t(0, 0, 0))
			break;

		// Reset the log timer every logstep OR we bounced
		if ((s & 2) || logtimer >= logstep) logtimer = 0;
		else ++logtimer;
	}
	path.push_back(vecSrc);
}

int CCSGrenadeHint::Step(vec3_t& vecSrc, vec3_t& vecThrow, int tick, float interval)
{
	// Apply gravity
	vec3_t move;
	AddGravityMove(move, vecThrow, interval, false);

	// Push entity
	trace_t tr;
	PushEntity(vecSrc, move, tr);

	int result = 0;
	// Check ending conditions
	if (CheckDetonate(vecThrow, tr, tick, interval))
	{
		result |= 1;
	}

	// Resolve collisions
	if (tr.flFraction != 1.0f)
	{
		result |= 2; // Collision!
		ResolveFlyCollisionCustom(tr, vecThrow, interval);
	}

	if (tr.entity && ((player_t*)tr.entity)->is_player())
	{
		TracerColor = color(255, 0, 0, 255);
	}

	if ((result & 1) || vecThrow == vec3_t(0, 0, 0) || tr.flFraction != 1.0f)
	{
		vec3_t angles;
		math::vector_angles((tr.end - tr.start).normalized(), angles);
		OtherCollisions.push_back(std::make_pair(tr.end, angles));
	}

	// Set new position
	vecSrc = tr.end;

	return result;
}

bool CCSGrenadeHint::CheckDetonate(vec3_t& vecThrow, const trace_t& tr, int tick, float interval)
{
	firegrenade_didnt_hit = false;
	switch (type)
	{
	case class_ids::csmokegrenade:
	case class_ids::cdecoygrenade:
		// Velocity must be <0.1, this is only checked every 0.2s
		if (vecThrow.length() < 0.1f)
		{
			int det_tick_mod = static_cast<int>(0.2f / interval);
			return !(tick % det_tick_mod);
		}
		return false;

		/* TIMES AREN'T COMPLETELY RIGHT FROM WHAT I'VE SEEN ! ! ! */
	case class_ids::cmolotovgrenade:
	case class_ids::cincendiarygrenade:
		// Detonate when hitting the floor
		if (tr.flFraction != 1.0f && tr.plane.normal.z > 0.7f)
			return true;
		// OR we've been flying for too long

	case class_ids::cflashbang:
	case class_ids::chegrenade: {
		// Pure timer based, detonate at 1.5s, checked every 0.2s
		firegrenade_didnt_hit = static_cast<float>(tick) * interval > 1.5f && !(tick % static_cast<int>(0.2f / interval));
		return firegrenade_didnt_hit;
	}
	default:
		//Assert(false);
		return false;
	}
}

void CCSGrenadeHint::TraceHull(vec3_t& src, vec3_t& end, trace_t& tr)
{
	// Setup grenade hull
	static const vec3_t hull[2] = { vec3_t(-2.0f, -2.0f, -2.0f), vec3_t(2.0f, 2.0f, 2.0f) };

	trace_filter filter;
	//filter.SetIgnoreClass("BaseCSGrenadeProjectile");
	filter.skip = interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player());

	ray_t ray;
	ray.initialize(src, end, hull[0], hull[1]);

	const unsigned int mask = 0x200400B;
	interfaces::trace_ray->trace_ray(ray, mask, &filter, &tr);
}

void CCSGrenadeHint::AddGravityMove(vec3_t& move, vec3_t& vel, float frametime, bool onground)
{
	vec3_t basevel(0.0f, 0.0f, 0.0f);

	move.x = (vel.x + basevel.x) * frametime;
	move.y = (vel.y + basevel.y) * frametime;

	if (onground)
	{
		move.z = (vel.z + basevel.z) * frametime;
	}
	else
	{
		// Game calls GetActualGravity( this );
		float gravity = 800.0f * 0.4f;

		float newZ = vel.z - (gravity * frametime);
		move.z = ((vel.z + newZ) / 2.0f + basevel.z) * frametime;

		vel.z = newZ;
	}
}

void CCSGrenadeHint::PushEntity(vec3_t& src, const vec3_t& move, trace_t& tr)
{
	vec3_t vecAbsEnd = src;
	vecAbsEnd += move;

	// Trace through world
	TraceHull(src, vecAbsEnd, tr);
}

void CCSGrenadeHint::ResolveFlyCollisionCustom(trace_t& tr, vec3_t& vecVelocity, float interval)
{
	// Calculate elasticity
	float flSurfaceElasticity = 1.0;  // Assume all surfaces have the same elasticity
	float flGrenadeElasticity = 0.45f; // GetGrenadeElasticity()
	float flTotalElasticity = flGrenadeElasticity * flSurfaceElasticity;
	if (flTotalElasticity > 0.9f) flTotalElasticity = 0.9f;
	if (flTotalElasticity < 0.0f) flTotalElasticity = 0.0f;

	// Calculate bounce
	vec3_t vecAbsVelocity;
	PhysicsClipVelocity(vecVelocity, tr.plane.normal, vecAbsVelocity, 2.0f);
	vecAbsVelocity *= flTotalElasticity;

	// Stop completely once we move too slow
	float flSpeedSqr = vecAbsVelocity.length_sqr();
	static const float flMinSpeedSqr = 20.0f * 20.0f; // 30.0f * 30.0f in CSS
	if (flSpeedSqr < flMinSpeedSqr)
	{
		vecAbsVelocity = vec3_t(0, 0, 0);
	}

	// Stop if on ground
	if (tr.plane.normal.z > 0.7f)
	{
		vecVelocity = vecAbsVelocity;
		vecAbsVelocity *= ((1.0f - tr.flFraction) * interval);
		PushEntity(tr.end, vecAbsVelocity, tr);
	}
	else
	{
		vecVelocity = vecAbsVelocity;
	}
}

int CCSGrenadeHint::PhysicsClipVelocity(const vec3_t& in, const vec3_t& normal, vec3_t& out, float overbounce)
{
	static const float STOP_EPSILON = 0.1f;

	float    backoff;
	float    change;
	float    angle;
	int        i, blocked;

	blocked = 0;

	angle = normal[2];

	if (angle > 0)
	{
		blocked |= 1;        // floor
	}
	if (!angle)
	{
		blocked |= 2;        // step
	}

	backoff = in.Dot(normal) * overbounce;

	for (i = 0; i < 3; i++)
	{
		change = normal[i] * backoff;
		out[i] = in[i] - change;
		if (out[i] > -STOP_EPSILON && out[i] < STOP_EPSILON)
		{
			out[i] = 0;
		}
	}

	return blocked;
}