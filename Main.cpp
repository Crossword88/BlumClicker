#include "Including.h"

int main(int argc, char** argv) 
{
    ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 0);
    // Find window by name
    HWND hwnd = FindWindow(NULL, L"TelegramDesktop"); // name of the blum window
    if (hwnd == NULL) 
    {
        cerr << "Window not found" << endl;
        return -1;
    }

    windowsPrepare(hwnd);

    int key = 0;
    //Sleep(2000);    //wait to start a game
    while (key != 27) 
    {
        // Get window coordinates
        Rect windowRect;
        if (!GetWindowRectangle(hwnd, windowRect)) {
            cerr << "Failed to get window rect" << endl;
            return -1;
        }

        // Capture the screen area corresponding to the window
        Mat screen = CaptureScreen(windowRect);    //C:\\Users\\Serg\\Desktop\\game.jpg
        if (screen.empty()) {
            cerr << "Failed to capture screen" << endl;
            return -1;
        }

        Mat hsvImage;
        cvtColor(screen, hsvImage, COLOR_BGR2HSV);

        // Identifying the color green in an image
        Mat greenMask;
        inRange(hsvImage, Scalar(30, 90, 80), Scalar(40, 255, 255), greenMask);

        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(greenMask, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

        // Searching through the found contours

        contoursEnum(screen, contours);

        // Show the capture image
        imshow("Objects Detection", screen);

        // Delay between frames
        key = waitKey(10); // Delay of 30 milliseconds between frames
    }

    return 0;
}

//401x711