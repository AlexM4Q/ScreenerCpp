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
		std::vector<char> ToPixels(HBITMAP BitmapHandle, int width, int height)
	{
		BITMAP Bmp = { 0 };
		BITMAPINFO Info = { 0 };
		std::vector<char> Pixels = std::vector<char>();

		HDC DC = CreateCompatibleDC(NULL);
		std::memset(&Info, 0, sizeof(BITMAPINFO)); //not necessary really..
		HBITMAP OldBitmap = (HBITMAP)SelectObject(DC, BitmapHandle);
		GetObject(BitmapHandle, sizeof(Bmp), &Bmp);

		Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		Info.bmiHeader.biWidth = width = Bmp.bmWidth;
		Info.bmiHeader.biHeight = height = Bmp.bmHeight;
		Info.bmiHeader.biPlanes = 1;
		Info.bmiHeader.biBitCount = Bmp.bmBitsPixel;
		Info.bmiHeader.biCompression = BI_RGB;
		Info.bmiHeader.biSizeImage = ((width * Bmp.bmBitsPixel + 31) / 32) * 4 * height;

		Pixels.resize(Info.bmiHeader.biSizeImage);
		GetDIBits(DC, BitmapHandle, 0, height, &Pixels[0], &Info, DIB_RGB_COLORS);
		SelectObject(DC, OldBitmap);

		height = std::abs(height);
		DeleteDC(DC);
		return Pixels;
	}

	vector<char> screen_util::get_desktop_screen_shot()
	{
		HWND hwnd = FindWindow(NULL, TEXT("Безымянный - Paint"));
		RECT rc;
		GetClientRect(hwnd, &rc);

		//create
		HDC hdcScreen = GetDC(nullptr);
		HDC hdc = CreateCompatibleDC(hdcScreen);
		const auto width = rc.right - rc.left;
		const auto height = rc.bottom - rc.top;
		HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen, width, height);
		SelectObject(hdc, hbmp);

		//Print to memory hdc
		PrintWindow(hwnd, hdc, PW_CLIENTONLY);

		//copy to clipboard
		// OpenClipboard(nullptr);
		// EmptyClipboard();
		// SetClipboardData(CF_BITMAP, hbmp);
		// CloseClipboard();

		//release
		// DeleteDC(hdc);
		// DeleteObject(hbmp);
		// ReleaseDC(nullptr, hdcScreen);

		const auto length = width * height;
		// const auto bmp_buffer = new BYTE[length];
		// const auto bmp_buffer = static_cast<BYTE*>(GlobalAlloc(GPTR, length));

		// BITMAPINFO bmpInfo;
		// memset(&bmpInfo, 0, sizeof(BITMAPINFOHEADER));
		// bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		// GetDIBits(hdc, hbmp, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS);
		// bmpInfo.bmiHeader.biBitCount = 32;
		// bmpInfo.bmiHeader.biCompression = BI_RGB;
		// GetDIBits(hdc, hbmp, 0, bmpInfo.bmiHeader.biHeight, bmp_buffer, &bmpInfo, DIB_RGB_COLORS);


		// BITMAP bitmap;
		// int error = GetObject(hbmp, sizeof(BITMAP), &bitmap);

		// GetBitmapBits(hbmp, length, bmp_buffer);
		
		// return vector<char>(bmp_buffer, bmp_buffer + length);


		std::this_thread::sleep_for(2s);
		return ToPixels(hbmp,width, -height);
	}
}
