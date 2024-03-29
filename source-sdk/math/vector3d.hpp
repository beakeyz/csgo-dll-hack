#pragma once
#pragma warning( disable : 26451 )
#include <limits>
#include <cmath>
#include <algorithm>



inline float bits_to_float(std::uint32_t i) {
	union convertor_t {
		float f; unsigned long ul;
	} tmp;

	tmp.ul = i;
	return tmp.f;
}

constexpr double M_PI = 3.14159265358979323846;
constexpr float M_RADPI = 57.295779513082f;
constexpr float M_PI_F = static_cast<float>(M_PI);
constexpr auto radiansToDegrees = [](float radians) constexpr noexcept { return radians * 180.0f / M_PI_F; };

constexpr float RAD2DEG(const float x) {
	return (float)(x) * (float)(180.f / M_PI_F);
}
constexpr float DEG2RAD(const float x) {
	return (float)(x) * (float)(M_PI_F / 180.f);
}

constexpr std::uint32_t FLOAT32_NAN_BITS = 0x7FC00000;
const float FLOAT32_NAN = bits_to_float(FLOAT32_NAN_BITS);
#define VEC_T_NAN FLOAT32_NAN
#define ASSERT( _exp ) ( (void ) 0 )

class vec3_t {
public:

	vec3_t();
	vec3_t(float, float, float);
	~vec3_t();

	float x, y, z;

	vec3_t& operator+=(const vec3_t& v) {
		x += v.x; y += v.y; z += v.z; return *this;
	}
	vec3_t& operator-=(const vec3_t& v) {
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}
	vec3_t& operator*=(float v) {
		x *= v; y *= v; z *= v; return *this;
	}
	vec3_t operator+(const vec3_t& v) {
		return vec3_t{ x + v.x, y + v.y, z + v.z };
	}
	vec3_t operator-(const vec3_t& v) {
		return vec3_t{ x - v.x, y - v.y, z - v.z };
	}
	vec3_t operator*(float fl) const {
		return vec3_t(x * fl, y * fl, z * fl);
	}
	vec3_t operator*(const vec3_t& v) const {
		return vec3_t(x * v.x, y * v.y, z * v.z);
	}
	vec3_t& operator/=(float fl) {
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}
	bool operator==(const vec3_t& src) const
	{
		return (src.x == x) && (src.y == y) && (src.z == z);
	}

	bool operator!=(const vec3_t& src) const
	{
		return (src.x != x) || (src.y != y) || (src.z != z);
	}
	auto operator-(const vec3_t& other) const -> vec3_t {
		auto buf = *this;

		buf.x -= other.x;
		buf.y -= other.y;
		buf.z -= other.z;

		return buf;
	}

	auto operator/(float other) const {
		vec3_t vec;
		vec.x = x / other;
		vec.y = y / other;
		vec.z = z / other;
		return vec;
	}

	float& operator[](int i) {
		return ((float*)this)[i];
	}
	float operator[](int i) const {
		return ((float*)this)[i];
	}
	inline void MulAdd(const vec3_t& a, const vec3_t& b, float scalar) {
		x = a.x + b.x * scalar;
		y = a.y + b.y * scalar;
		z = a.z + b.z * scalar;
	}
	inline float length_2d() const {
		return sqrt((x * x) + (y * y));
	}
	void crossproduct(vec3_t v1, vec3_t v2, vec3_t cross_p) const {
		cross_p.x = (v1.y * v2.z) - (v1.z * v2.y); //i
		cross_p.y = -((v1.x * v2.z) - (v1.z * v2.x)); //j
		cross_p.z = (v1.x * v2.y) - (v1.y * v2.x); //k
	}

	vec3_t cross(const vec3_t& other) const {
		vec3_t res;
		crossproduct(*this, other, res);
		return res;
	}

	auto toAngle() const noexcept
	{
		return vec3_t{ radiansToDegrees(std::atan2(-z, std::hypot(x, y))),
					   radiansToDegrees(std::atan2(y, x)),
					   0.0f };
	}

	float DistTo(const vec3_t& vOther) const
	{
		vec3_t delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.length();
	}

	constexpr bool noNull() const noexcept {
		return x || y || z;
	}

	void init(float ix, float iy, float iz);
	void clamp();
	vec3_t clamped();
	vec3_t normalized();
	vec3_t calculateRelativeAngle(const vec3_t& source, const vec3_t& destination, const vec3_t& viewAngles) noexcept;
	float Dot(const vec3_t &vOther) const;
	float distance_to(const vec3_t& other);
	void normalize();
	float length();
	float length_sqr();
	float length_2d_sqr(void) const;
	float dot(const vec3_t other);
	float dot(const float* other);

	static auto fromAngle(const vec3_t& angle) noexcept
	{
		return vec3_t{ std::cos(DEG2RAD(angle.x)) * std::cos(DEG2RAD(angle.y)),
					   std::cos(DEG2RAD(angle.x)) * std::sin(DEG2RAD(angle.y)),
					  -std::sin(DEG2RAD(angle.x)) };
	}


private:

};

inline vec3_t operator*(float lhs, const vec3_t& rhs) {
	return vec3_t(rhs.x * lhs, rhs.x * lhs, rhs.x * lhs);
}

struct matrix_t {
	matrix_t() { }
	matrix_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23)
	{
		mat_val[0][0] = m00;	mat_val[0][1] = m01; mat_val[0][2] = m02; mat_val[0][3] = m03;
		mat_val[1][0] = m10;	mat_val[1][1] = m11; mat_val[1][2] = m12; mat_val[1][3] = m13;
		mat_val[2][0] = m20;	mat_val[2][1] = m21; mat_val[2][2] = m22; mat_val[2][3] = m23;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	void init(const vec3_t& x, const vec3_t& y, const vec3_t& z, const vec3_t& origin) {
		mat_val[0][0] = x.x; mat_val[0][1] = y.x; mat_val[0][2] = z.x; mat_val[0][3] = origin.x;
		mat_val[1][0] = x.y; mat_val[1][1] = y.y; mat_val[1][2] = z.y; mat_val[1][3] = origin.y;
		mat_val[2][0] = x.z; mat_val[2][1] = y.z; mat_val[2][2] = z.z; mat_val[2][3] = origin.z;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	matrix_t(const vec3_t& x, const vec3_t& y, const vec3_t& z, const vec3_t& origin) {
		init(x, y, z, origin);
	}

	inline void set_origin(vec3_t const& p) {
		mat_val[0][3] = p.x;
		mat_val[1][3] = p.y;
		mat_val[2][3] = p.z;
	}

	inline vec3_t getOrigin() {
		return vec3_t{ mat_val[0][3], mat_val[1][3], mat_val[2][3] };
	}

	inline void invalidate(void) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 4; j++) {
				mat_val[i][j] = VEC_T_NAN;
			}
		}
	}

	float* operator[](int i) { ASSERT((i >= 0) && (i < 3)); return mat_val[i]; }
	const float* operator[](int i) const { ASSERT((i >= 0) && (i < 3)); return mat_val[i]; }
	float* base() { return &mat_val[0][0]; }
	const float* base() const { return &mat_val[0][0]; }

	float mat_val[3][4];
};
