#include "TextWriter.h"

TextWriter::TextWriter()
{
}

TextWriter::TextWriter(const TextWriter &)
{
}

TextWriter::~TextWriter()
{
}

bool TextWriter::Initialize(ID3D11Device* pdevice)
{
	HRESULT hResult = FW1CreateFactory(FW1_VERSION, &pFW1Factory);
	hResult = pFW1Factory->CreateFontWrapper(pdevice, L"Arial", &pFontWrapper);
	if (SUCCEEDED(hResult)) {
		return true;
	}
	return false;
}

void TextWriter::drawText(ID3D11DeviceContext* pContext)
{
	pFontWrapper->DrawString(
		pContext,
		L"Text",// String
		128.0f,// Font size
		100.0f,// X
		50.0f,// Y
		0xff0099ff,// Text color
		FW1_RESTORESTATE// Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}

void TextWriter::Shutdown()
{
	if (pFontWrapper)
	{
		pFontWrapper->Release();
		pFontWrapper = 0;
	}
	if (pFW1Factory)
	{
		pFW1Factory->Release();
		pFW1Factory = 0;
	}
}
