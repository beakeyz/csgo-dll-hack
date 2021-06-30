#pragma once
#include "../../features.hpp"
#include "../../../menu/variables.hpp"

enum resolve_mode_e {
	RESOLVE_MODE_NONE,
	RESOLVE_MODE_LBYT
};

struct resolver_data_t
{
	resolver_data_t() { }

	resolve_mode_e m_resolve_mode;
	float m_resolved_yaw;
};

extern resolver_data_t m_resolver_data[65];