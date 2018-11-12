#include "stdafx.h"
#include "byte_util.hpp"
#include <vector>

using namespace std;

namespace screener::winapp::utils
{
	void byte_util::insert_int(vector<char>& buffer, const int value, const int pos)
	{
		buffer.insert(buffer.begin() + pos, value >> 24);
		buffer.insert(buffer.begin() + pos + 1, value >> 16);
		buffer.insert(buffer.begin() + pos + 2, value >> 8);
		buffer.insert(buffer.begin() + pos + 3, value);
	}
}
