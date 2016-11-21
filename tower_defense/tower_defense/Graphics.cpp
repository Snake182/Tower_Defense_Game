#include "graphics.h"


Graphics::Graphics()
{
	m_TextureShader = 0;
}


Graphics::Graphics(const Graphics& other)
{
}


Graphics::~Graphics()
{
}


bool Graphics::Initialize(D3D * d3d)
{
	bool result;

	// Create the texture shader object.
	m_TextureShader = new TextureShader;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_TextureShader->Initialize(d3d->GetDevice());
	if (!result)
	{
		return false;
	}

	return true;
}


void Graphics::Shutdown()
{
	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}
	return;
}


bool Graphics::Frame(ObjectManager * objManager, D3D* d3d)
{
	bool result;


	// Render the graphics scene.
	result = Render(objManager, d3d);
	if (!result)
	{
		return false;
	}

	return true;
}


bool Graphics::Render(ObjectManager * objManager, D3D* d3d)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	d3d->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	objManager->getCamera()->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	d3d->GetWorldMatrix(worldMatrix);
	objManager->getCamera()->GetViewMatrix(viewMatrix);
	d3d->GetProjectionMatrix(projectionMatrix);

	// Drawing ordering reversed for rendering, enables to have the player on the foreground
	for (int i = objManager->getModels().size() - 1; i >= 0; i--) {
		objManager->getModels().at(i)->Render(d3d->GetDeviceContext());
		
		// get model matrix
		XMMATRIX modelMatrix = objManager->getModels().at(i)->getModelMatrix(worldMatrix, viewMatrix, projectionMatrix);

		// Render the models using the texture shader.
		result = m_TextureShader->Render(d3d->GetDeviceContext(), modelMatrix, viewMatrix, projectionMatrix, objManager->getModels().at(i)->GetTexture());
		if (!result)
		{
			return false;
		}
	}

	// Present the rendered scene to the screen.
	d3d->EndScene();

	return true;
}