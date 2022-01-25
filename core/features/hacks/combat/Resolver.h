#pragma once
#include "../../features.hpp"
#include "../../../menu/variables.hpp"
#include "../../module.h"
#include <deque>

class player_t;

struct ResolveInfo
{
	ResolveInfo()
	{
		m_bActive = false;

		m_flVelocity = 0.f;
		m_vecVelocity = vec3_t(0, 0, 0);
		m_angEyeAngles = vec3_t(0, 0, 0);
		m_flSimulationTime = 0.f;
		m_flLowerBodyYawTarget = 0.f;

		m_flStandingTime = 0.f;
		m_flMovingLBY = 0.f;
		m_flLbyDelta = 180.f;
		m_bIsMoving = false;

		m_angDirectionFirstMoving = vec3_t(0, 0, 0);
		m_nCorrectedFakewalkIdx = 0;
	}

	void SaveRecord(player_t* player)
	{
		m_flLowerBodyYawTarget = player->lower_body_yaw();
		m_flSimulationTime = player->simulation_time();
		m_flVelocity = player->velocity().length_2d();
		m_vecVelocity = player->velocity();
		m_angEyeAngles = player->eye_angles();

		m_iLayerCount = player->GetNumAnimOverlays();
		for (int i = 0; i < m_iLayerCount; i++)
			animationLayer[i] = player->GetAnimOverlays()[i];
	}

	bool operator==(const ResolveInfo& other)
	{
		return other.m_flSimulationTime == m_flSimulationTime;
	}

	bool m_bActive;

	float_t m_flVelocity;
	vec3_t m_vecVelocity;
	vec3_t m_angEyeAngles;
	float_t m_flSimulationTime;
	float_t m_flLowerBodyYawTarget;

	float_t m_iLayerCount = 0.F;
	AnimationLayer animationLayer[15];

	float_t m_flStandingTime;
	float_t m_flMovingLBY;
	float_t m_flLbyDelta;
	bool m_bIsMoving;

	vec3_t m_angDirectionFirstMoving;
	int32_t m_nCorrectedFakewalkIdx;

	int32_t m_nShotsMissed = 0;
};

class Resolver : public singleton<Resolver>
{

public:

	void Log();
	void Resolve();

	void Override();

	ResolveInfo arr_infos[65];

	bool is_enabled = false;

private:

	float_t ResolveFakewalk(player_t* player, ResolveInfo& curtickrecord);
	bool IsEntityMoving(player_t* player);
	bool IsAdjustingBalance(player_t* player, ResolveInfo& record, AnimationLayer* layer);
	bool IsAdjustingStopMoving(player_t* player, ResolveInfo& record, AnimationLayer* layer);
	bool IsFakewalking(player_t* player, ResolveInfo& record);

	const inline float_t GetDelta(float_t a, float_t b)
	{
		return fabsf(math::clamp(a - b, -180.F, 180.F));
	}

	const inline float_t LBYDelta(const ResolveInfo& v)
	{
		return v.m_angEyeAngles.y - v.m_flLowerBodyYawTarget;
	}
};