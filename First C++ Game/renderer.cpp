#include <algorithm>

#include "renderer.h"
#include "win32_platform.h"

const float& Renderer::get_render_scale() {return render_scale_;}

void Renderer::clear_screen(Render_State& render_state,uint32_t color){
	uint32_t* pixel = (uint32_t*)render_state.get_memory();

	for (int y = 0; y < render_state.get_height(); ++y) {
		for (int x = 0; x < render_state.get_width(); ++x) {
			*pixel++ = color;

		}
	}
}

void Renderer::draw_rect_in_pixels(Render_State& render_state, int pos_x, int pos_y, int width, int height, const uint32_t& color)
{
	// limiting the range
	pos_x = std::clamp<int>(pos_x, 0, render_state.get_width());
	pos_y = std::clamp<int>(pos_y, 0, render_state.get_height());
	width = std::clamp<int>(width, 0, render_state.get_width());
	height = std::clamp<int>(height, 0, render_state.get_height());

	// draw pixels
	for (int y = pos_y; y < height; ++y) {
		uint32_t* pixel = (uint32_t*)render_state.get_memory () + pos_x + y * render_state.get_width();
		for (int x = pos_x; x < width; ++x) {
			*pixel++ = color;

		}
	}
}
void Renderer::draw_rect_centered(Render_State& render_state, float pos_x, float pos_y, float half_width, float half_height, const uint32_t& color) { // parameters are in percentages of screen

	// change unit to percentage of screen
	pos_x *= render_state.get_height() * render_scale_; //picking height for these means the width of the monitor does not affect the sizes of rectangles, but the height does
	pos_y *= render_state.get_height() * render_scale_;
	half_width *= render_state.get_height() * render_scale_;
	half_height *= render_state.get_height() * render_scale_;

	// centers our pos
	pos_x += render_state.get_width() / 2.f;
	pos_y += render_state.get_height() / 2.f;

	// draw half_width around pos
	int left = pos_x - half_width;
	int right = pos_x + half_width;
	int bottom = pos_y - half_height;
	int top = pos_y + half_height;

	draw_rect_in_pixels(render_state, left, bottom, right, top, color);
}