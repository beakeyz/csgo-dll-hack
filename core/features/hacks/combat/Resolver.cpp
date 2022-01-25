#include "./Resolver.h"

void Resolver::Log()
{
	if (!csgo::local_player->is_alive()) {
		return;
	}

	for (int i = 1; i <= interfaces::globals->maxClients; i++)
	{
		auto& record = arr_infos[i];

		player_t* player = static_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!player || !player->is_alive() || player->team() == csgo::local_player->team())
		{
			record.m_bActive = false;
			continue;
		}

		if (player->dormant())
			continue;

		if (record.m_flSimulationTime == player->simulation_time())
			continue;

		record.SaveRecord(player);
		record.m_bActive = true;
	}
}

void Resolver::Resolve()
{
	if (!csgo::local_player->is_alive()) {
		return;
	}

	for (int i = 1; i <= interfaces::globals->maxClients; i++)
	{
		auto& record = arr_infos[i];
		if (!record.m_bActive)
			continue;

		player_t* player = static_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!player || !player->is_alive() || player->dormant() || player == csgo::local_player)
			continue;

		if (record.m_flVelocity == 0.f && player->velocity().length_2d() != 0.f)
		{
			math::vector_angles(player->velocity(), record.m_angDirectionFirstMoving);
			record.m_nCorrectedFakewalkIdx = 0;
		}

		auto firedShots = csgo::local_player->shots_fired();

		/*
		if (g_Options.debug_fliponkey)
		{
			float_t new_yaw = player->m_flLowerBodyYawTarget();
			if (g_InputSystem->IsButtonDown(g_Options.debug_flipkey))
				new_yaw += 180.f;
			new_yaw = Math::ClampYaw(new_yaw);
			player->m_angEyeAngles().yaw = new_yaw;
			return;
		}
		*/

		/*
		if (g_Options.hvh_resolver_override && g_InputSystem->IsButtonDown(g_Options.hvh_resolver_override_key))
		{
			Override(); //needs an improvement sometimes fucked up xD

			Global::resolverModes[player->EntIndex()] = "Overriding";

			return;
		}
		*/

		
		AnimationLayer curBalanceLayer, prevBalanceLayer;

		ResolveInfo curtickrecord;
		curtickrecord.SaveRecord(player);

		
		if ((player->flags() & fl_onground) && (IsFakewalking(player, curtickrecord) || (player->velocity().length_2d() > 0.1f && player->velocity().length_2d() < 45.f && !(player->flags() & fl_ducking)))) //Fakewalk, shiftwalk check // We have to rework the fakewalk resolving, it sucks :D
		{
			float_t new_yaw = ResolveFakewalk(player, curtickrecord);
			new_yaw = math::clamp(new_yaw, -180, 180);

			player->eye_angles().y = new_yaw;

			//Global::resolverModes[player->EntIndex()] = "Fakewalking";

			continue;
		}
		
		if (IsEntityMoving(player))
		{
			float_t new_yaw = player->lower_body_yaw();
			new_yaw = math::clamp(new_yaw, -180, 180);

			player->eye_angles().y = new_yaw;

			record.m_flStandingTime = player->simulation_time();
			record.m_flMovingLBY = player->lower_body_yaw();
			record.m_bIsMoving = true;

			//Global::resolverModes[player->EntIndex()] = "Moving";

			continue;
		}
		
		if (IsAdjustingBalance(player, curtickrecord, &curBalanceLayer))
		{
			if (fabsf(LBYDelta(curtickrecord)) > 35.f)
			{
				float_t new_yaw = player->lower_body_yaw() + record.m_flLbyDelta;
				new_yaw = math::clamp(new_yaw, -180, 180);

				player->eye_angles().y = new_yaw;

				//Global::resolverModes[player->EntIndex()] = "Fakehead (delta > 35)";
			}
			if (IsAdjustingBalance(player, record, &prevBalanceLayer))
			{
				if ((prevBalanceLayer.m_flCycle != curBalanceLayer.m_flCycle) || curBalanceLayer.m_flWeight == 1.f)
				{
					float
						flAnimTime = curBalanceLayer.m_flCycle,
						flSimTime = player->simulation_time();

					//if (flAnimTime < 0.01f && prevBalanceLayer.m_flCycle > 0.01f && g_Options.rage_lagcompensation && CMBacktracking::Get().IsTickValid(TIME_TO_TICKS(flSimTime - flAnimTime)))
					//{
					//	CMBacktracking::Get().SetOverwriteTick(player, QAngle(player->m_angEyeAngles().pitch, player->m_flLowerBodyYawTarget(), 0), (flSimTime - flAnimTime), 2);
					//}

					float_t new_yaw = player->lower_body_yaw();
					new_yaw = math::clamp(new_yaw, -180, 180);

					player->eye_angles().y = new_yaw;

					//Global::resolverModes[player->EntIndex()] = "Breaking LBY";

					continue;
				}
				else if (curBalanceLayer.m_flWeight == 0.f && (prevBalanceLayer.m_flCycle > 0.92f && curBalanceLayer.m_flCycle > 0.92f)) // breaking lby with delta < 120
				{
					if (player->simulation_time() >= record.m_flStandingTime + 0.22f && record.m_bIsMoving)
					{
						record.m_flLbyDelta = record.m_flLowerBodyYawTarget - player->lower_body_yaw();

						float_t new_yaw = player->lower_body_yaw() + record.m_flLbyDelta;
						new_yaw = math::clamp(new_yaw, -180, 180);

						player->eye_angles().y = new_yaw;

						record.m_bIsMoving = false;

						//Global::resolverModes[player->EntIndex()] = "Breaking LBY (delta < 120)";

						continue;
					}

					if (player->simulation_time() >= record.m_flStandingTime + 1.32f && std::fabsf(record.m_flLbyDelta) < 35.f)
					{
						record.m_flLbyDelta = record.m_flMovingLBY - player->lower_body_yaw();
						float_t new_yaw = player->lower_body_yaw() + record.m_flLbyDelta;
						new_yaw = math::clamp(new_yaw, -180, 180);

						player->eye_angles().y = new_yaw;

						record.m_bIsMoving = false;

						//Global::resolverModes[player->EntIndex()] = "LBY delta < 35";

						continue;
					}
				}
			}
			else
			{
				float_t new_yaw = player->lower_body_yaw();
				new_yaw = math::clamp(new_yaw, -180, 180);

				player->eye_angles().y = new_yaw;

				//Global::resolverModes[player->EntIndex()] = "Other";

				continue;
			}
		}
		/*
		if (player->simulation_time() >= record.m_flStandingTime + 0.22f && record.m_bIsMoving)
		{
			record.m_flLbyDelta = record.m_flLowerBodyYawTarget - player->lower_body_yaw();

			float_t new_yaw = player->lower_body_yaw() + record.m_flLbyDelta;
			new_yaw = math::clamp(new_yaw, -180, 180);

			player->eye_angles().y = new_yaw;

			record.m_bIsMoving = false;

			//Global::resolverModes[player->EntIndex()] = "Breaking LBY (delta < 120)";

			continue;
		}
		if (player->simulation_time() >= record.m_flStandingTime + 1.32f && std::fabsf(record.m_flLbyDelta) < 35.f)
		{
			record.m_flLbyDelta = record.m_flMovingLBY - player->lower_body_yaw();
			float_t new_yaw = player->lower_body_yaw() + record.m_flLbyDelta;
			new_yaw = math::clamp(new_yaw, -180, 180);

			player->eye_angles().y= new_yaw;

			record.m_bIsMoving = false;

			//Global::resolverModes[player->EntIndex()] = "LBY delta < 35";

			continue;
		}

		*/
		float_t new_yaw = player->lower_body_yaw() + record.m_flLbyDelta;
		new_yaw = math::clamp(new_yaw, -180, 180);

		player->eye_angles().y = new_yaw;

	}

	
}

void Resolver::Override()
{
	//if (!g_Options.hvh_resolver_override)
	//	return;

	//if (!g_InputSystem->IsButtonDown(g_Options.hvh_resolver_override_key))
	//	return;

	int w, h, cx, cy;

	interfaces::engine->get_screen_size(w, h);

	cx = w / 2;
	cy = h / 2;

	vec3_t crosshair = vec3_t(cx, cy, 0);

	player_t* nearest_player = nullptr;
	float bestFoV = 0;
	vec3_t bestHead2D;

	for (int i = 1; i <= interfaces::globals->maxClients; i++) //0 is always the world entity
	{
		player_t* player = static_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		//if (!CMBacktracking::Get().IsPlayerValid(player)) //ghetto
		//	continue;

		vec3_t headPos3D = player->getBonePos(8), headPos2D;

		if (!math::world_to_screen(headPos3D, headPos2D))
			continue;

		float FoV2D = crosshair.DistTo(headPos2D);

		if (!nearest_player || FoV2D < bestFoV)
		{
			nearest_player = player;
			bestFoV = FoV2D;
			bestHead2D = headPos2D;
		}
	}

	if (nearest_player) //use pointers and avoid double calling of GetClientEntity
	{
		int minX = cx - (w / 10), maxX = cx + (w / 10);

		if (bestHead2D.x < minX || bestHead2D.x > maxX)
			return;

		int totalWidth = maxX - minX;

		int playerX = bestHead2D.x - minX;

		int yawCorrection = -(((playerX * 360) / totalWidth) - 180);

		float_t new_yaw = yawCorrection;

		new_yaw = math::clamp(new_yaw, -180, 180);

		nearest_player->eye_angles().y += new_yaw;
	}
}

float_t Resolver::ResolveFakewalk(player_t* player, ResolveInfo& curtickrecord)
{
	auto& record = arr_infos[player->index()];

	float_t yaw;
	int32_t correctedFakewalkIdx = record.m_nCorrectedFakewalkIdx;

	if (correctedFakewalkIdx < 2)
	{
		yaw = record.m_angDirectionFirstMoving.y + 180.f;
	}
	else if (correctedFakewalkIdx < 4)
	{
		yaw = player->lower_body_yaw() + record.m_flLbyDelta;
	}
	else if (correctedFakewalkIdx < 6)
	{
		yaw = record.m_angDirectionFirstMoving.y;
	}
	else
	{
		vec3_t dir;
		math::vector_angles(curtickrecord.m_vecVelocity, dir);

		yaw = dir.y;
	}

	return yaw;
}

bool Resolver::IsEntityMoving(player_t* player)
{
	return (player->velocity().length_2d() > 0.1f && player->flags() & fl_onground);
}

bool Resolver::IsAdjustingBalance(player_t* player, ResolveInfo& record, AnimationLayer* layer)
{
	for (int i = 0; i < record.m_iLayerCount; i++)
	{
		const int activity = player->GetSequenceActivity(record.animationLayer[i].m_nSequence);
		if (activity == 979)
		{
			*layer = record.animationLayer[i];
			return true;
		}
		Sleep(1);
	}
	return false;
}

bool Resolver::IsAdjustingStopMoving(player_t* player, ResolveInfo& record, AnimationLayer* layer)
{
	for (int i = 0; i < record.m_iLayerCount; i++)
	{
		const int activity = player->GetSequenceActivity(record.animationLayer[i].m_nSequence);
		if (activity == 980)
		{
			*layer = record.animationLayer[i];
			return true;
		}
	}
	return false;
}

bool Resolver::IsFakewalking(player_t* player, ResolveInfo& record)
{
	bool
		bFakewalking = false,
		stage1 = false,			// stages needed cause we are iterating all layers, eitherwise won't work :)
		stage2 = false,
		stage3 = false;

	for (int i = 0; i < record.m_iLayerCount; i++)
	{
		if (record.animationLayer[i].m_nSequence == 26 && record.animationLayer[i].m_flWeight < 0.4f)
			stage1 = true;
		if (record.animationLayer[i].m_nSequence == 7 && record.animationLayer[i].m_flWeight > 0.001f)
			stage2 = true;
		if (record.animationLayer[i].m_nSequence == 2 && record.animationLayer[i].m_flWeight == 0)
			stage3 = true;
	}

	if (stage1 && stage2)
		if (stage3 || (player->flags() & fl_ducking)) // since weight from stage3 can be 0 aswell when crouching, we need this kind of check, cause you can fakewalk while crouching, thats why it's nested under stage1 and stage2
			bFakewalking = true;
		else
			bFakewalking = false;
	else
		bFakewalking = false;

	return bFakewalking;
}
