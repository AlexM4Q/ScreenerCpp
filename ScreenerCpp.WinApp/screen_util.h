#pragma once
#include <vector>

using namespace std;

namespace screener::winapp::util
{
	class screen_util
	{
	public:
		static vector<char> get_window_screen();
		static vector<char> get_window_screen(const char*);
		static vector<char> get_window_screen(HWND);
	};
}
