#include "Level.h"

Level::Level()
{
}


Level::~Level()
{
}

bool Level::Initialize(int level_number)
{
	return createDecor(level_number);
}


bool Level::createDecor(int level_number) {

	string level_file = "../Engine/level_data/level_" + to_string(level_number) + ".xml";
	// models positions (position horizontal, position vertical, size, file name)
	vector<tuple<float, float, float, int, char*>> modelsPositions;

	//parse XML
	pugi::xml_document doc;
	if (!doc.load_file(level_file.c_str())) {
		return false;
	}

	pugi::xml_node xml_models = doc.child("models");

	// for each model in models 
	for (pugi::xml_node xml_model = xml_models.first_child(); xml_model; xml_model = xml_model.next_sibling())
	{
		float pozHorizontal = xml_model.attribute("posHorizontal").as_float();
		float pozVertical   = xml_model.attribute("posVertical").as_float();
		float size			= xml_model.attribute("size").as_float();
		int isFullScreen    = xml_model.attribute("isFullScreen").as_int();
		string textureFile  = xml_model.attribute("textureFile").as_string();

		char * c_textureFile = new char[textureFile.size() + 1];
		std::copy(textureFile.begin(), textureFile.end(), c_textureFile);
		c_textureFile[textureFile.size()] = '\0';

		modelsPositions.push_back ( make_tuple ( pozHorizontal, pozVertical, size, isFullScreen, c_textureFile) );
	}

	// models
	this->models = this->createModels(modelsPositions);
}

vector<Model*> Level::createModels(vector<tuple<float, float, float, int, char*>> modelsPositions)
{
	vector<Model*> models;
	for (int i = 0; i < modelsPositions.size(); i++) {
		models.push_back(new Model(get<0>(modelsPositions.at(i)), get<1>(modelsPositions.at(i)), get<2>(modelsPositions.at(i)), get<3>(modelsPositions.at(i)), get<4>(modelsPositions.at(i))));
	}
	return models;
}