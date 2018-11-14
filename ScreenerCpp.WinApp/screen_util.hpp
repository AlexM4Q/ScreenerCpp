#pragma once
#include <vector>
#include "image_wrapper.hpp"

using namespace std;

namespace screener::winapp::utils
{
	using namespace data;

	class screen_util
	{
		static const unsigned char rgb565_bpp = 2;

	public:
		static image_wrapper* get_window_screen();
		static image_wrapper* get_window_screen(const char*);
		static image_wrapper* get_window_screen(HWND);
	};
}
