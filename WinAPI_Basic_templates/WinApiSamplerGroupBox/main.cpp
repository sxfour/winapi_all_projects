#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <string>
#include <Windows.h>
#include <CommCtrl.h>

HWND window = nullptr;
HWND groupBox1 = nullptr;
HWND groupBox2 = nullptr;
WNDPROC defWndProc = nullptr;

LRESULT OnWindowClose(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PostQuitMessage(0);

	return CallWindowProc(defWndProc, hWnd, message, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if (message == WM_CLOSE && hWnd == window) {
		return OnWindowClose(hWnd, message, wParam, lParam);
	}

	return CallWindowProc(defWndProc, hWnd, message, wParam, lParam);
}

int main() {
	window = CreateWindowEx(0, WC_DIALOG, L"GroupBox example", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 656, 521, nullptr, nullptr, nullptr, nullptr);
	groupBox1 = CreateWindowEx(0, WC_BUTTON, L"GroupBox 1", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 10, 305, 460, window, nullptr, nullptr, nullptr);
	groupBox2 = CreateWindowEx(0, WC_BUTTON, L"", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 325, 10, 305, 460, window, nullptr, nullptr, nullptr);
	SetWindowPos(groupBox1, 0, 0, 0, 5116, 589, SWP_FRAMECHANGED | WS_VISIBLE);
	SetWindowPos(groupBox2, 0, 0, 0, 5116, 589, SWP_FRAMECHANGED | WS_VISIBLE);
	defWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc)));
	
	ShowWindow(window, SW_SHOW);

	MSG message = { 0 };
	while (GetMessage(&message, nullptr, 0, 0)) {
		DispatchMessage(&message);
	}

	return 0;
}