#pragma once
#include "../../source-sdk/math/vector3d.hpp"

class glow_object_definition_t {
public:
	void set(color& color, bool render_when_occluded, bool render_when_unoccluded, int style = 0)
	{
		this->m_vecClr = vec3_t(color.r / 255.f, color.g / 255.f, color.b / 255.f);
		this->m_flAlpha = (float)color.a / 255.f;

		this->m_bRenderWhenOccluded = render_when_occluded;
		this->m_bRenderWhenUnoccluded = render_when_unoccluded;
		this->m_nGlowStyle = style;
	}

	void color(color& color)
	{
		this->m_vecClr = vec3_t(color.r / 255.f, color.g / 255.f, color.b / 255.f);
		this->m_flAlpha = (float)color.a / 255.f;
	}

	void glow_style(int style)
	{
		this->m_nGlowStyle = style;
	}

	void full_bloom_render(bool value)
	{
		this->m_bFullBloomRender = value;
	}

	void render_when_occluded(bool value)
	{
		this->m_bRenderWhenOccluded = value;
	}

	void render_when_unoccluded(bool value)
	{
		this->m_bRenderWhenUnoccluded = value;
	}

	bool unused()
	{
		return m_nNextFreeSlot != -2;
	}

	int   m_nNextFreeSlot;
	void* m_pEntity;
	vec3_t m_vecClr;
	float m_flAlpha;
	bool  m_bGlowAlphaCappedByRenderAlpha;
	float m_flGlowAlphaFunctionOfMaxVelocity;
	float m_flGlowAlphaMax;
	float m_flGlowPulseOverdrive;
	bool  m_bRenderWhenOccluded;
	bool  m_bRenderWhenUnoccluded;
	bool  m_bFullBloomRender;
	int   m_nFullBloomStencilTestValue;
	int   m_nGlowStyle;
	int   m_nSplitScreenSlot;
};

class glow_manager_t {
public:
	glow_object_definition_t* objects;
	char pad[8];
	int size;
};