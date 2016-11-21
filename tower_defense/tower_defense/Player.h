#pragma once
#include "pugixml.hpp"
#include "model.h"

class Player
{
public:
	Player();
	~Player();
	bool Initialize();
	Model * getModel() { return this->model; }
private:
	Model * model;
};

