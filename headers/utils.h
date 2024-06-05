#pragma once

#include "Windows.h"
#include "opencv2/opencv.hpp"
#include <vector>

using namespace std;
using namespace cv;

const String window_detection_name = "Object Detection";
extern int low_b, low_g, low_r;
extern int high_b, high_g, high_r;

bool IsTelegramDesktop(HWND hwnd);
void createTaskBars();
void setDebugRange(Mat& screen, Mat& res);
bool GetWindowRectangle(HWND hwnd, Rect& rect);
std::string type2str(int type);
Mat CaptureScreen(const Rect& screenRect);
void ClickAt(int x, int y);

// callback для ползунков
static void on_low_B_thresh_trackbar(int, void*);
static void on_high_B_thresh_trackbar(int, void*);
static void on_low_G_thresh_trackbar(int, void*);
static void on_high_G_thresh_trackbar(int, void*);
static void on_low_R_thresh_trackbar(int, void*);
static void on_high_R_thresh_trackbar(int, void*);
