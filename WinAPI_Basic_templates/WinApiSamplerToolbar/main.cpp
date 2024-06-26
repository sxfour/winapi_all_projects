#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "comctl32")

#include <map>
#include <string>
#include <vector>
#include <Windows.h>
#include <CommCtrl.h>

using namespace std;
using namespace std::literals;

HWND window = nullptr;
HWND toolbar1 = nullptr;
HWND textBox1 = nullptr;
WNDPROC defWndProc = nullptr;
HIMAGELIST imageList1 = nullptr;

enum class IdMenu {
	New = 1,
	Open,
	Save,
	Separator1,
	Cut, 
	Copy, 
	Paste,
	Separator2,
	Print,
	Separator3,
	About,
};

LRESULT OnWindowClose(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PostQuitMessage(0);

	return CallWindowProc(defWndProc, hWnd, message, wParam, lParam);
}

LRESULT OnWindowResize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	LRESULT result = CallWindowProc(defWndProc, hWnd, message, wParam, lParam);
	RECT rect;

	GetClientRect(window, &rect);
	SetWindowPos(textBox1, HWND_TOP, 0, 44, rect.right - rect.left, rect.bottom - rect.top - 44, 0);
	
	return result;
}

wstring GetText(HWND hWnd) {
	size_t size = SendMessage(hWnd, WM_GETTEXTLENGTH, 0, 0);
	if (!size) {
		return L"";
	}
	wstring text(size, '\0');
	SendMessage(hWnd, WM_GETTEXT, (WPARAM)text.size() + 1, (LPARAM)text.c_str());

	return text;
}

void AppendLine(HWND hWnd, const wstring& text) {
	wstring textTextBox1 = GetText(hWnd) + text + L"\r\n"s;
	SetWindowText(hWnd, textTextBox1.c_str());
}

LRESULT OnToolBarItemClick(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static map<IdMenu, std::wstring> items = { {IdMenu::New, L"�������"}, {IdMenu::Open, L"�������"}, {IdMenu::Save, L"���������"}, {IdMenu::Cut, L"��������"}, {IdMenu::Copy, L"����������"}, {IdMenu::Paste, L"��������"}, {IdMenu::Print, L"������"}, {IdMenu::About, L"� ���������"} };
	auto it = items.find(static_cast<IdMenu>(LOWORD(wParam)));

	AppendLine(textBox1, (it != items.end() ? it->second : L"(unknown item)"s) + L" = ������ ��������"s);

	return CallWindowProc(defWndProc, hWnd, message, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if (message == WM_CLOSE && hWnd == window) {
		return OnWindowClose(hWnd, message, wParam, lParam);
	}
	if (message == WM_SIZE && hWnd == window) {
		return OnWindowResize(hWnd, message, wParam, lParam);
	}
	if (message == WM_COMMAND && HIWORD(wParam) == BN_CLICKED && reinterpret_cast<HWND>(lParam) == toolbar1) {
		return OnToolBarItemClick(hWnd, message, wParam, lParam);
	}


	return CallWindowProc(defWndProc, hWnd, message, wParam, lParam);
}

int main() {
	constexpr size_t ImageListID = 0;
	vector<TBBUTTON>toolbarButtons{
		{ MAKELONG(STD_FILENEW, ImageListID), static_cast<int32_t>(IdMenu::New), TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, (INT_PTR)L"&�������"},
		{ MAKELONG(STD_FILEOPEN, ImageListID), static_cast<int32_t>(IdMenu::Open), TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, (INT_PTR)L"&�������..."},
		{ MAKELONG(STD_FILESAVE, ImageListID), static_cast<int32_t>(IdMenu::Save), TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, (INT_PTR)L"&���������"},
		{ MAKELONG(0, ImageListID), static_cast<int32_t>(IdMenu::Separator1), 0, BTNS_SEP, {0}, 0, (INT_PTR)L""},
		{ MAKELONG(STD_CUT, ImageListID), static_cast<int32_t>(IdMenu::Cut), TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, (INT_PTR)L"&��������"},
		{ MAKELONG(STD_COPY, ImageListID), static_cast<int32_t>(IdMenu::Copy), TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, (INT_PTR)L"&����������"},
		{ MAKELONG(STD_PASTE, ImageListID), static_cast<int32_t>(IdMenu::Paste), TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, (INT_PTR)L"&��������"},
		{ MAKELONG(0, ImageListID), static_cast<int32_t>(IdMenu::Separator2), 0, BTNS_SEP, {0}, 0, (INT_PTR)L""},
		{ MAKELONG(STD_PRINT, ImageListID), static_cast<int32_t>(IdMenu::Print), TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, (INT_PTR)L"&������"},
		{ MAKELONG(0, ImageListID), static_cast<int32_t>(IdMenu::Separator3), 0, BTNS_SEP, {0}, 0, (INT_PTR)L""},
		{ MAKELONG(STD_HELP, ImageListID), static_cast<int32_t>(IdMenu::About), TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, (INT_PTR)L"&� ���������..."},
	};

	window = CreateWindowEx(0, WC_DIALOG, L"Toolbar example", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, nullptr, nullptr, nullptr, nullptr);
	toolbar1 = CreateWindowEx(0, TOOLBARCLASSNAME, nullptr, WS_CHILD | WS_VISIBLE | TBSTYLE_WRAPABLE, 0, 0, 0, 0, window, nullptr, nullptr, nullptr);
	textBox1 = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, L"", WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_READONLY | ES_MULTILINE | ES_WANTRETURN, 10, 10, 100, 23, window, nullptr, nullptr, nullptr);
	imageList1 = ImageList_Create(16, 16, ILC_COLOR16 | ILC_MASK, static_cast<int32_t>(toolbarButtons.size()), 0);

	SendMessage(toolbar1, TB_SETIMAGELIST, ImageListID, reinterpret_cast<LPARAM>(imageList1));
	SendMessage(toolbar1, TB_LOADIMAGES, (WPARAM)IDB_STD_SMALL_COLOR, (LPARAM)HINST_COMMCTRL);
	SendMessage(toolbar1, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(toolbar1, TB_ADDBUTTONS, (WPARAM)toolbarButtons.size(), (LPARAM)toolbarButtons.data());
	SendMessage(toolbar1, TB_AUTOSIZE, 0, 0);

	defWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
	ShowWindow(window, SW_SHOW);

	MSG message = { 0 };
	while (GetMessage(&message, nullptr, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return (int)message.wParam;
}