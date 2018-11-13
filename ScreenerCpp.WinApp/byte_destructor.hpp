#pragma once
#include <vector>
#include <functional>

using namespace std;

namespace screener::winapp::destructors
{
	typedef function<void(vector<char>)> bytes_consumer;

	enum data_index : unsigned int
	{
		type_index = 0,
		last_index = 1,
		order_index = 2
	};

	enum data_type : char
	{
		data_image = 0
	};

	class byte_destructor
	{
	public:

		static const unsigned int BUFFER_SIZE = 62000;
		static const unsigned int BYTES_RESERVE = 3;

		static void destruct(vector<char>, data_type, bytes_consumer);
	};
}
