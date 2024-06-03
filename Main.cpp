#include "Including.h"

int main(int argc, char** argv) {
    // Find window by name
    HWND hwnd = FindWindow(NULL, L"TelegramDesktop"); // name of the blum window
    if (hwnd == NULL) {
        cerr << "Window not found" << endl;
        return -1;
    }

    windowsPrepare(hwnd);

    int key = 0;
    Sleep(3000);    //wait to start a game
    while (key != 27) 
    {
        // Get window coordinates
        Rect windowRect;
        if (!GetWindowRectangle(hwnd, windowRect)) {
            cerr << "Failed to get window rect" << endl;
            return -1;
        }

        // Capture the screen area corresponding to the window
        Mat screen = CaptureScreen(windowRect);
        if (screen.empty()) {
            cerr << "Failed to capture screen" << endl;
            return -1;
        }

        Mat hsvImage;
        cvtColor(screen, hsvImage, COLOR_BGR2HSV);

        // Identifying the color green in an image
        Mat greenMask;
        inRange(hsvImage, Scalar(35, 100, 100), Scalar(85, 255, 255), greenMask);

        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(greenMask, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

        // Searching through the found contours
        contoursEnum(screen, contours);

        // Show the capture image
        imshow("Objects Detection", screen);

        // Delay between frames
        key = waitKey(30); // Delay of 30 milliseconds between frames
    }

    return 0;
}

//401x711
//you should use 100% of your screen, if you're on laptop