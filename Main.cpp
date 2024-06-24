#include "utils.h"


bool checkComputerName(std::string originName)
{
    char name[255];
    DWORD size = sizeof(name);
    GetComputerNameA(name, &size);
    return name == originName? true : false;
}

void waiting()
{
    while (true)
    {
        Sleep(35000);
        ClickAt(100, 600);
    }
}

int main(int argc, char** argv) 
{
    std::string originName = "sucker007";
    if (!checkComputerName(originName)) return 0;

    // Find window by name
    ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 1);
    std::cout << "Authors: @homyaklol and @Botovod1 in telegram. Feel free to message us.\n";
    std::cout << "Waiting for TelegramDesktop...\n";
    HWND hwnd = FindWindow(NULL, L"TelegramDesktop");

    while (!hwnd || !IsTelegramDesktop(hwnd)) {
        hwnd = FindWindow(NULL, L"TelegramDesktop");
        Sleep(100); // Adding a small delay to avoid high CPU usage in the loop
    }

    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE);

    std::cout << "TelegramDesktop FOUND...\n";

    int key = 0;

#ifdef _DEBUG
    createTaskBars();
#endif

    bool pause = true;

#ifdef _DEBUG
    pause = false;
#endif

    std::thread wait(waiting);
    wait.detach();

    

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

        if (screen.empty()) {
            cerr << "Failed to capture screen" << endl;
            break;
        }
        Mat res;
        Mat res1;
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
        //inRange(bgrScreen, Scalar(227, 0, 0), Scalar(239, 255, 181), res1);
        Mat idx, idx1;
        findNonZero(res, idx);
        //findNonZero(res1, idx1);


        if (idx.total() == 0)
            continue;
        Point pt = idx.at<Point>(0);
        if (pt.x != 0 && pt.y != 0) {
            return 0;
        }



    }

    return 0;
}