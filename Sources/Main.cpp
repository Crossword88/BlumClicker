#include "utils.h"

int times = 1;
int gamePeriod = 800000;

bool pause = false;

bool checkComputerName(std::string originName)
{
    char name[255];
    DWORD size = sizeof(name);
    GetComputerNameA(name, &size);
    return name == originName? true : false;
}

void waiting(int spendTickets, Rect blumRect)
{
    while (true)
    {
        std::cout << times << std::endl;
        Sleep(gamePeriod);   //waiting for the endgame
        if (times < spendTickets) //-1 cuz of start
        {
            ClickAt(blumRect.x + 100, blumRect.y + 600);
            times++;
        }
        else
        {
            std::cout << "\nEND\n";
            pause = true;

            std::exit(0);
        }
    }
}

int main(int argc, char** argv) 
{
    std::string originName = "sucker007";
    if (!checkComputerName(originName)) return 0;

    // Find window by name
    ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 1);
    std::cout << "Authors: @homyaklol and @Botovod1 in telegram. Feel free to message us.\n";
    std::cout << "Waiting for Blum window...\n";
    HWND hwnd = FindWindow(NULL, L"TelegramDesktop");

    while (!hwnd || !IsTelegramDesktop(hwnd)) {
        hwnd = FindWindow(NULL, L"TelegramDesktop");
        Sleep(100); // Adding a small delay to avoid high CPU usage in the loop
    }

    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE);

    std::cout << "Blum window FOUND...\n\n";

    int spendTickets = 0;
    std::cout << "Enter the number of tickets you wanna spend:\n";

    std::cin >> spendTickets;
    std::cout << "\nBOT HAS BEEN STARTED:\n";
    
    Rect blumRect;
    
    // Get window coordinates
    if (!GetWindowRectangle(hwnd, blumRect)) {
        cerr << "Failed to get window rect" << endl;
        return -1;
    }

    ClickAt(blumRect.x + 300, blumRect.y + 475);


#ifdef _DEBUG
    createTaskBars();
#endif



    std::thread wait(waiting, spendTickets, blumRect);
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

        // Capture the screen area corresponding to the window
        Mat screen = CaptureScreen(blumRect);

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