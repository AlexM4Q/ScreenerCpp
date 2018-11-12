#pragma once
#include <vector>

using namespace std;

namespace screener::winapp::data
{
	struct image_wrapper
	{
		int width;
		int height;
		vector<char> bytes;
	};
}
