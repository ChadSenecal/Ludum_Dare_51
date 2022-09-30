#include<stdio.h>
//The pragma warning supression system wasn't working, so I lowered the warning system from external includes in properties.
//https://developercommunity.visualstudio.com/t/pragma-warningpush-0-no-longer-works/1433518
#include<windows.h>

//Introductory Basic console app stuff
/* 
#define MAGICNUMBER 22

int addTwoIntegers(int first, int second) {
	return (first + second);
}

int main(int argc, char* argv[]) {
	int z = addTwoIntegers(3, 2);
	printf("Answer: %i\n", z);

	printf("My magic number: %i\n", MAGICNUMBER);
	
	OutputDebugStringA("Hello ASCII.\n");

	OutputDebugStringW(L"Hello Unicode.\n");

	OutputDebugString("Hello ASCII.\n");

	printf("Argument: %i\n", argc);
	if (argc > 1) {
		printf("- First Argument: %s\n", argv[1]);
	}
	
	return(0);
}*/

//NOTES:
/*
* DECLARATION
HINSTANCE - A hangle to a instance of the program
PWSTR - Essentially a macro for a long pointer to a string
hInstance(Instance) - A handle used to identify the .exe when loaded in memory
hPrevInstance(PreviousInstance) - depreciated function, left at 0
lpCmdLine(CommandLine) - contains the command-line arguments as Unicode String(Since we switched to ASCII it will be ASCII)
nCmdShow(CmdShow) - This is a flag that says weither the application will be minimized, macimized, or shown normally
(!)What is a handle? - A handle is a token that represents a resource that is managed by the Windows Kernal.(It  can be a window, a file, etc... It is a simeple way of identifying a particulate resource that you want to work with)
(0)In Cpp, if you aren't going to use the parameters you can just place the data type without a identifier.
(1)If a datatype is a data struct and you wanna intialize to all zero do = { 0 }
(2)You can do UNREFERENCED_PARAMETER((Your_VAR)) To tell the compiler that the parameter is unreferenced
(3)
* CONTENTS OF THE WINDOW
WNDCLASSEXA - It is a data struct that contains windows class information, and is to be used with RegisterClassEx and GetClassInfoEx functions
RegisterClassEX - Registers a window class for subsequent use in calls to the CreateWindow or CreateWindowEx function.(Or essentially just connects everything together)

* Steps at which stuff is done:
(1)DECLAREATION & REGISTRATION:
	HWND - The declaration for a handle data struct.
	-We are declare a empty resource handle for the creation of the window later on.
	-We declare a Main Window Procedure function that we do not necessarily need to fill yet.
	-We declare a Window Class where we will place some infromation about our window.
	-We will register the Window Class to make sure it's in(doesn't really specify what registration is)
(2)HANDLE TO WINDOW CONNECTION:
	-This is soley assigning the CreateWindowEx function to the Handle, with the CreateWindowEx fuction having proper parameters in palce.
	-You're gonna need this to use it more:https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
	-If you want to have two styles at once within the CreateWindowEx function do (style1) | (style2)
(3)MSG LOOP:
	MSG - "Contains message information from a thread's message queue"
	-The GetMessage() function "Retrieves a message from the calling thread's message queue. The function dispatches incoming sent messages until a posted message is available for retrieval."
	-The TranslateMessgae() function "Translates virtual-key messages into character messages. The character messages are posted to the calling thread's message queue, to be read the next time the thread calls the GetMessage or PeekMessage function."
	-The DispatchMessage() function "Dispatches a message to a window procedure"(Goes to the connected Window Procedure)
(4)MAIN WINDOW PROCEDURE:
	CALLBACK - MACRO to _sdtdcall which is not the default calling convention on most compilers but is needed.
	_sdtdcaall - used to call Win32 API functions. - This is Microsoft specific.(on x64 it is essentially ignored for sake of running)
	LRESULT - Signed result of message processing
	-The function in itself sorta handles actions taken by the user on the window itself.
	-The WM_CLOSE and others like it are MACROS to Msg action codes.
*/

//Information from where I left off.
/*
For doing this, I am at Ryan Ries EP0003 timestamp : 7 : 42 : https://www.youtube.com/watch?v=bED3Xqpcalo
Ludum Dare site: https://ldjam.com/events/ludum-dare/51/
Very helpful site if I just wanted to get right into certain content: http://winprog.org/tutorial/simple_window.html
Look at this if you want, don't really know what it would do for you: https://stackoverflow.com/questions/50587293/switching-from-opengl-to-gdi
*/

//This is soley a predeclaration reference to the MainWindowProcedure function.
LRESULT CALLBACK MainWindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, PWSTR CommandLine, int CmdShow) {

	UNREFERENCED_PARAMETER(PreviousInstance);

	UNREFERENCED_PARAMETER(CommandLine);

	//Creation of the handle for the window
	HWND WindowHandle = 0;

	//Creation of the Windows Class
	WNDCLASSEXA WindowClass = { 0 };
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
	WindowClass.lpszClassName = "Coolio";
	WindowClass.hIconSm = LoadIconA(NULL, IDI_APPLICATION);

	//Window Class Registration
	if (!RegisterClassExA(&WindowClass)) {
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	
	//This is the creation of the window with assigning it to a handle
	WindowHandle = CreateWindowExA(WS_EX_CLIENTEDGE,
		WindowClass.lpszClassName, "Window Title",WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
		NULL, NULL, Instance, NULL);

	if (WindowHandle == NULL) {
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	//Shows the actual graphical window itself(Not needed if the WS_VISIBLE style is added to CreateWindowEx)
	//ShowWindow(WindowHandle, CmdShow);

	//Updates the window with the handle after creation and assignment of window and handle.
	//UpdateWindow(WindowHandle);

	//This is the message loop of the window
	MSG Message = { 0 };
	while (GetMessageA(&Message, NULL, 0, 0) > 0) {
		TranslateMessage(&Message);
		DispatchMessageA(&Message);
	}

	return(0);
}


//This is the main window procedure.
LRESULT CALLBACK MainWindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//What is the Messge that we recieved in dispatchMessage(&Message) and how do we handle it.
	switch (msg)
	{
		//I click the close button, it destroys the window
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		//The window is destroyed, quit out of the program
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		//Default windows procs
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}