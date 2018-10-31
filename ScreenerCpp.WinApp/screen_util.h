#pragma once
#include <vector>

using namespace std;

namespace screener::winapp::util
{
	class screen_util
	{
	public:
		static vector<char> get_desktop_screen_shot();
	};
}
