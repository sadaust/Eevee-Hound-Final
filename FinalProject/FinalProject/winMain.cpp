#include <Windows.h>

//windowed res
#define W_SCREEN_WIDTH 800
#define W_SCREEN_HEIGHT 600

#define WINDOW_TITLE "H.A.L.T."
int SCREEN_WIDTH;
int SCREEN_HEIGHT;
int					g_windowCount;  //keeps track of how many windows we have
HWND				g_hWnd;			// Handle to the window
HINSTANCE			g_hInstance;	// Handle to the application instance
bool				g_bWindowed;	// Boolean for windowed or full-screen
RECT				g_winRect;
LONG				g_wStyle;		//window style
LONG				g_fStyle;		//fullscreen style

// Entry point for the game or application.
int WINAPI WinMain(HINSTANCE hInstance,	// Handle to the application
	HINSTANCE hPrevInstance,	// Handle to the previous app
	LPTSTR lpCmdLine,		// Command line string
	int lpCmdShow);			// Show window value

LRESULT CALLBACK WndProc(HWND hWnd,			// Handle to the window
	UINT message,		// Incoming Message
	WPARAM wparam,		// Message Info
	LPARAM lparam);	// Message Info

void InitWindow(void)
{
	WNDCLASSEX wndClass;  
	ZeroMemory(&wndClass, sizeof(wndClass));
	// set up the window
	wndClass.cbSize			= sizeof(WNDCLASSEX);			// size of window structure
	wndClass.lpfnWndProc	= (WNDPROC)WndProc;				// message callback
	wndClass.lpszClassName	= WINDOW_TITLE;					// class name
	wndClass.hInstance		= g_hInstance;					// handle to the application
	wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);	// default cursor
	//wndClass.hbrBackground	= (HBRUSH)(COLOR_WINDOWFRAME);	// background brush
	SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);
	SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN);
	// register a new type of window
	RegisterClassEx(&wndClass);
	DWORD style;
	int height, width;
	g_wStyle = g_fStyle = 0;

	g_wStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	g_fStyle = WS_POPUP;
	if(g_bWindowed) {
		style = g_wStyle;
		width = W_SCREEN_WIDTH;
		height = W_SCREEN_HEIGHT;
	}
	else {
		style = g_fStyle;
		width = SCREEN_WIDTH;
		height = SCREEN_HEIGHT;
	}

	g_hWnd = CreateWindowEx(
		NULL,
		WINDOW_TITLE, WINDOW_TITLE, 							// window class name and title
		style,													// window style
		CW_USEDEFAULT, CW_USEDEFAULT,							// x and y coordinates
		width, height,											// width and height of window
		NULL, NULL,												// parent window and menu
		g_hInstance,											// handle to application
		NULL);

	// Display the window
	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);
	g_windowCount = 1;
}
//creates a new window and destroyes the old one
void changeWindow()
{
	DWORD style;
	int height, width;
	HWND old = g_hWnd;
	g_hWnd = 0;
	if(g_bWindowed) {
		style = g_wStyle;
		width = W_SCREEN_WIDTH;
		height = W_SCREEN_HEIGHT;
	}
	else {
		style = g_fStyle;
		width = SCREEN_WIDTH;
		height = SCREEN_HEIGHT;
	}
	g_hWnd = CreateWindowEx(
		NULL,
		WINDOW_TITLE, WINDOW_TITLE, 							// window class name and title
		style,													// window style
		CW_USEDEFAULT, CW_USEDEFAULT,							// x and y coordinates
		width, height,											// width and height of window
		NULL, NULL,												// parent window and menu
		g_hInstance,											// handle to application
		NULL);
	// Display the window
	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);
	++g_windowCount;
	DestroyWindow(old);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow ) {
	g_hInstance = hInstance;	// Store application handle
	g_bWindowed = true;		// Windowed mode or full-screen
	bool run = true;
	// Init the window
	InitWindow();
	MSG msg; 
	ZeroMemory(&msg, sizeof(msg));
	//Main Loop
	while(run) { 
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if(msg.message == WM_QUIT)
				run = false;
		}
	}
	// Unregister window
	UnregisterClass(WINDOW_TITLE, g_hInstance);
	// Return successful
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	// attempt to handle your messages
	switch(message)
	{
	case (WM_PAINT):
		{
			InvalidateRect(hWnd,&g_winRect,TRUE);
			break;
		}		
	case(WM_DESTROY):
		{
			PostQuitMessage(0);
			break;
		}
		return DefWindowProc(hWnd, message, wparam, lparam);
	}
	// pass to default handler
	return DefWindowProc(hWnd, message, wparam, lparam);
}