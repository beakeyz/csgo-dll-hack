#include "../utilities/csgo.hpp"

float math::clamp(float in, float low, float high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}

float math::rad_to_deg(float x) {
	return (float)(x) * (float)(180.f / M_PI_F);
}

float math::deg_to_rad(float x) {
	return (float)(x) * (float)(M_PI_F / 180.f);
}

float math::normalize_angle(float angle) {
	if (angle > 180.f || angle < -180.f) {
		auto revolutions = std::round(std::abs(angle / 360.f));

		if (angle < 0.f)
			angle = angle + 360.f * revolutions;
		else
			angle = angle - 360.f * revolutions;
	}

	return angle;
}

float math::angle_difference(float destAngle, float srcAngle)
{
	float delta;

	delta = fmodf(destAngle - srcAngle, 360.0f);
	if (destAngle > srcAngle)
	{
		if (delta >= 180)
			delta -= 360;
	}
	else
	{
		if (delta <= -180)
			delta += 360;
	}
	return delta;
}

//aimtux
void math::correct_movement(vec3_t old_angles, c_usercmd* cmd, float old_forwardmove, float old_sidemove) {
	float delta_view;
	float f1;
	float f2;

	if (old_angles.y < 0.f)
		f1 = 360.0f + old_angles.y;
	else
		f1 = old_angles.y;

	if (cmd->viewangles.y < 0.0f)
		f2 = 360.0f + cmd->viewangles.y;
	else
		f2 = cmd->viewangles.y;

	if (f2 < f1)
		delta_view = abs(f2 - f1);
	else
		delta_view = 360.0f - abs(f1 - f2);

	delta_view = 360.0f - delta_view;

	cmd->forwardmove = cos(DEG2RAD(delta_view)) * old_forwardmove + cos(DEG2RAD(delta_view + 90.f)) * old_sidemove;
	cmd->sidemove = sin(DEG2RAD(delta_view)) * old_forwardmove + sin(DEG2RAD(delta_view + 90.f)) * old_sidemove;
}

void math::CorrectMovement(vec3_t vOldAngles, c_usercmd* pCmd, float fOldForward, float fOldSidemove)
{
	float deltaView = pCmd->viewangles.y - vOldAngles.y;
	float f1;
	float f2;

	if (vOldAngles.y < 0.f)
	{
		f1 = 360.0f + vOldAngles.y;
	}
	else
	{
		f1 = vOldAngles.y;
	}

	if (pCmd->viewangles.y < 0.0f)
	{
		f2 = 360.0f + pCmd->viewangles.y;
	}
	else
	{
		f2 = pCmd->viewangles.y;
	}

	if (f2 < f1)
	{
		deltaView = abs(f2 - f1);
	}
	else
	{
		deltaView = 360.0f - abs(f1 - f2);
	}
	deltaView = 360.0f - deltaView;

	const auto movetype = csgo::local_player->move_type();

	if (movetype != movetype_ladder) {
		pCmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
		pCmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;

		//pCmd->buttons &= ~in_moveright;
		//pCmd->buttons &= ~in_moveright;
		//pCmd->buttons &= ~in_forward;
		//pCmd->buttons &= ~in_back;
	}
}

vec3_t math::calculate_angle(vec3_t& a, vec3_t& b) {
	vec3_t angles;
	vec3_t delta;
	delta.x = (a.x - b.x);
	delta.y = (a.y - b.y);
	delta.z = (a.z - b.z);

	double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);
	angles.x = (float)(atanf(delta.z / hyp) * 57.295779513082f);
	angles.y = (float)(atanf(delta.y / delta.x) * 57.295779513082f);

	angles.z = 0.0f;
	if (delta.x >= 0.0) { angles.y += 180.0f; }
	return angles;
}

void math::sin_cos(float r, float* s, float* c) {
	*s = sin(r);
	*c = cos(r);
}

vec3_t math::angle_vector(vec3_t angle) {
	auto sy = sin(angle.y / 180.f * static_cast<float>(M_PI));
	auto cy = cos(angle.y / 180.f * static_cast<float>(M_PI));

	auto sp = sin(angle.x / 180.f * static_cast<float>(M_PI));
	auto cp = cos(angle.x / 180.f * static_cast<float>(M_PI));

	return vec3_t(cp * cy, cp * sy, -sp);
}

void math::transform_vector(vec3_t & a, matrix_t & b, vec3_t & out) {
	out.x = a.dot(b.mat_val[0]) + b.mat_val[0][3];
	out.y = a.dot(b.mat_val[1]) + b.mat_val[1][3];
	out.z = a.dot(b.mat_val[2]) + b.mat_val[2][3];
}

void math::vector_transform(const vec3_t& in1, const matrix_t& in2, vec3_t& out)
{
	out = vec3_t((in1.Dot(vec3_t(in2[0][0], in2[0][1], in2[0][2]))) + in2[0][3], (in1.Dot(vec3_t(in2[1][0], in2[1][1], in2[1][2]))) + in2[1][3], in1.Dot(vec3_t(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3]);
}

vec3_t math::vector_transform(const vec3_t& in1, const matrix_t& in2)
{
	vec3_t out = vec3_t((in1.Dot(vec3_t(in2[0][0], in2[0][1], in2[0][2]))) + in2[0][3], (in1.Dot(vec3_t(in2[1][0], in2[1][1], in2[1][2]))) + in2[1][3], in1.Dot(vec3_t(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3]);
	return out;
}
float math::normalize_yaw(float f) {
	while (f < -180.0f)
		f += 360.0f;

	while (f > 180.0f)
		f -= 360.0f;

	return f;
}

float math::normalize_pitch(float pitch)
{
	while (pitch > 89.0f)
		pitch -= 180.0f;

	while (pitch < -89.0f)
		pitch += 180.0f;

	return pitch;
}

void math::vector_angles(const vec3_t & forward, vec3_t & angles) {
	if (forward.y == 0.0f && forward.x == 0.0f) {
		angles.x = (forward.z > 0.0f) ? 270.0f : 90.0f;
		angles.y = 0.0f;
	}
	else {
		angles.x = atan2(-forward.z, vec2_t(forward).length()) * -180 / static_cast<float>(M_PI);
		angles.y = atan2(forward.y, forward.x) * 180 / static_cast<float>(M_PI);

		if (angles.y > 90)
			angles.y -= 180;
		else if (angles.y < 90)
			angles.y += 180;
		else if (angles.y == 90)
			angles.y = 0;
	}

	angles.z = 0.0f;
}

void math::angle_to_vector(const vec3_t& angles, vec3_t& forward) {
	float sp, sy, cp, cy;

	sin_cos(deg_to_rad(angles.y), &sy, &cy);
	sin_cos(deg_to_rad(angles.x), &sp, &cp);

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}

void math::angle_vectors(const vec3_t & angles, vec3_t& forward, vec3_t& right, vec3_t& up) {
	float sp, sy, sr, cp, cy, cr;

	sin_cos(DEG2RAD(angles.x), &sp, &cp);
	sin_cos(DEG2RAD(angles.y), &sy, &cy);
	sin_cos(DEG2RAD(angles.z), &sr, &cr);

	forward.x = (cp * cy);
	forward.y = (cp * sy);
	forward.z = (-sp);
	right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
	right.y = (-1 * sr * sp * sy + -1 * cr * cy);
	right.z = (-1 * sr * cp);
	up.x = (cr * sp * cy + -sr * -sy);
	up.y = (cr * sp * sy + -sr * cy);
	up.z = (cr * cp);
}

vec3_t math::vector_add(vec3_t & a, vec3_t & b) {
	return vec3_t(a.x + b.x,
		a.y + b.y,
		a.z + b.z);
}

vec3_t math::vector_subtract(vec3_t & a, vec3_t & b) {
	return vec3_t(a.x - b.x,
		a.y - b.y,
		a.z - b.z);
}

vec3_t math::vector_multiply(vec3_t & a, vec3_t & b) {
	return vec3_t(a.x * b.x,
		a.y * b.y,
		a.z * b.z);
}

vec3_t math::vector_divide(vec3_t & a, vec3_t & b) {
	return vec3_t(a.x / b.x,
		a.y / b.y,
		a.z / b.z);
}

bool math::screen_transform(const vec3_t & point, vec3_t & screen) {
	auto matrix = interfaces::engine->world_to_screen_matrix();

	float w = matrix[3][0] * point.x + matrix[3][1] * point.y + matrix[3][2] * point.z + matrix[3][3];
	screen.x = matrix[0][0] * point.x + matrix[0][1] * point.y + matrix[0][2] * point.z + matrix[0][3];
	screen.y = matrix[1][0] * point.x + matrix[1][1] * point.y + matrix[1][2] * point.z + matrix[1][3];
	screen.z = 0.0f;

	int inverse_width = static_cast<int>((w < 0.001f) ? -1.0f / w :
		1.0f / w);

	screen.x *= inverse_width;
	screen.y *= inverse_width;
	return (w < 0.001f);
}

/*
bool math::world_to_screen(const vec3_t & origin, vec2_t & screen) {
	static std::uintptr_t view_matrix;
	if ( !view_matrix )
		view_matrix = *reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( utilities::pattern_scan( "client.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9" ) ) + 3 ) + 176;

	const auto& matrix = *reinterpret_cast< view_matrix_t* >( view_matrix );

	const auto w = matrix.m[ 3 ][ 0 ] * origin.x + matrix.m[ 3 ][ 1 ] * origin.y + matrix.m[ 3 ][ 2 ] * origin.z + matrix.m[ 3 ][ 3 ];
	if ( w < 0.001f )
		return false;

	int x, y;
	interfaces::engine->get_screen_size( x, y );
	
	screen.x = static_cast<float>(x) / 2.0f;
	screen.y = static_cast<float>(y) / 2.0f;

	screen.x *= 1.0f + ( matrix.m[ 0 ][ 0 ] * origin.x + matrix.m[ 0 ][ 1 ] * origin.y + matrix.m[ 0 ][ 2 ] * origin.z + matrix.m[ 0 ][ 3 ] ) / w;
	screen.y *= 1.0f - ( matrix.m[ 1 ][ 0 ] * origin.x + matrix.m[ 1 ][ 1 ] * origin.y + matrix.m[ 1 ][ 2 ] * origin.z + matrix.m[ 1 ][ 3 ] ) / w;

	return true;
}
*/

bool math::world_to_screen(const vec3_t& origin, vec3_t& screen)
{
	//crashes
	//return !interfaces::debug_overlay->screen_position(origin, screen);
	return !interfaces::debug_overlay->ScreenPosition(origin, screen);
}

vec3_t math::calc_angle(vec3_t v1, vec3_t v2) {
	vec3_t delta = v1 - v2;

	float len = delta.length();

	if (delta.z == 0.0f && len == 0.0f)
		return vec3_t{};

	if (delta.y == 0.0f && delta.x == 0.0f)
		return vec3_t{};

	vec3_t angles;
	angles.x = (asinf(delta.z / delta.length()) * 57.295779513082f);

	angles.y = (atanf(delta.y / delta.x) * 57.295779513082f);

	angles.z = 0.0f;
	if (delta.x >= 0.0f) { angles.y += 180.0f; }

	angles.clamp();

	return angles;
}

float math::random_float(float min, float max) {
	static auto o_random_float = reinterpret_cast<float(*)(float, float)>(GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomFloat"));
	return o_random_float(min, max);
}
