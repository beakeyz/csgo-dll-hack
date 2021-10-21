#include "renderer.hpp"

unsigned long render::fonts::watermark_font;
unsigned long render::fonts::watermark_font_small;
unsigned long render::fonts::watermark_font_other;
unsigned long render::fonts::font_bigboi;

void render::initialize() {
	render::fonts::watermark_font = interfaces::surface->font_create();
	render::fonts::watermark_font_small = interfaces::surface->font_create();
	render::fonts::watermark_font_other = interfaces::surface->font_create();
	render::fonts::font_bigboi = interfaces::surface->font_create();

	interfaces::surface->set_font_glyph(render::fonts::watermark_font, "Andale Mono", 16, 500, 0, 0, font_flags::fontflag_dropshadow);
	interfaces::surface->set_font_glyph(render::fonts::watermark_font_small, "Andale Mono", 12, 500, 0, 0, font_flags::fontflag_dropshadow);
	interfaces::surface->set_font_glyph(render::fonts::watermark_font_other, "Andale Mono", 12, 250, 0, 0, font_flags::fontflag_outline);
	interfaces::surface->set_font_glyph(render::fonts::font_bigboi, "Andale Mono", 20, 500, 0, 0, font_flags::fontflag_antialias);


	console::log("[setup] render initialized!\n");
}

void render::draw_line(std::int32_t x1, std::int32_t y1, std::int32_t x2, std::int32_t y2, color colour) {
	interfaces::surface->set_drawing_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_line(x1, y1, x2, y2);
}

void render::text(std::int32_t x, std::int32_t y, unsigned long font, const wchar_t *text, bool centered, color color) {
	interfaces::surface->draw_text_font(font);
	int text_width, text_height;

	if (centered) {
		interfaces::surface->get_text_size(font, text, text_width, text_height);
		interfaces::surface->draw_text_pos(x - text_width / 2, y);
	}
	else
		interfaces::surface->draw_text_pos(x, y );

	interfaces::surface->set_text_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_render_text(text, wcslen(text));
}

void render::text(std::int32_t x, std::int32_t y, unsigned long font, std::string text, bool centered, color color) {
	wchar_t temp[128];
	int text_width, text_height;
	if (MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, temp, 128) > 0) {
		interfaces::surface->draw_text_font(font);
		if (centered) {
			interfaces::surface->get_text_size(font, temp, text_width, text_height);
			interfaces::surface->draw_text_pos(x - text_width / 2, y);
		}
		else
			interfaces::surface->draw_text_pos(x, y);
		interfaces::surface->set_text_color(color.r, color.g, color.b, color.a);
		interfaces::surface->draw_render_text(temp, wcslen(temp));
	}
}

void render::draw_rect(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color) {
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_outlined_rect(x, y, width, height);
}

void render::draw_filled_rect(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color) {
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_filled_rectangle(x, y, width, height);
}

void render::draw_outline(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color) {
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_outlined_rect(x, y, width, height);
}

void render::draw_textured_polygon(std::int32_t vertices_count, vertex_t* vertices, color color) {
	static unsigned char buf[4] = { 255, 255, 255, 255 };
	unsigned int texture_id{};
	if (!texture_id) {
		texture_id = interfaces::surface->create_new_texture_id(true);
		interfaces::surface->set_texture_rgba(texture_id, buf, 1, 1);
	}
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->set_texture(texture_id);
	interfaces::surface->draw_polygon(vertices_count, vertices);
}

void render::draw_circle(std::int32_t x, std::int32_t y, std::int32_t radius, std::int32_t segments, color color) {
	
	color.a = static_cast<int>(color.a * 1);

	static bool once = true;

	static std::vector<float> temppointsx;
	static std::vector<float> temppointsy;

	if (once)
	{
		float step = (float)M_PI * 2.0f / segments;
		for (float a = 0; a < (M_PI * 2.0f); a += step)
		{
			temppointsx.push_back(cosf(a));
			temppointsy.push_back(sinf(a));
		}
		once = false;
	}

	std::vector<int> pointsx;
	std::vector<int> pointsy;
	std::vector<vertex_t> vertices;

	for (int i = 0; i < temppointsx.size(); i++)
	{
		float eeks = radius * temppointsx[i] + x;
		float why = radius * temppointsy[i] + y;
		pointsx.push_back(eeks);
		pointsy.push_back(why);

		vertices.emplace_back(vertex_t(vec2_t(eeks, why)));
	}

	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_textured_polygon(segments, vertices.data());
}

void render::draw_rounded_rect(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, std::int32_t r, color c, bool left_under, bool left_up, bool right_under, bool right_up)
{
	if (left_up)
		render::draw_circle(x + r, y + r, r, 180, c);

	if (right_up)
		render::draw_circle(x + w - r, y + r, r, 180, c);
	
	if (left_under)
		render::draw_circle(x + r, y + h - r, r, 180, c);

	if (right_under)
		render::draw_circle(x + w - r, y + h - r, r, 180, c);

	//upper
	render::draw_filled_rect(x + r, y, w - r * 2, h / 2, c);
	
	//lower
	render::draw_filled_rect(x + r, y + h / 2, w - r * 2, h / 2, c);

	//left
	render::draw_filled_rect(x, (left_up ? y + r : y), w / 2, ( !left_up ? (left_under ? h - r : h) : (left_under ? h - r * 2 : h - r)), c);

	//right
	render::draw_filled_rect(x + w / 2, (right_up ? y + r : y), w / 2, (!right_up ? (right_under ? h - r : h) : (right_under ? h - r * 2 : h - r)), c);
	//render::draw_filled_rect(x + w /2, (right_up ? y + r : y), w / 2, (right_under ? h - r * 2 : h), c);
}

vec2_t render::get_text_size(unsigned long font, std::string text) {
	std::wstring a(text.begin(), text.end());
	const wchar_t* wstr = a.c_str();
	static int w, h;

	interfaces::surface->get_text_size(font, wstr, w, h);
	return { static_cast<float>(w), static_cast<float>(h) };
}
