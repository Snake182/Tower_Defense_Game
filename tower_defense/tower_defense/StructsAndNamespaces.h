#pragma once

//includes
#include <d3d11.h>
#include <directxmath.h>
#include <stdio.h>
#include <gl/gl.h>
#include "FreeImage.h"
#include "FW1FontWrapper.h"
#include <windows.h>
#include <d3dcompiler.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

//links
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

//namespaces
using namespace DirectX;
using namespace std;

//structs
struct VertexType
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
};

struct MatrixBufferType
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};