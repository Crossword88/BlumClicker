#include "utils.h"


int main(int argc, char** argv) {
    // Find window by name
    HWND hwnd = FindWindow(NULL, L"TelegramDesktop"); // Replace with the name of your window
    if (!hwnd) {
        cerr << "Window not found" << endl;
        return -1;
    }

    int key = 0;

#ifdef _DEBUG
    createTaskBars();
#endif

    while (true) { // Press ESC to exit

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
            break;
        }
        Mat res;
        Mat bgrScreen;
        cvtColor(screen, bgrScreen, COLOR_RGBA2RGB);

#ifdef _DEBUG
        setDebugRange(bgrScreen, res);
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
        ClickAt(windowRect.x + pt.x, windowRect.y + pt.y);
    }

    return 0;
}
