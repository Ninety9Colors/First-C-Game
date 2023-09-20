#ifndef WIN32_PLATFORM_H
#define WIN32_PLATFORM_H

#include <mutex>

#include <windows.h>

class Render_State {
public:
	const void* get_memory();
	void create_memory(const HWND& hwnd);
	const int& get_width();
	void set_width(const int& i);
	const int& get_height();
	void set_height(const int& i);
	const BITMAPINFO& get_bitmap_info();
	void set_bitmap_info(const BITMAPINFO& info);

private:
	void* memory_; //void pointer to memory on the heap
	std::mutex memory_mutex_;
	int width_;
	std::mutex width_mutex_;
	int height_;
	std::mutex height_mutex_;
	BITMAPINFO bitmap_info_;
	std::mutex bitmap_info_mutex_;

};

#endif// WIN32_PLATFORM_H
