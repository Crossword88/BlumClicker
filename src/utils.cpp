#include "utils.h"

int low_b = 0, low_g = 0, low_r = 0;
int high_b = 255, high_g = 255, high_r = 255;

bool IsTelegramDesktop(HWND hwnd) {
    const int bufferSize = 256;
    wchar_t windowTitle[bufferSize];
    GetWindowText(hwnd, windowTitle, bufferSize);
    return wcscmp(windowTitle, L"TelegramDesktop") == 0;
}


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

std::string type2str(int type) {
    std::string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch (depth) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
    }

    r += "C";
    r += (chans + '0');

    return r;
}

static void on_low_B_thresh_trackbar(int, void*) {
    low_b = min(high_b - 1, low_b);
    setTrackbarPos("Low B", window_detection_name, low_b);
}
static void on_high_B_thresh_trackbar(int, void*) {
    high_b = max(high_b, low_b + 1);
    setTrackbarPos("High B", window_detection_name, high_b);
}
static void on_low_G_thresh_trackbar(int, void*) {
    low_g = min(high_g - 1, low_g);
    setTrackbarPos("Low G", window_detection_name, low_g);
}
static void on_high_G_thresh_trackbar(int, void*) {
    high_g = max(high_g, low_g + 1);
    setTrackbarPos("High G", window_detection_name, high_g);
}
static void on_low_R_thresh_trackbar(int, void*) {
    low_r = min(high_r - 1, low_r);
    setTrackbarPos("Low R", window_detection_name, low_r);
}
static void on_high_R_thresh_trackbar(int, void*) {
    high_r = max(high_r, low_r + 1);
    setTrackbarPos("High R", window_detection_name, high_r);
}

// Function to capture a screenshot of the screen
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
    bi.biHeight = -screenRect.height;
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

    return screenMat;
}

// Function to simulate a mouse click
void ClickAt(int x, int y) {
    SetCursorPos(x, y);
    mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, x, y, 0, 0);
}

void createTaskBars() {
    namedWindow(window_detection_name, WINDOW_NORMAL);

    createTrackbar("Low B", window_detection_name, &low_b, 255, on_low_B_thresh_trackbar);
    createTrackbar("High B", window_detection_name, &high_b, 255, on_high_B_thresh_trackbar);

    createTrackbar("Low G", window_detection_name, &low_g, 255, on_low_G_thresh_trackbar);
    createTrackbar("High G", window_detection_name, &high_g, 255, on_high_G_thresh_trackbar);

    createTrackbar("Low R", window_detection_name, &low_r, 255, on_low_R_thresh_trackbar);
    createTrackbar("High R", window_detection_name, &high_r, 255, on_high_R_thresh_trackbar);
}

void setDebugRange(Mat screen, Mat res) {
    inRange(screen, Scalar(low_b, low_g, low_r), Scalar(high_b, high_g, high_r), res);
}
