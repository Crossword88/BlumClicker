#include "utils.h"


int main(int argc, char** argv) {
    // Find window by name
    std::cout << "Authors: @homyaklol @Botovod1 in telegram. Feel free to message me.\n";
    std::cout << "Waiting for TelegramDesktop...\n";
    HWND hwnd = FindWindow(NULL, L"TelegramDesktop");

    while (!hwnd || !IsTelegramDesktop(hwnd)) {
        hwnd = FindWindow(NULL, L"TelegramDesktop");
        Sleep(100); // Adding a small delay to avoid high CPU usage in the loop
    }

    std::cout << "TelegramDesktop FOUND...\n";

    int key = 0;

#ifdef _DEBUG
    createTaskBars();
#endif

    bool pause = true;

#ifdef _DEBUG
    pause = false;
#endif
    std::cout << "To start / pause the bot press P\n";

    while (true) { // Press ESC to exit

        if (GetAsyncKeyState(0x50)) {
            pause = !pause;
            cout << "Pause set to " << pause << endl;
            // Adding a small delay to debounce the ESC key press
            Sleep(200);
        }

        if (pause) {
            continue;
        }

        // Get window coordinates
        Rect windowRect;
        if (!GetWindowRectangle(hwnd, windowRect)) {
            cerr << "Failed to get window rect" << endl;
            return -1;
        }

        // Capture the screen area corresponding to the window
        Mat screen = CaptureScreen(windowRect);
       // Mat screen = imread("C://img.png");

        if (screen.empty()) {
            cerr << "Failed to capture screen" << endl;
            break;
        }
        Mat res;
        Mat bgrScreen;
        cvtColor(screen, bgrScreen, COLOR_RGBA2RGB);

#ifdef _DEBUG
        imshow("Blum", bgrScreen);
        setDebugRange(bgrScreen, res);
        imshow("Mask", res);
        waitKey(30);
        continue;
#endif

        inRange(bgrScreen, Scalar(0, 199, 200), Scalar(0, 255, 255), res);
        Mat idx;
        findNonZero(res, idx);

        if (idx.total() == 0)
            continue;
        Point pt = idx.at<Point>(0);
        if (pt.x != 0 && pt.y != 0) {
            ClickAt(windowRect.x + pt.x, windowRect.y + pt.y);
        }
        Sleep(60);
    }

    return 0;
}
