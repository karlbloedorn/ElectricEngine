#pragma once
#include <json/json.h>
#include <iostream>
#include <fstream>
#include "Entity.hpp"
#include <functional>

using namespace std;

class Assets{
private:
	string assetPath;
public:
	vector<Entity> * entities ;
	void LoadEntities();
	Assets(string assetPath);
};

