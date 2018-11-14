#include "stdafx.h"
#include "screen_util.hpp"
#include <windows.h>
#include <vector>
#include <thread>

using namespace std;

namespace screener::winapp::utils
{
	image_wrapper* screen_util::get_window_screen()
	{
		return get_window_screen("Безымянный - Paint");
	}

	image_wrapper* screen_util::get_window_screen(const char* window_name)
	{
		return get_window_screen(FindWindow(nullptr, L"Безымянный - Paint"));
	}

	image_wrapper* screen_util::get_window_screen(const HWND handle_window)
	{
		RECT rect;
		GetClientRect(handle_window, &rect);
		const auto width = rect.right - rect.left;
		const auto height = rect.bottom - rect.top;
		const auto row_length = rgb565_bpp * width;

		const auto hdc_window = GetWindowDC(handle_window);
		const auto hdc = CreateCompatibleDC(hdc_window);
		const auto hbitmap = CreateCompatibleBitmap(hdc_window, width, height);
		SelectObject(hdc, hbitmap);
		PrintWindow(handle_window, hdc, PW_CLIENTONLY);
		ReleaseDC(handle_window, hdc_window);

		BITMAP bmp;
		GetObject(hbitmap, sizeof bmp, &bmp);

		BITMAPINFO info;
		info.bmiHeader.biSize = sizeof BITMAPINFOHEADER;
		info.bmiHeader.biWidth = bmp.bmWidth;
		info.bmiHeader.biHeight = bmp.bmHeight;
		info.bmiHeader.biPlanes = bmp.bmPlanes;
		info.bmiHeader.biBitCount = bmp.bmBitsPixel;
		info.bmiHeader.biCompression = BI_RGB;
		info.bmiHeader.biSizeImage = bmp.bmBitsPixel / 8 * height * width;

		const auto pixels32 = new char[info.bmiHeader.biSizeImage];
		GetDIBits(hdc, hbitmap, 0, height, &pixels32[0], &info, DIB_RGB_COLORS);
		DeleteDC(hdc);
		DeleteObject(hbitmap);

		auto pixels16 = vector<char>(rgb565_bpp * height * width);
		for (auto i32 = 0, i16 = 0; i16 < pixels16.size(); i32 += 4, i16 += rgb565_bpp)
		{
			const short color = pixels32[i32] >> 3 & 0x001F
				| pixels32[i32 + 1] << 3 & 0x07E0
				| pixels32[i32 + 2] << 8 & 0xF800;

			const auto inv_ind = (height - 1 - int(i16 / row_length)) * row_length + i16 % row_length;

			pixels16[inv_ind] = color;
			pixels16[inv_ind + 1] = color >> 8;
		}

		delete[] pixels32;

		const auto image = new image_wrapper();
		image->width = width;
		image->height = height;
		image->bytes = pixels16;
		return image;
	}
}
