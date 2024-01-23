#include <Windows.h>
#include <iostream>
#include <thread>

using namespace std;

// İlk thread için işlev
void ThreadFunction1() {
    for (int i = 0; i < 5; ++i) {
        cout << "Thread 1: " << i << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

// İkinci thread için işlev
void ThreadFunction2() {
    for (int i = 0; i < 5; ++i) {
        cout << "Thread 2: " << i << endl;
        this_thread::sleep_for(chrono::milliseconds(700));
    }
}

// Windows uygulama pencere işlevi
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

int main() {
    // Pencere sınıfı tanımlama
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"ThreadedAppClass";

    // Pencere sınıfını kaydetme
    RegisterClass(&wc);

    // Pencere oluşturma
    HWND hwnd = CreateWindowEx(
        0,
        L"ThreadedAppClass",
        L"Threaded Application",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    // Pencereyi gösterme
    ShowWindow(hwnd, SW_SHOWNORMAL);

    // İlk thread'i başlatma
    thread t1(ThreadFunction1);

    // İkinci thread'i başlatma
    thread t2(ThreadFunction2);

    // Windows olay döngüsü
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Thread'leri bekletme
    t1.join();
    t2.join();

    return 0;
}