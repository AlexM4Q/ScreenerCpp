#include "stdafx.h"
#include "byte_util.h"
#include <vector>

using namespace std;

namespace screener::winapp::util
{
	void byte_util::insert_length(vector<char>& buffer)
	{
		const auto size = buffer.size();
		buffer.insert(buffer.begin(), static_cast<BYTE>(size));
		buffer.insert(buffer.begin(), static_cast<BYTE>(size >> 8));
		buffer.insert(buffer.begin(), static_cast<BYTE>(size >> 16));
		buffer.insert(buffer.begin(), static_cast<BYTE>(size >> 24));
	}
}
