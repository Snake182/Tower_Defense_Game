#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

bool Player::Initialize()
{
	string level_file = "../Engine/player_data/init_player.xml";

	//parse XML
	pugi::xml_document doc;
	if (!doc.load_file(level_file.c_str())) {
		return false;
	}

	float pozHorizontal = doc.child("player").attribute("posHorizontal").as_float();
	float pozVertical = doc.child("player").attribute("posVertical").as_float();
	float size = doc.child("player").attribute("size").as_float();
	float speed_ratio = doc.child("player").attribute("speed_ratio").as_float();
	string textureFile = doc.child("player").attribute("textureFile").as_string();

	char * c_textureFile = new char[textureFile.size() + 1];
	std::copy(textureFile.begin(), textureFile.end(), c_textureFile);
	c_textureFile[textureFile.size()] = '\0';

	this->model = new Model(pozHorizontal, pozVertical, size, speed_ratio, c_textureFile);
}
