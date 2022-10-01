#include <stdio.h>
#include <windows.h>

#define GAMENAME "GameName"
//use OutputDebugStirng() for dev comments

//Predeclaration call
LRESULT CALLBACK MainWindowProcedure(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM wParam, _In_ LPARAM lParam);
BOOL isOneGame(void);

int WinMain(HINSTANCE Instance,HINSTANCE prevInstance, PWSTR CommandLine, int CmdShow) {
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(CommandLine);
	UNREFERENCED_PARAMETER(CmdShow);

	if (isOneGame()) {
		return(0);
	}

	HWND WindowHandle = 0;

	WNDCLASSEXA WindowClass = { 0 };
	WindowClass.cbSize = sizeof(WNDCLASSEXA);
	WindowClass.cbSize = sizeof(WNDCLASSEXA);
	WindowClass.style = 0;
	WindowClass.lpfnWndProc = MainWindowProcedure;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = Instance;
	WindowClass.hIcon = LoadIconA(NULL, IDI_APPLICATION);
	WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = GAMENAME "_WindowClass";
	WindowClass.hIconSm = LoadIconA(NULL, IDI_APPLICATION);

	if (!RegisterClassExA(&WindowClass)) {
		MessageBoxA(NULL, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
		return (0);
	}

	WindowHandle = CreateWindowExA(WS_EX_CLIENTEDGE, WindowClass.lpszClassName, GAMENAME, WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 240, 120, NULL, NULL, Instance, NULL);

	if (WindowHandle == NULL) {
		MessageBoxA(NULL, "Window Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
		return(0);
	}

	MSG Message = { 0 };
	while (GetMessageA(&Message, NULL, 0, 0) > 0) {
		TranslateMessage(&Message);
		DispatchMessageA(&Message);
	}

	return 0;
}

LRESULT CALLBACK MainWindowProcedure(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM wParam, _In_ LPARAM lParam) {
	LRESULT result = 0;
	switch (Message) {
		case WM_CLOSE:
			DestroyWindow(WindowHandle);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			result = DefWindowProcA(WindowHandle, Message, wParam, lParam);
	}
	return result;
}

BOOL isOneGame(void) {
	HANDLE Mutex = NULL;

	Mutex = CreateMutexA(NULL, FALSE, GAMENAME);

	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		return(TRUE);
	}else {
		return(FALSE);
	}
}