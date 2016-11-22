#pragma once
#include "pugixml.hpp"
#include "model.h"

class Level
{
public:
	Level();
	~Level();
	bool Initialize(int);
	bool createDecor(int);
	vector<Model*> createModels(vector<tuple<float, float, float, char*>> modelsPositions);
	vector<Model *> getModels() { return this->models; }
private:
	vector<Model *> models;
};

