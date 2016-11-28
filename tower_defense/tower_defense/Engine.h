#pragma once

#define WIN32_LEAN_AND_MEAN

#include "input.h"
#include "graphics.h"
#include "TextWriter.h"

class Engine
{
public:
	Engine();
	Engine(const Engine&);
	~Engine();

	bool Initialize(int);
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame(ObjectManager*);
	void InitializeWindows(int&, int&);
	void ShutdownWindows();
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	Input* m_Input;
	Graphics* m_Graphics;
	ObjectManager* objManager;
	D3D* m_Direct3D;
	TextWriter* m_textWriter;

	int screenWidth;
	int screenHeight;
	int level_number;
};

// FUNCTION PROTOTYPES //
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// GLOBALS //
static Engine* ApplicationHandle = 0;
