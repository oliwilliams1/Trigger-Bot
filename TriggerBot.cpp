#include "screenshotApi.h"
#include <iostream>
#include <windows.h>
#include <chrono>

void mouseClick() {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;

    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(INPUT));

    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(INPUT));
}

bool isRedDominant(const cv::Vec3b& color) {
    int r = color[2];
    int g = color[1];
    int b = color[0];

    return (r > (g + b) * 1.33 && (r + g + b) > 200 && g < 90);
}

int main() {
    int centerX = 960;
    int centerY = 540;
    int boxSize = 5;

    int startingPointX = centerX - boxSize / 2;
    int endingPointX = centerX + boxSize / 2;
    int y = centerY;

    auto lastClickTime = std::chrono::steady_clock::now();
    const auto clickInterval = std::chrono::milliseconds(300);

    while (true) {
        // Capture the screen
        cv::Mat screen = captureScreen();

        for (int i = startingPointX; i <= endingPointX; ++i) {
            cv::Vec3b color = screen.at<cv::Vec3b>(y, i);
            if (isRedDominant(color)) {
                // Check if 300 ms have passed since the last click
                auto now = std::chrono::steady_clock::now();
                if (now - lastClickTime >= clickInterval) {
                    mouseClick();
                    lastClickTime = now;
                }
            }
        }

        Sleep(10);
    }
    return 0;
}