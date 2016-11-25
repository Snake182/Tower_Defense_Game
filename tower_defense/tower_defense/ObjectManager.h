#pragma once

#include "StructsAndNamespaces.h"
#include "camera.h"
#include "d3d.h"
#include "Level.h"
#include "Player.h"

class ObjectManager
{
public:
	bool initialize(D3D *, int, int, int);
	void Shutdown();
	ObjectManager();
	~ObjectManager();

	Camera* getCamera() { return this->m_Camera; }
	vector<Model *> getModels() { return this->models; }
	Player * getPlayer() { return this->player; }

	void setModels(vector<Model *> models) { this->models = models; }
	void addModel(Model * model) { this->models.push_back(model); }

private:
	Camera* m_Camera;
	vector<Model *> models;
	Level * level;
	Player * player;
};