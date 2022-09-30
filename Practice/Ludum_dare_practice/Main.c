//BufferOver info and warnings
/*
*Inside the C language, and subsequent libraries, there are a lot of depreciated functions that do not balance check and can cause overflows.
* A good example of this is memcpy, in which we won't get a warning not to use if we set warnings for windows.h to 0.
* So, just make sure that your external includes, or the windows.h file has warnings set to 3 at all times.
* Also, if you find yourself using a depreciated function, use the _s version of it so there is no warning. i.e. memcpy_s()
*/

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
HINSTANCE - It is essentially another identifier to the window class. In the main function, it would be a identifier to the main application. It is imparative to use if multiple window class's have the same name. ALso, it kinda works like a handle as well.
^^ in reference to HINSTANCE, you can just use HANDLE for the declaration(not recommended), or you can use get getModuleHandleA(NULL(supposed to be Instance name)) to reference it. 
PWSTR - Essentially a macro for a long pointer to a string
hInstance(Instance) - A handle used to identify the .exe when loaded in memory
hPrevInstance(PreviousInstance) - depreciated function, left at 0
lpCmdLine(CommandLine) - contains the command-line arguments as Unicode String(Since we switched to ASCII it will be ASCII)
nCmdShow(CmdShow) - This is a flag that says weither the application will be minimized, maximized, or shown normally
(!)What is a handle? - A handle is a token that represents a resource that is managed by the Windows Kernal.(It  can be a window, a file, etc... It is a simeple way of identifying a particulate resource that you want to work with)
(0)In Cpp, if you aren't going to use the parameters you can just place the data type without a identifier.
(1)If a datatype is a data struct and you wanna intialize to all zero do = { 0 }
(2)You can do UNREFERENCED_PARAMETER((Your_VAR)) To tell the compiler that the parameter is unreferenced
(3)If you place a #define string right next to a stirng, it automatically concantanates the two together. So NAME "_Thing" works.
(4)If you create a buffer like char buf[12] = {0}, and want to convert it from integer to ascii do _itoa_s(). This is useful for outputting the window messages.
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
	^^ if the handle is null, it will recieve messages from any window that belongs on the current thread. Furthurmore, both window messages and thread messags will be processed.
	-The TranslateMessgae() function "Translates virtual-key messages into character messages. The character messages are posted to the calling thread's message queue, to be read the next time the thread calls the GetMessage or PeekMessage function."
	-The DispatchMessage() function "Dispatches a message to a window procedure"(Goes to the connected Window Procedure)
	-Code, besides cases, inside of a switch statement does not work.(duhh)
(4)MAIN WINDOW PROCEDURE:
	CALLBACK - MACRO to _sdtdcall which is not the default calling convention on most compilers but is needed.
	_sdtdcaall - used to call Win32 API functions. - This is Microsoft specific.(on x64 it is essentially ignored for sake of running)
	LRESULT - Signed result of message processing
	-The function in itself sorta handles actions taken by the user on the window itself.
	-The WM_CLOSE and others like it are MACROS to Msg action codes.

	*Make sure to check the documentation of the Action codes like WM_CLOSE before returning them. They have specific return types for the functions they do in the program. 

What if we do not want multiple copies of the game running at once?
(*) We are first going to create a BOOL function called "GameIsAlreadyRunning(void)"
(*) Then, we are creating what is called a Mutex.
Mutex - Show for Mutual Exclusion, the mutex is used to control mutually exclusive acess to a handle or shared resource.

AFTER Window CREATION - Game info
(*) So, we cannot keep doing the getMessage(), this is due to the fact that it stops the program entirely before recieving another message.
^^ To midigate this, we are going to be using the PeekMessage() function in the while loop instead.
(*) For game creation, this is the levels how how things are done:
(1) Window Creation.
(2) There is the main game loop inside of the main function.
(3) The messages are handled in case the user want's to exit from such.
(4) The live input of the user is processed.
(5) The game gets rendered to the screen.
(6) We do some cool black magic stuff to make sure that the game is not taking up a ton of CPU or GPU and is running smoothely.
*/

//Information from where I left off.
/*
For doing this, I am at Ryan Ries EP0005: https://www.youtube.com/watch?v=aQ4w5iVh4UE&list=PLlaINRtydtNWuRfd4Ra3KeD6L9FP_tDE7&index=5
Ludum Dare site: https://ldjam.com/events/ludum-dare/51/
Very helpful site if I just wanted to get right into certain content: http://winprog.org/tutorial/simple_window.html
Look at this if you want, don't really know what it would do for you: https://stackoverflow.com/questions/50587293/switching-from-opengl-to-gdi
*/

//SAL (Source annotation language)
/*
* By using SAL, you can give the compiler some hints on how to use a parameter. Make's your code more efficent.
* (_In_) - says that the parameter is an input variable.
* (_Inout_) - says that the parameter is both a input and a output.
* (_Out_) - says that the parameter is an output variable
* (*)There is more information on this language here: https://learn.microsoft.com/en-us/cpp/code-quality/understanding-sal?view=msvc-170
*/

//This is soley a predeclaration reference to the MainWindowProcedure function.
LRESULT CALLBACK MainWindowProcedure(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM wParam, _In_ LPARAM lParam);

//Just a call to the function
BOOL GameIsAlreadyRunning(void);

int WinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, PWSTR CommandLine, int CmdShow) {

	UNREFERENCED_PARAMETER(PreviousInstance);

	UNREFERENCED_PARAMETER(CommandLine);

	UNREFERENCED_PARAMETER(CmdShow);

	if (GameIsAlreadyRunning() == TRUE) {
		MessageBoxA(NULL, "Another instance of this program is already running!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return(0);
	}

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
LRESULT CALLBACK MainWindowProcedure(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	LRESULT result = 0;

	//What is the Messge that we recieved in dispatchMessage(&Message) and how do we handle it.
	switch (Message)
	{
		//I click the close button, it destroys the window
		case WM_CLOSE:
			//Removes the window and all of it's functionality
			DestroyWindow(WindowHandle);
			break;
		//The window is destroyed, quit out of the program
		case WM_DESTROY:
			//The PostQuitMessage essentially just queues a message that doesn't pass the while conditional and forces a false for the program to close.
			PostQuitMessage(0);
			break;
		//Default windows procs
		default:
			result = DefWindowProcA(WindowHandle, Message, wParam, lParam);
	}
	return result;
}

BOOL GameIsAlreadyRunning(void) {
	//Creation of the handle for the Mutex
	HANDLE Mutex = NULL;

	//Creation of the mutex
	Mutex = CreateMutexA(NULL, FALSE, "Name_of_program");

	//Since, possibly, there can only be a single instance of mutex, if there is a creation of another mutex that has the same name an error will occur.
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		return(TRUE);
	}else {
		return(FALSE);
	}
}