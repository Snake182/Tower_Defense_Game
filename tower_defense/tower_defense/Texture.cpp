#include "texture.h"


Texture::Texture()
{
	m_imageFileData = 0;
	m_texture = 0;
	m_textureView = 0;
}


Texture::Texture(const Texture& other)
{
}


Texture::~Texture()
{
}


bool Texture::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;
	int height, width;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hResult;
	int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	FIBITMAP * imagePointer;

	// Load the targa image data into memory.
	result = LoadImageFile(filename, width, height, rowPitch, imagePointer);
	if(!result)
	{
		return false;
	}

	// Setup the description of the texture.
	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// Create the empty texture.
	hResult = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
	if(FAILED(hResult))
	{
		return false;
	}

	// Set the row pitch of the targa image data.
	rowPitch = (width * 4) * sizeof(unsigned char);

	// Copy the targa image data into the texture.
	deviceContext->UpdateSubresource(m_texture, 0, NULL, m_imageFileData, rowPitch, 0);

	// Setup the shader resource view description.
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	// Create the shader resource view for the texture.
	hResult = device->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView);
	if(FAILED(hResult))
	{
		return false;
	}

	// Generate mipmaps for this texture.
	deviceContext->GenerateMips(m_textureView);

	FreeImage_Unload(imagePointer);

	return true;
}


void Texture::Shutdown()
{
	// Release the texture view resource.
	if(m_textureView)
	{
		m_textureView->Release();
		m_textureView = 0;
	}

	// Release the texture.
	if(m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	// Release the targa data.
	if(m_imageFileData)
	{
		delete [] m_imageFileData;
		m_imageFileData = 0;
	}

	return;
}


ID3D11ShaderResourceView* Texture::GetTexture()
{
	return m_textureView;
}


bool Texture::LoadImageFile(char* path, int& width, int& height, int& rowPitch, FIBITMAP*& imagePointer)
{
	FREE_IMAGE_FORMAT imageFormat = FIF_UNKNOWN;
	unsigned int bpp;

	imagePointer = 0;
	imageFormat = FreeImage_GetFileType(path, 0);

	if (imageFormat == FIF_UNKNOWN)
		imageFormat = FreeImage_GetFIFFromFilename(path);
	if (imageFormat == FIF_UNKNOWN)
		return false;

	if (FreeImage_FIFSupportsReading(imageFormat))
		imagePointer = FreeImage_Load(imageFormat, path);

	if (imagePointer == nullptr) return false;

	bpp = FreeImage_GetBPP(imagePointer);
	if (bpp != 32)
	{
		FIBITMAP* temp = FreeImage_ConvertTo32Bits(imagePointer);
		FreeImage_Unload(imagePointer);
		imagePointer = temp;
	}

	if (!FreeImage_FlipVertical(imagePointer)) return false;

	width = FreeImage_GetWidth(imagePointer);
	height = FreeImage_GetHeight(imagePointer);
	rowPitch = FreeImage_GetPitch(imagePointer);

	m_imageFileData = (unsigned char*)FreeImage_GetBits(imagePointer);

	if ((m_imageFileData == 0) || (width == 0) || (height == 0))
		return false;

	return true;
}