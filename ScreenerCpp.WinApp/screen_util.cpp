#include "stdafx.h"
#include "screen_util.h"

#include <windows.h>
#include <vector>
#include <ostream>
#include <iostream>
#include <thread>

using namespace std;

namespace screener::winapp::util
{
	vector<char> screen_util::get_window_screen()
	{
		return get_window_screen("Безымянный - Paint");
	}

	vector<char> screen_util::get_window_screen(const char* window_name)
	{
		return get_window_screen(FindWindow(nullptr, L"Безымянный - Paint"));
	}

	vector<char> screen_util::get_window_screen(const HWND handle_window)
	{
		RECT rect;
		GetClientRect(handle_window, &rect);
		const auto width = rect.right - rect.left;
		const auto height = rect.bottom - rect.top;

		const auto hdc_screen = GetDC(nullptr);
		const auto hdc = CreateCompatibleDC(hdc_screen);
		const auto hbitmap = CreateCompatibleBitmap(hdc_screen, width, height);
		SelectObject(hdc, hbitmap);

		PrintWindow(handle_window, hdc, PW_CLIENTONLY);

		DeleteDC(hdc);
		ReleaseDC(nullptr, hdc_screen);

		BITMAP bmp;
		GetObject(hbitmap, sizeof bmp, &bmp);

		BITMAPINFO info;
		info.bmiHeader.biSize = sizeof BITMAPINFOHEADER;
		info.bmiHeader.biWidth = width;
		info.bmiHeader.biHeight = height;
		info.bmiHeader.biPlanes = 1;
		info.bmiHeader.biBitCount = bmp.bmBitsPixel;
		info.bmiHeader.biCompression = BI_RGB;
		info.bmiHeader.biSizeImage = (width * bmp.bmBitsPixel + 31) / 8 * height;

		auto pixels = vector<char>(info.bmiHeader.biSizeImage);
		const auto dc = CreateCompatibleDC(nullptr);
		GetDIBits(dc, hbitmap, 0, height, &pixels[0], &info, DIB_RGB_COLORS);
		DeleteDC(dc);
		DeleteObject(hbitmap);

		this_thread::sleep_for(1s);
		return pixels;
	}
}
