#include <windows.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <fstream>
#include <iostream>

int main()
{
    HDC hScreenDC = GetDC(NULL);
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, screenWidth, screenHeight);
    SelectObject(hMemoryDC, hBitmap);
    BitBlt(hMemoryDC, 0, 0, screenWidth, screenHeight, hScreenDC, 0, 0, SRCCOPY);
    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = screenWidth;
    bi.biHeight = -screenHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;
    char* buffer = new char[screenWidth * screenHeight * 4];
    GetDIBits(hScreenDC, hBitmap, 0, screenHeight, buffer, reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS);

    std::string filename = "screenshot.jpg";
    if (stbi_write_jpg(filename.c_str(), screenWidth, screenHeight, 4, buffer, 100)) {
        std::cout << "screenshot saved as " << filename << std::endl;;
    }
    else {
        std::cout << "error saving screenshot" << std::endl;
    }

    delete[] buffer;
    ReleaseDC(NULL, hScreenDC);
    DeleteDC(hMemoryDC);
    DeleteObject(hBitmap);
    return 0;
}
