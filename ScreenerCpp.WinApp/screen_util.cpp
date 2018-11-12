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
		auto image = new image_wrapper();

		RECT rect;
		GetClientRect(handle_window, &rect);
		image->width = rect.right - rect.left;
		image->height = rect.bottom - rect.top;

		const auto hdc_window = GetDC(handle_window);
		const auto hdc = CreateCompatibleDC(hdc_window);
		const auto hbitmap = CreateCompatibleBitmap(hdc_window, image->width, image->height);
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
		info.bmiHeader.biSizeImage = (image->width * bmp.bmBitsPixel + 31) / 8 * image->height;

		image->bytes = vector<char>(info.bmiHeader.biSizeImage);
		GetDIBits(hdc, hbitmap, 0, image->height, &image->bytes[0], &info, DIB_RGB_COLORS);
		DeleteDC(hdc);
		DeleteObject(hbitmap);

		return image;
	}
}
