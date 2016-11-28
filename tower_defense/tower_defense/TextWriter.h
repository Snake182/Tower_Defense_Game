#pragma once
#include "StructsAndNamespaces.h"

class TextWriter
{
public:
	TextWriter();
	TextWriter(const TextWriter&);
	~TextWriter();

	bool Initialize(ID3D11Device* pdevice);
	void drawText(ID3D11DeviceContext* pContext);
	void Shutdown();

private:

	IFW1Factory *pFW1Factory;
	IFW1FontWrapper *pFontWrapper;

};