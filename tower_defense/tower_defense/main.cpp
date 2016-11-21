#include "Engine.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	Engine* engine;
	bool result;

	// Create the system object.
	engine = new Engine;
	if(!engine)	return 0;

	// Initialize and run the system object.
	result = engine->Initialize();
	if(result) engine->Run();

	// Shutdown and release the system object.
	engine->Shutdown();
	delete engine;
	engine = 0;

	return 0;
}