#pragma once
#include <cstdint>
#include "../../source-sdk/math/vector3d.hpp"

class entity_t;
class c_usercmd;

struct Box
{
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
};

struct HPInfo
{
	int hp = -1;
	int hp_difference = 0;
	float hp_difference_time = 0.0f;
};

namespace utilities {
	template< typename t >
	__forceinline static t call_virtual( void *name, int offset ) {
		int *table = *( int ** ) name;
		int address = table[ offset ];
		return ( t ) ( address );
	}

	std::uint8_t* pattern_scan(const char* module_name, const char* signature) noexcept;

	//TODO: add more game-related utils (boundingbox function, among others)

	bool get_bbox(entity_t* e, Box& b, bool player_esp);
	void movement_fix(c_usercmd* cmd);
	void rotate_movement(c_usercmd* cmd, float yaw);

	//void trace_line(Vector& start, Vector& end, unsigned int mask, CTraceFilter* filter, CGameTrace* tr);


}
