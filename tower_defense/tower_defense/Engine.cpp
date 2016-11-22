#include "Engine.h"

Engine::Engine()
{
	m_Input = 0;
	m_Graphics = 0;
	level_number = 1;
}


Engine::Engine(const Engine& other)
{
}


Engine::~Engine()
{
}


bool Engine::Initialize(int level_number)
{
	bool result;

	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the Direct3D object.
	m_Direct3D = new D3D;
	if (!m_Direct3D) return false;

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR, m_hwnd);
	if (!result)
	{
		MessageBox(NULL, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = new Input;
	if(!m_Input) return false;

	// Initialize the input object.
	m_Input->Initialize();

	// object manager, will create the objects
	objManager = new ObjectManager;
	if (!objManager) return false;

	//initialize object manager with the first level
	result = objManager->initialize(m_Direct3D, level_number);
	if (!result) return false;

	// Create the graphics object. This object will handle rendering all the graphics for this application.
	m_Graphics = new Graphics;
	if(!m_Graphics)	return false;

	// Initialize the graphics object.
	result = m_Graphics->Initialize(m_Direct3D);
	if(!result)	return false;

	//draw text
	this->drawText(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext());

	return true;
}


void Engine::Shutdown()
{
	// Release the object manager.
	if (objManager)
	{
		delete objManager;
		objManager = 0;
	}

	// Release the graphics object.
	if(m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Release the input object.
	if(m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	// Shutdown the window.
	ShutdownWindows();
	
	return;
}


void Engine::Run()
{
	MSG msg;
	bool done, result;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	
	// Loop until there is a quit message from the window or the user.
	done = false;
	while(!done)
	{
		// Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if(msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame(objManager);
			if(!result)
			{
				done = true;
			}
		}
	}

	return;
}


bool Engine::Frame(ObjectManager * objManager)
{
	bool result;

	// Check if the user pressed escape and wants to exit the application.
	if(m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// player movement.
	if (m_Input->IsKeyDown(VK_UP))
	{
		objManager->getPlayer()->getModel()->changePosition(XMFLOAT3(0, 0.1f * objManager->getPlayer()->getModel()->getSpeedRatio(), 0));
	}
	if (m_Input->IsKeyDown(VK_RIGHT))
	{
		objManager->getPlayer()->getModel()->changePosition(XMFLOAT3(0.1f * objManager->getPlayer()->getModel()->getSpeedRatio(), 0, 0));
	}
	if (m_Input->IsKeyDown(VK_DOWN))
	{
		objManager->getPlayer()->getModel()->changePosition(XMFLOAT3(0, -0.1f * objManager->getPlayer()->getModel()->getSpeedRatio(), 0));
	}
	if (m_Input->IsKeyDown(VK_LEFT))
	{
		objManager->getPlayer()->getModel()->changePosition(XMFLOAT3(-0.1f * objManager->getPlayer()->getModel()->getSpeedRatio(), 0, 0));
	}
	//conditions for new level or previous level
	if (m_Input->IsKeyDown(0x4E)) //N key, as in Next
	{
		this->level_number += 1;
		Initialize(this->level_number);
	}
	if (m_Input->IsKeyDown(0x50)) //P key as in Previous
	{
		this->level_number -= 1;
		Initialize(this->level_number);
	}

	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame(objManager, m_Direct3D);
	if(!result)
	{
		return false;
	}

	return true;
}


LRESULT CALLBACK Engine::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch(umsg)
	{
		// Check if a key has been pressed on the keyboard.
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}

		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}

		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}


void Engine::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Engine";

	// Setup the windows class with default settings.
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hinstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize        = sizeof(WNDCLASSEX);
	
	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth  = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, 
						    WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
						    posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}


void Engine::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);//test
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}

void Engine::drawText(ID3D11Device *pDevice, ID3D11DeviceContext *pContext) {
	IFW1Factory *pFW1Factory;
	HRESULT hResult = FW1CreateFactory(FW1_VERSION, &pFW1Factory);

	IFW1FontWrapper *pFontWrapper;
	hResult = pFW1Factory->CreateFontWrapper(pDevice, L"Arial", &pFontWrapper);
	
	if (SUCCEEDED(hResult))
	{
		pFontWrapper->DrawString(
			pContext,
			L"Text",// String
			128.0f,// Font size
			100.0f,// X position
			50.0f,// Y position
			0xff0099ff,// Text color, 0xAaBbGgRr
			0// Flags (for example FW1_RESTORESTATE to keep context states unchanged)
		);

		pFontWrapper->Release();
		pFW1Factory->Release();
	}
}