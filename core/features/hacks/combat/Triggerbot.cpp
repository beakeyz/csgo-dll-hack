#include "../../features.hpp"
#include "../../../menu/variables.hpp"

void combat::triggerbot::triggerbot(c_usercmd* cmd)
{
    if (!csgo::local_player || csgo::local_player->health() < 0 || csgo::local_player->next_attack() > interfaces::globals->realtime || csgo::local_player->is_defusing() || !combat::triggerbot::enabled)
        return;

    const auto activeWeapon = csgo::local_player->active_weapon();
    if (!activeWeapon || !activeWeapon->clip1_count() || activeWeapon->next_primary_attack() > interfaces::globals->realtime)
        return;

    if (csgo::local_player->shots_fired() > 0)
        return;


    static auto lastTime = 0.0f;
    static auto lastContact = 0.0f;

    const auto now = interfaces::globals->realtime;

   // if (now - lastContact < config->triggerbot[weaponIndex].burstTime) {
     //   cmd->buttons |= in_attack;
    //    return;
    //}
    lastContact = 0.0f;


    if (now - lastTime < combat::triggerbot::delay)
        return;

    //if (!cfg.ignoreFlash && localPlayer->isFlashed())
    //    return;

    //if (cfg.scopedOnly && activeWeapon->isSniperRifle() && !localPlayer->isScoped())
    //    return;

    const auto weaponData = activeWeapon->get_weapon_data();
    if (!weaponData)
        return;

    auto startPos = csgo::local_player->get_eye_pos();
    auto endPos = startPos + vec3_t::fromAngle(cmd->viewangles + csgo::local_player->aim_punch_angle()) * weaponData->weapon_range;

    //if (!cfg.ignoreSmoke && memory->lineGoesThroughSmoke(startPos, endPos, 1))
    //    return;

    trace_t* trace = new trace_t();
    trace_filter_one_entity* filter = new trace_filter_one_entity();
    interfaces::trace_ray->trace_ray(ray_t(startPos, endPos), 0x46004009, filter, trace);

    lastTime = now;

    if (!trace->entity || !trace->entity->is_player())
        return;

    //if (!cfg.friendlyFire && !localPlayer->isOtherEnemy(trace->entity))
    //    return;

    if (trace->entity->dormant())
        return;

    //if (damage >= (cfg.killshot ? trace.entity->health() : cfg.minDamage)) {
    cmd->buttons &= ~in_attack;
    lastTime = 0.0f;
    lastContact = now;
    delete trace;
    delete filter;
   // }
}