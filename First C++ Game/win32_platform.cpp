#include <windows.h>

#include <mutex>

#include "renderer.h"
#include "win32_platform.h"

namespace {
	bool global_running = true;
	Render_State render_state;
	Renderer renderer;
}

const void* Render_State::get_memory() {
	std::lock_guard<std::mutex> lock(memory_mutex_);
	return memory_;
};
void Render_State::create_memory(const HWND& hwnd) {
	RECT temp_rect;
	GetClientRect(hwnd, &temp_rect); //gives rect the top, bottom, right, and left pixels
	render_state.set_width(temp_rect.right - temp_rect.left);
	render_state.set_height(temp_rect.bottom - temp_rect.top);
	int memory_size = get_width() * get_height() * sizeof(uint32_t); //pixels * size of a RGB pixel
	{
		std::lock_guard<std::mutex> lock(memory_mutex_);
		if (memory_) VirtualFree(memory_, 0, MEM_RELEASE); // removes the memory so it can be reallocated
		memory_ = (VirtualAlloc(0, memory_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE)); // creates memory on the heap with windows specific alloc
	}
};
const int& Render_State::get_width() {
	std::lock_guard<std::mutex> lock{ width_mutex_ };
	return width_;
}
void Render_State::set_width(const int& i) {
	std::lock_guard<std::mutex> lock{ width_mutex_ };
	width_ = i;
}
const int& Render_State::get_height() {
	std::lock_guard<std::mutex> lock{ width_mutex_ };
	return height_;
}
void Render_State::set_height(const int& i) {
	std::lock_guard<std::mutex> lock{ width_mutex_ };
	height_ = i;
}
const BITMAPINFO& Render_State::get_bitmap_info() {
	std::lock_guard<std::mutex> lock{ bitmap_info_mutex_ };
	return bitmap_info_;
}
void Render_State::set_bitmap_info(const BITMAPINFO& info) {
	std::lock_guard<std::mutex> lock{ bitmap_info_mutex_ };
	bitmap_info_ = info;
}

LRESULT CALLBACK window_processor(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { //returns LRESULT which is an integer
	LRESULT result{0};

	switch (uMsg) {
		case WM_SIZE: {//window changed size

			render_state.create_memory(hwnd);

			// initialize render_state bitmapinfo
			BITMAPINFO temp_bitmap_info{};
			temp_bitmap_info.bmiHeader.biSize = sizeof(temp_bitmap_info.bmiHeader);
			temp_bitmap_info.bmiHeader.biWidth = render_state.get_width();
			temp_bitmap_info.bmiHeader.biHeight = render_state.get_height();
			temp_bitmap_info.bmiHeader.biPlanes = 1; //must be one for some reason
			temp_bitmap_info.bmiHeader.biBitCount = 32;
			temp_bitmap_info.bmiHeader.biCompression = BI_RGB; // no compression format
			render_state.set_bitmap_info(temp_bitmap_info);

		}break;
		case WM_CLOSE: {
			global_running = false;

		}break;
		case WM_DESTROY: {
			global_running = false;

		}break;
		default: {
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);

		}
	};
	return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	//TO CREATE A WINDOW IN WINDOWS 3 STEPS
	//1. Create a window class
	WNDCLASS window_class{};
	window_class.hInstance = hInstance;
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	const wchar_t CLASS_NAME[] = L"Game Window Class";
	window_class.lpszClassName = CLASS_NAME;
	window_class.lpfnWndProc = window_processor;
	//2. Register the class - needs a pointer to the window class
	RegisterClass(&window_class);
	//3. Create window
	const wchar_t WINDOW_NAME[] = L"My First Game";
	HWND window = CreateWindow(window_class.lpszClassName, WINDOW_NAME,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, //overlapped window adds minimize and buttons
		CW_USEDEFAULT, CW_USEDEFAULT, // dont care about position so use default
		CW_USEDEFAULT, CW_USEDEFAULT, // dont care about width and height
		0, 0, // dont have a parent window or a menu
		hInstance,
		0); // pass additional info if we want

	HDC device_context = GetDC(window); // creates handle to a device context
	while (global_running)
	{
		//Input
		MSG message;

		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {//takes messages from the window we DIDNT read so far, PM_REMOVE removes them once they are read
			//dispatch messages to our callback
			TranslateMessage(&message); // translates VIRTUAL keystrokes to ACTUAL characters so the callback function understands them, also allows arrow keys
			DispatchMessage(&message);
		}
		//Simulate
		renderer.clear_screen(render_state, 0x111111);
		renderer.draw_rect_centered(render_state, 0, 0, 99, 1, 0x00FF00);
		//Render
		StretchDIBits(device_context, // to draw something you need a device context
			0, 0, // for xDest and yDest, 0 starts at the top left corner
			render_state.get_width(), render_state.get_height(),
			0, 0, render_state.get_width(), render_state.get_height(), // dimensions for when window interprets memory as a 2d rectangle
			render_state.get_memory(),
			&render_state.get_bitmap_info(), // HOW TO INTERPRET THE MEMORY
			DIB_RGB_COLORS,
			SRCCOPY); // how the memory is displayed as pixels
	}




}