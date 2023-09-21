#include <algorithm>

#include "renderer.h"

const float& Renderer::get_render_scale() {return render_scale_;}

void Renderer::clear_screen(const int& render_width, const int& render_height, const void* render_memory, uint32_t color){
	uint32_t* pixel = (uint32_t*)render_memory;

	for (int y = 0; y < render_height; ++y) {
		for (int x = 0; x < render_width; ++x) {
			*pixel++ = color;

		}
	}
}

void Renderer::draw_rect_in_pixels(int pos_x, int pos_y, int rect_width, int rect_height, const int &render_width, const int& render_height, const void* render_memory, const uint32_t& color)
{
	// limiting the range
	pos_x = std::clamp<int>(pos_x, 0, render_width);
	pos_y = std::clamp<int>(pos_y, 0, render_height);
	rect_width = std::clamp<int>(rect_width, 0, render_width);
	rect_height = std::clamp<int>(rect_height, 0, render_height);

	// draw pixels
	for (int y = pos_y; y < rect_height; ++y) {
		uint32_t* pixel = (uint32_t*)render_memory + pos_x + y * render_width;
		for (int x = pos_x; x < rect_width; ++x) {
			*pixel++ = color;

		}
	}
}
void Renderer::draw_rect_centered(int pos_x, int pos_y, int half_width, int half_height, const int& render_width, const int& render_height, const void* render_memory, const uint32_t& color) { // parameters are in percentages of screen

	// change unit to percentage of screen
	pos_x *= render_height * render_scale_; //picking height for these means the width of the monitor does not affect the sizes of rectangles, but the height does
	pos_y *= render_height * render_scale_;
	half_width *= render_height * render_scale_;
	half_height *= render_height * render_scale_;

	// centers our pos
	pos_x += render_width / 2.f;
	pos_y += render_height / 2.f;

	// draw half_width around pos
	int left = pos_x - half_width;
	int right = pos_x + half_width;
	int bottom = pos_y - half_height;
	int top = pos_y + half_height;

	draw_rect_in_pixels(left, bottom, right, top, render_width, render_height, render_memory, color);
}