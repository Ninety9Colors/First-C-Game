#ifndef WIN32_PLATFORM_H
#define WIN32_PLATFORM_H

#include <mutex>

#include <windows.h>

class Render_State {
public:
	static const void* get_memory();
	static void create_memory(const HWND& hwnd);
	static const int& get_width();
	static void set_width(const int& i);
	static const int& get_height();
	static void set_height(const int& i);
	static const BITMAPINFO& get_bitmap_info();
	static void set_bitmap_info(const BITMAPINFO& info);

private:
	static void* memory_; //void pointer to memory on the heap
	static std::mutex memory_mutex_;
	static int width_;
	static std::mutex width_mutex_;
	static int height_;
	static std::mutex height_mutex_;
	static BITMAPINFO bitmap_info_;
	static std::mutex bitmap_info_mutex_;

};

#endif// WIN32_PLATFORM_H
