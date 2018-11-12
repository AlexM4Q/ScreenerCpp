#include "stdafx.h"
#include "image_destructor.hpp"
#include "byte_util.hpp"

namespace screener::winapp::destructors
{
	using namespace utils;

	void image_destructor::destruct(image_wrapper& image, const bytes_consumer consumer)
	{
		byte_util::insert_int(image.bytes, image.width, 0);
		byte_util::insert_int(image.bytes, image.height, 4);
		return byte_destructor::destruct(image.bytes, data_image, consumer);
	}
}
