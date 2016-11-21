#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define WIN32_LEAN_AND_MEAN

#include "input.h"
#include "graphics.h"

class Engine
{
public:
	Engine();
	Engine(const Engine&);
	~Engine();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame(ObjectManager*);
	void InitializeWindows(int&, int&);
	void ShutdownWindows();
	void drawText(ID3D11Device *pDevice, ID3D11DeviceContext *pContext);
private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	Input* m_Input;
	Graphics* m_Graphics;
	ObjectManager* objManager;
	D3D* m_Direct3D;

	int screenWidth;
	int screenHeight;
};

// FUNCTION PROTOTYPES //
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// GLOBALS //
static Engine* ApplicationHandle = 0;


#endif