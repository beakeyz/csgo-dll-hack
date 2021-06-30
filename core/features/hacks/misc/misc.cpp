#include "../../features.hpp"
#include <stdlib.h>
#include "../../../menu/variables.hpp"

void combat::resolver::resolver() {
	if (!combat::resolver::enabled)
		return;

	for (int i = 0; i < interfaces::globals->max_clients; ++i)
	{
		auto player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (player && player != csgo::local_player)
		{
			player->eye_angles().y = player->lower_body_yaw();
			player->eye_angles().normalize();
			player->eye_angles().clamp();
		}
	}

}
