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

	static void save_bitmap_to_file(char* pixels, const long width, const long height,
	                                const WORD bpp, const LPCTSTR path)
	{
		const unsigned long headers_size = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		const unsigned long pixel_data_size = bpp / 8 * height * width;

		BITMAPINFOHEADER bmp_info_header = {0};

		// Set the size  
		bmp_info_header.biSize = sizeof(BITMAPINFOHEADER);

		// Bit count  
		bmp_info_header.biBitCount = bpp;

		// Use all colors  
		bmp_info_header.biClrImportant = 0;

		// Use as many colors according to bits per pixel  
		bmp_info_header.biClrUsed = 0;

		// Store as un Compressed  
		bmp_info_header.biCompression = BI_RGB;

		// Set the height in pixels  
		bmp_info_header.biHeight = height;

		// Width of the Image in pixels  
		bmp_info_header.biWidth = width;

		// Default number of planes  
		bmp_info_header.biPlanes = 1;

		// Calculate the image size in bytes  
		bmp_info_header.biSizeImage = pixel_data_size;

		BITMAPFILEHEADER bfh = {0};

		// This value should be values of BM letters i.e 0x4D42  
		// 0x4D = M 0×42 = B storing in reverse order to match with endian  
		bfh.bfType = 0x4D42;
		//bfh.bfType = 'B'+('M' << 8); 

		// <<8 used to shift ‘M’ to end  */  

		// Offset to the RGBQUAD  
		bfh.bfOffBits = headers_size;

		// Total size of image including size of headers  
		bfh.bfSize = headers_size + pixel_data_size;

		// Create the file in disk to write  
		const auto bitmap_file = CreateFile(path, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
		                                    nullptr);

		// Return if error opening file  
		if (!bitmap_file) return;

		DWORD dw_written = 0;

		// Write the File header  
		WriteFile(bitmap_file, &bfh, sizeof bfh, &dw_written, nullptr);

		// Write the bitmap info header  
		WriteFile(bitmap_file, &bmp_info_header, sizeof bmp_info_header, &dw_written, nullptr);

		// Write the RGB Data  
		WriteFile(bitmap_file, pixels, bmp_info_header.biSizeImage, &dw_written, nullptr);

		// Close the file handle  
		CloseHandle(bitmap_file);
	}

	image_wrapper* screen_util::get_window_screen(const HWND handle_window)
	{
		RECT rect;
		GetClientRect(handle_window, &rect);
		const auto width = rect.right - rect.left;
		const auto height = rect.bottom - rect.top;

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

		auto pixels16 = vector<char>(2 * height * width);
		for (auto i32 = 0, i16 = 0; i16 < pixels16.size(); i32 += 4, i16 += 2)
		{
			const auto r = pixels32[i32 + 0];
			const auto g = pixels32[i32 + 1];
			const auto b = pixels32[i32 + 2];
			const short color = ((r >> 3) << 11) & 0xF800 | ((g >> 2) << 5) & 0x07E0 | (b >> 3) & 0x001F;

			//const short color = ((int(r / 255 * 31) << 11) | (int(g / 255 * 63) << 5) | (int(b / 255 * 31)));

			pixels16[i16] = color;
			pixels16[i16 + 1] = color >> 8;
		}

		delete[] pixels32;

		save_bitmap_to_file(pixels16.data(), width, height, 16, L"C:\\Users\\zhelonkin\\bluesquare.bmp");

		const auto image = new image_wrapper();
		image->width = width;
		image->height = height;
		image->bytes = pixels16;
		return image;
	}
}
