#pragma once

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <Windows.h>
#include <iostream>

using namespace std;
using namespace cv;

// Function to get window coordinates
bool GetWindowRectangle(HWND hwnd, Rect& rect) {
    RECT winRect;
    if (::GetWindowRect(hwnd, &winRect)) {
        rect.x = winRect.left + 10;
        rect.y = winRect.top + 20;
        rect.width = (winRect.right - winRect.left) - 20;
        rect.height = (winRect.bottom - winRect.top) - 30; // 20 pixels from the top and 10 from the bottom
        return true;
    }
    return false;
}

// Function to capture screenshot of the screen
Mat CaptureScreen(const Rect& screenRect) {
    HDC hScreenDC = GetDC(NULL);
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, screenRect.width, screenRect.height);
    SelectObject(hMemoryDC, hBitmap);
    BitBlt(hMemoryDC, 0, 0, screenRect.width, screenRect.height, hScreenDC, screenRect.x, screenRect.y, SRCCOPY);

    Mat screenMat(screenRect.height, screenRect.width, CV_8UC4);
    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = screenRect.width;
    bi.biHeight = -screenRect.height; // ¬ысота изображени€ отрицательна дл€ правильного направлени€ координат
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    GetDIBits(hMemoryDC, hBitmap, 0, screenMat.rows, screenMat.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    DeleteObject(hBitmap);
    DeleteDC(hMemoryDC);
    ReleaseDC(NULL, hScreenDC);

    // ќбрезка верхней части изображени€ на cropTop пикселей
    Rect croppedRect(0, 110, screenRect.width, screenRect.height - 110);
    Mat croppedScreenMat = screenMat(croppedRect);

    return croppedScreenMat;
}

void mouseClick(Rect boundRect)
{
    SetCursorPos(boundRect.x, boundRect.y + 110);
    Sleep(100000);
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
}

void contoursEnum(Mat screen, vector<vector<Point>> contours)
{
    for (size_t i = 0; i < contours.size(); i++) {
        // ѕолучение ограничивающего пр€моугольника дл€ каждого контура
        Rect boundRect = boundingRect(contours[i]);

        // Show rectangle on the picture
        rectangle(screen, boundRect, Scalar(0, 0, 255), 2);

        // print coordinats of the rectangles
        /*cout << "Object " << i + 1 << ": ";
        cout << "Top-left corner: (" << boundRect.x << ", " << boundRect.y << "), ";
        cout << "Width: " << boundRect.width << ", ";
        cout << "Height: " << boundRect.height << endl;*/

        //mouseClick(boundRect);
    }
}

void windowsPrepare(HWND hwnd)
{
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE);  //always active

    // Bring the window to the foreground and make it active
    SetForegroundWindow(hwnd);
    SetActiveWindow(hwnd);

    Sleep(2000);
    namedWindow("Objects Detection", WINDOW_NORMAL);
    HWND windowOut = FindWindow(NULL, L"Objects Detection");
    if (windowOut == NULL) 
        MessageBox(0, L"LOh", L"aha", MB_ICONERROR);
    else
        SetWindowPos(windowOut, HWND_TOPMOST, 400, 20, 0, 0, SWP_NOSIZE);

    // Wait a bit to ensure the window is fully on top
    Sleep(100);
}