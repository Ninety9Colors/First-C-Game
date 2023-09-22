#ifndef RENDERER_H
#define RENDERER_H

#include "win32_platform.h"

class Renderer {
private:
	static constexpr float render_scale_ = 0.01f;
public:
	const float& get_render_scale();

	void clear_screen(Render_State& render_state, uint32_t color);
	void draw_rect_in_pixels(Render_State& render_state, int pos_x, int pos_y, int width, int height, const uint32_t& color);
	void draw_rect_centered(Render_State& render_state, float pos_x, float pos_y, float half_width, float half_height, const uint32_t& color);
};
#endif