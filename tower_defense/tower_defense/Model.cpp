#include "model.h"


Model::Model(float posHorizontal, float posVertical, float size, char* filename)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_posHorizontal = posHorizontal;
	m_posVertical = posVertical;
	m_size = size;
	m_filename = filename;
	position.x = 0;
	position.y = 0;
	position.z = 0;
}

Model::Model(float posHorizontal, float posVertical, float size, float speed_ratio, char* filename)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_posHorizontal = posHorizontal;
	m_posVertical = posVertical;
	m_size = size;
	m_filename = filename;
	m_speed_ratio = speed_ratio;
	position.x = 0;
	position.y = 0;
	position.z = 0;
}


Model::Model(const Model& other)
{
}

Model::Model()
{
}


Model::~Model()
{
}


bool Model::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename)
{
	bool result;


	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
	{
		MessageBox(NULL, L"Could not initialize the texture object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void Model::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}


XMMATRIX Model::getModelMatrix(XMMATRIX world, XMMATRIX view, XMMATRIX projection) {
	XMMATRIX translation = XMMatrixTranslationFromVector(XMLoadFloat3(&position));
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(0.f, 0.f, 0.f);
	XMMATRIX scaling = XMMatrixScaling(1.f, 1.f, 1.f);

	XMMATRIX modelMatrix = XMMatrixMultiply(scaling, rotation);
	modelMatrix = XMMatrixMultiply(modelMatrix, translation);
	modelMatrix = XMMatrixMultiply(modelMatrix, world);

	return modelMatrix;
}

void Model::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

ID3D11ShaderResourceView* Model::GetTexture()
{
	return m_Texture->GetTexture();
}


bool Model::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set the number of indices in the index array.
	const int index_count = 6;

	// Create the vertex array.
	vertices = new VertexType[index_count];
	if(!vertices)
	{
		return false;
	}

	
	// Create the index array.
	indices = new unsigned long[index_count];
	if(!indices)
	{
		return false;
	}

	// Load the vertex array with data.
	//triangle 1
	vertices[0].position = XMFLOAT3(-m_size + m_posHorizontal, -m_size + m_posVertical, 0.0f);  // Bottom left.
	vertices[0].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(-m_size + m_posHorizontal, m_size + m_posVertical, 0.0f);  // Top left.
	vertices[1].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[2].position = XMFLOAT3(m_size + m_posHorizontal, m_size + m_posVertical, 0.0f);  // Top Right.
	vertices[2].texture = XMFLOAT2(1.0f, 0.0f);

	//triangle 2
	vertices[3].position = XMFLOAT3(m_size + m_posHorizontal, m_size + m_posVertical, 0.0f);  // Top Right.
	vertices[3].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[4].position = XMFLOAT3(m_size + m_posHorizontal, -m_size + m_posVertical, 0.0f);  // Bottom Right.
	vertices[4].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[5].position = XMFLOAT3(-m_size + m_posHorizontal, -m_size + m_posVertical, 0.0f);  // Bottom left.
	vertices[5].texture = XMFLOAT2(0.0f, 1.0f);

	// Load the index array with data.
	for (int i = 0; i < index_count; i++) {
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * index_count;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * index_count;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

bool Model::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new Texture;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void Model::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

void Model::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void Model::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

//change position to the new one by checking if it doesn't exit the window
void Model::changePosition(XMFLOAT3 direction/*, int screenWidth, int screenHeight*/)
{
	/*float x = this->position.x;
	float y = this->position.y;
	float new_x = this->position.x + direction.x;
	float new_y = this->position.y + direction.y;

	if ((new_x > (-screenWidth / 2.f)) && (new_x < (screenWidth / 2.f))) {
		x = new_x;
	}
	if ((new_y > (-screenHeight / 2.f)) && (new_y < (screenHeight / 2.f))) {
		y = new_y;
	}*/
	
	float x = this->position.x + direction.x;
	float y = this->position.y + direction.y;
	float z = this->position.z + direction.z;

	this->position = XMFLOAT3(x, y, z);
}
