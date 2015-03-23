#include "Assets.hpp"
using namespace std;

Assets::Assets(string assetPath){
	this->assetPath = assetPath;
}
void Assets::LoadEntities(){

	auto entitiesPath = this->assetPath + "entities.json";
	std::ifstream entitiesFile(entitiesPath);
	if (entitiesFile.fail()) {
		cout << "Opening entity file '" << entitiesPath << "' failed " << errno << " " << endl;
		exit(1);
	}
	Json::Value root; 
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(entitiesFile, root);
	if (!parsingSuccessful)
	{
		cout << "Failed to parse configuration\n"	<< reader.getFormattedErrorMessages();
		exit(1);
	}
	this->entities = new vector<Entity>(root.size());

	for (int index = 0; index < root.size(); ++index) {
		auto v = root[index];
		int id = v.get("entityID", -1).asInt();
		std::string name = v.get("name", "").asString();
		bool render = v.get("render", false).asBool();
		bool collide = v.get("collide", false).asBool();

		Entity & cur  = this->entities->at(index);
		cur.entityID = id;
		cur.name = name;
		cur.render = render;
		cur.collide = collide;
		if (id == -1 || name.length() == 0){
			cout << "Failed to parse entity file. Entity had either no id or name" << endl;
			exit(1);
		}
	}

}