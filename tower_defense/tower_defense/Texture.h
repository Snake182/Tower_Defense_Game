#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#include "StructsAndNamespaces.h"


class Texture
{
private:

public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool LoadImageFile(char* path, int& width, int& height, int& rowPitch, FIBITMAP*& imagePointer);

private:
	unsigned char* m_imageFileData;
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView;

};

#endif