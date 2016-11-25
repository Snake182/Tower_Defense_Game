#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
	level = 0;
	player = 0;
	m_Camera = 0;
}


ObjectManager::~ObjectManager()
{
}

bool ObjectManager::initialize(D3D * d3d, int level_number, int screenWidth, int screenHeight)
{
	bool result;

	// Create the camera object.
	m_Camera = new Camera;
	if (!m_Camera)
	{
		return false;
	}

	//create and initialize the level object
	level = new Level();
	if (!level) return false;
	if(!level->Initialize(level_number)) return false;
	this->setModels(level->getModels());

	//create and initialize the player object
	player = new Player();
	if (!player) return false;
	player->Initialize();
	this->addModel(player->getModel()); 

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	// Create and initialize the model objects.
	for (int i = 0; i < this->models.size(); i++) {

		result = this->models.at(i)->Initialize(d3d->GetDevice(), d3d->GetDeviceContext(), this->models.at(i)->getFilename(), screenWidth, screenHeight);
		if (!result)
		{
			MessageBox(NULL, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
	}

	return true;
}

void ObjectManager::Shutdown()
{
	// Release the model objects.
	for (int i = 0; i < this->models.size(); i++) {
		if (this->models.at(i))
		{
			this->models.at(i)->Shutdown();
			delete this->models.at(i);
			this->models.at(i) = 0;
		}
	}

	// Release the player.
	if (player)
	{
		delete player;
		player = 0;
	}

	// Release the level.
	if (level)
	{
		delete level;
		level = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	return;
}