#ifndef RENDERER_H
#define RENDERER_H

#include "win32_platform.h"

class Renderer {
private:
	static constexpr float render_scale_ = 0.01f;
public:
	const float& get_render_scale();

	static void clear_screen(const int& render_width, const int& render_height, const void* render_memory, uint32_t color);
	static void draw_rect_in_pixels(int pos_x, int pos_y, int width, int height, const int &render_width, const int& render_height, const void* render_memory, const uint32_t& color);
	static void draw_rect_centered(int pos_x, int pos_y, int rect_width, int rect_height, const int &render_width, const int& render_height, const void* render_memory, const uint32_t& color);
};
#endif