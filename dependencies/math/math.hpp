#pragma once

namespace math {
	void correct_movement(vec3_t old_angles, c_usercmd* cmd, float old_forwardmove, float old_sidemove);
	void CorrectMovement(vec3_t vOldAngles, c_usercmd* pCmd, float fOldForward, float fOldSidemove);
	vec3_t calculate_angle(vec3_t& a, vec3_t& b);
	void sin_cos(float r, float* s, float* c);
	vec3_t angle_vector(vec3_t angle);
	void transform_vector(vec3_t&, matrix_t&, vec3_t&);
	void vector_angles(const vec3_t&, vec3_t&);
	void angle_vectors(const vec3_t&, vec3_t*, vec3_t*, vec3_t*);
	vec3_t vector_add(vec3_t&, vec3_t&);
	vec3_t vector_subtract(vec3_t&, vec3_t&);
	vec3_t vector_multiply(vec3_t&, vec3_t&);
	vec3_t vector_divide(vec3_t&, vec3_t&);
	bool screen_transform(const vec3_t& point, vec3_t& screen);
	//bool world_to_screen(const vec3_t& origin, vec2_t& screen);
	bool world_to_screen(const vec3_t& origin, vec3_t& screen);
	float rad_to_deg(float x);
	float deg_to_rad(float x);
	float normalize_angle(float angle);
	float random_float(float min, float max);
	vec3_t calc_angle(vec3_t v1, vec3_t v2);
	void angle_to_vector(const vec3_t& angles, vec3_t& forward);
	float clamp(float in, float low, float high);
	
};
