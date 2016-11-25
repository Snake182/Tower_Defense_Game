#ifndef _MODEL_H_
#define _MODEL_H_

#include "texture.h"

template <typename T, unsigned S>
inline unsigned arraysize(const T(&v)[S]) { return S; }
static const float SCREEN_RATIO = 142.85;

class Model
{

public:
	Model(float, float, float, int, char*); //for unanimated objects
	Model(float, float, float, float, int, char*); //for animated objects
	Model(const Model&);
	Model();
	~Model();

	//game loop functions
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, int, int);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	//getters
	ID3D11ShaderResourceView* GetTexture();
	float getPosHorizontal() { return this->m_posHorizontal; }
	float getPosVertical() { return this->m_posVertical; }
	float getSpeedRatio() { return this->m_speed_ratio; }
	float getSize() { return this->m_size; }
	float getIsFullScreen() { return this->m_is_full_screen; }
	XMFLOAT3 getPosition() { return this->position; }
	char* getFilename() { return this->m_filename; }
	XMMATRIX getModelMatrix(XMMATRIX, XMMATRIX, XMMATRIX);

	//setters
	void setPosHorizontal(float posHorizontal) { this->m_posHorizontal = posHorizontal; }
	void setPosVertical(float posVertical) { this->m_posVertical = posVertical;	}
	void setSize(float size) { this->m_size = size;	}
	void setIsFullScreen(int is_full_screen) { this->m_is_full_screen = is_full_screen; }
	void setSpeedRatio(float speed_ratio) {	this->m_speed_ratio = speed_ratio; }
	void setFilename(char* filename) { this->m_filename = filename; }

	//other
	void changePosition(XMFLOAT3);
private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount;
	float m_posHorizontal;
	float m_posVertical;
	float m_size;
	float m_speed_ratio;
	int m_is_full_screen;
	char* m_filename;
	Texture* m_Texture;
	int m_screenWidth;
	int m_screenHeight;

	XMFLOAT3 position;
};

#endif