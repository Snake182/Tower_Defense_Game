#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "textureshader.h"
#include "d3d.h"
#include "ObjectManager.h"

// GLOBALS //
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Graphics
{
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool Initialize(D3D *);
	void Shutdown();
	bool Frame(ObjectManager *, D3D*);

private:
	bool Render(ObjectManager *, D3D*);

private:

	TextureShader* m_TextureShader;
};

#endif