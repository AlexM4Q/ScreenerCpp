#pragma once
#include "byte_destructor.hpp"
#include "image_wrapper.hpp"

namespace screener::winapp::destructors
{
	using namespace data;

	class image_destructor
	{
	public:
		static void destruct(image_wrapper&, bytes_consumer);
	};
}
