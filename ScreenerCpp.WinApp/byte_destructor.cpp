#include "stdafx.h"
#include "byte_destructor.hpp"

namespace screener::winapp::destructors
{
	void byte_destructor::destruct(vector<char> bytes, const data_type type, const bytes_consumer consumer)
	{
		const double total_size = bytes.size();
		const auto parts_count = char(ceil(total_size / BUFFER_SIZE));
		const auto buffer_size = int(ceil(total_size / parts_count));
		char index = 0;
		for (auto min = 0; min < total_size; min += buffer_size, index++)
		{
			const auto next = min + buffer_size;
			const auto part_size = next > total_size
				                       ? buffer_size + total_size - next
				                       : buffer_size;

			auto part = vector<char>(part_size + BYTES_RESERVE);
			part[type_index] = type;
			part[last_index] = parts_count - 1;
			part[order_index] = index;
			for (auto i = 0; i < part_size; i++)
			{
				part[i + BYTES_RESERVE] = bytes[i + min];
			}

			consumer(part);
		}
	}
}
