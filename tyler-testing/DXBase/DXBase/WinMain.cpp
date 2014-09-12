#include <Windows.h>
#include "DXTest.h"
//windowed res
#define W_SCREEN_WIDTH 800
#define W_SCREEN_HEIGHT 600

#define WINDOW_TITLE "DX Base"
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	g_hInstance = hInstance;	// Store application handle
	g_bWindowed = true;		// Windowed mode or full-screen
	bool run = true;
	// Init the window
	InitWindow();
	MSG msg; 
	ZeroMemory(&msg, sizeof(msg));
	//start directX test
	/*DXFrame test;
	test.init(g_hWnd,g_hInstance,g_bWindowed);
	TextureStruct testText;
	test.Load2D("uvtest.png",0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_XRGB(255,0,0),&testText.texInfo,0,&testText.objTex);
	cam temp,temp2,temp3,temp4;
	temp.cam_look_pos.x = 0;
	temp.cam_look_pos.y = 0;
	temp.cam_look_pos.z = 0;
	temp.cam_pos.x = 5;
	temp.cam_pos.y = 0;
	temp.cam_pos.z = 0;
	temp.cam_up_vec.x = 0;
	temp.cam_up_vec.y = 1;
	temp.cam_up_vec.z = 0;
	temp.drawDist = 200.0f;
	temp.fov_deg = 90.0f;

	temp2.cam_look_pos.x = 0;
	temp2.cam_look_pos.y = 0;
	temp2.cam_look_pos.z = 0;
	temp2.cam_pos.x = 0;
	temp2.cam_pos.y = 0;
	temp2.cam_pos.z = 5;
	temp2.cam_up_vec.x = 0;
	temp2.cam_up_vec.y = 1;
	temp2.cam_up_vec.z = 0;
	temp2.drawDist = 200.0f;
	temp2.fov_deg = 90.0f;

	temp3 = temp4 = temp;
	
	temp3.cam_pos.x = 0;
	temp3.cam_pos.z = -2;

	temp4.cam_pos.x = -2;

	test.setViewCount(4);
	//test.setViewCount(2);
	test.toggleSS();
	test.setCam(1,&temp);
	test.setCam(2,&temp2);
	test.setCam(3,&temp3);
	test.setCam(4,&temp4);

	D3DMATERIAL9 testMat;
	testMat.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	testMat.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		// Diffuse color reflected
	testMat.Emissive = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);		// Emissive color reflected
	testMat.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		// Specular
	testMat.Power = 0.0f;
	PrimObj testCube;
	testCube.mat = &testMat;
	testCube.objTex = &testText.objTex;
	D3DXMatrixIdentity(&testCube.matrix);
	test.CreateUVCube(testCube,1,1,1);

	RenInfo testRen;
	testRen.asset = &testCube;
	testRen.type  =	primitive;
	testRen.locCamNum = 0;

	test.addRen(testRen);
	*/
	DXTest test;
	test.init(g_hWnd,g_hInstance,g_bWindowed);
	//Main Loop
	while(run)
	{ 
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				run = false;
			else if(msg.message == WM_KEYDOWN)
			{
				switch(msg.wParam)
				{
				case ' ':
					//g_bWindowed = !g_bWindowed;
					//changeWindow();
					/*test.reSize(g_hWnd,g_hInstance,g_bWindowed);
					if(!g_bWindowed) {
					testText.objTex->Release();
					test.Load2D("uvtest.png",0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_XRGB(255,0,0),&testText.texInfo,0,&testText.objTex);
					testCube.obj->Release();
					test.CreateUVCube(testCube,1,1,1);
					//testCube.objTex = &testText.objTex;
					test.clearRen();
					test.addRen(testRen);
					test.setCam(1,&temp);
					test.setCam(2,&temp2);
					}*/
					break;
				/*case VK_ADD:
					temp.fov_deg++;
					test.setCam(1,&temp);
					break;
				case VK_SUBTRACT:
					temp.fov_deg--;
					test.setCam(1,&temp);
					break;*/
				}
			}
			DispatchMessage(&msg);
		}
		test.update();
		if(test.devLost()) {
			test.resetDev(g_hWnd,g_hInstance);
		}
		test.draw();
		/*test.Render();
		if(test.rendererLost())
		{
			test.reSize(g_hWnd,g_hInstance,g_bWindowed);
			if(!test.rendererLost()) {
				testText.objTex->Release();
				test.Load2D("uvtest.png",0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_XRGB(255,0,0),&testText.texInfo,0,&testText.objTex);
				testCube.obj->Release();
				test.CreateUVCube(testCube,1,1,1);
				//testCube.objTex = &testText.objTex;
				test.clearRen();
				test.addRen(testRen);
				test.setCam(1,&temp);
				test.setCam(2,&temp2);
			}
		}*/
	}
	/*testText.objTex->Release();
	testCube.obj->Release();
	testCube.objInd->Release();
	testCube.objDec->Release();
	//shutdown directX
	test.Shutdown();*/
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
			//decrement the window count due to one being destroyed
			//--g_windowCount;
			//if we have no windows quit
			//if(g_windowCount <= 0)
				PostQuitMessage(0);
			break;
		}
		return DefWindowProc(hWnd, message, wparam, lparam);
	}
	// pass to default handler
	return DefWindowProc(hWnd, message, wparam, lparam);
}