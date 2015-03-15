#include "SceneryItem.h"

using namespace std;

SceneryItem::SceneryItem()
{
	//glGenTextures(1, &texture);
	//Textures::SetupTexture(texture, "assets/meshes/palmtree/normal512.tga", true);


	this->shader = new Shader();

	this->shader->SetupShader("assets/shaders/scenery.vert", "assets/shaders/scenery.frag",
		list < string > {
		"in_Position"
	},
	list < string > {
			"projectionMatrix",
				"modelMatrix"
		});

}

SceneryItem::~SceneryItem()
{
}

bool SceneryItem::LoadFromObj(string filePath){

	ifstream shaderFile(filePath);
	if (shaderFile.fail()) {
		perror(filePath.c_str());
		return false;
	}
	vector<glm::vec3> vertexCoords;
	vector<string> faceLines;

	string line;
	while (getline(shaderFile, line)) {
		if (line.length() > 0){
			char firstChar = line.at(0);
			if (firstChar == '#'){
			} else if (firstChar == 'o'){
				cout << line << endl;
			} else if (firstChar == 'v'){
				auto vertexCoord = glm::vec3(0, 0, 0);
				sscanf_s(line.c_str(), "v %f %f %f", &vertexCoord.x, &vertexCoord.y, &vertexCoord.z);
				vertexCoords.push_back(vertexCoord);
			} else if (firstChar == 'vt'){
				
			} else if (firstChar == 'vn'){
				
			} else if (firstChar == 'f'){
				faceLines.push_back(line);
			}
		}
	}
	cout << "vertices: " << vertexCoords.size() << endl;
	cout << "faces: " << faceLines.size() << endl;

	numTriangles = faceLines.size()*2;
	triangles = new Triangle[numTriangles];

	int index = 0;
	for (int i = 0; i < faceLines.size(); i++){
		string curLine = faceLines[i];
		int a = 0;
		int b = 0;
		int c = 0;
		int d = 0;
		int ignore;

		sscanf_s(curLine.c_str(), "f %i/%*i/%*i %i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c, &d);
		triangles[index].a.position.x = vertexCoords[a].x;
		triangles[index].a.position.y = vertexCoords[a].y;
		triangles[index].a.position.z = vertexCoords[a].z;
		triangles[index].b.position.x = vertexCoords[b].x;
		triangles[index].b.position.y = vertexCoords[b].y;
		triangles[index].b.position.z = vertexCoords[b].z;
		triangles[index].c.position.x = vertexCoords[c].x;
		triangles[index].c.position.y = vertexCoords[c].y;
		triangles[index].c.position.z = vertexCoords[c].z;
		index++;
		triangles[index].a.position.x = vertexCoords[b].x;
		triangles[index].a.position.y = vertexCoords[b].y;
		triangles[index].a.position.z = vertexCoords[b].z;
		triangles[index].b.position.x = vertexCoords[c].x;
		triangles[index].b.position.y = vertexCoords[c].y;
		triangles[index].b.position.z = vertexCoords[c].z;
		triangles[index].c.position.x = vertexCoords[d].x;
		triangles[index].c.position.y = vertexCoords[d].y;
		triangles[index].c.position.z = vertexCoords[d].z;
		index++;
	}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, numTriangles * sizeof(Triangle), triangles, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] triangles;
}


void SceneryItem::Render(float * modelview, float * projection)
{
	//glBindTexture(GL_TEXTURE_2D, texture);
	this->shader->EnableShader();
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glUniformMatrix4fv(this->shader->modelMatrix, 1, GL_FALSE, modelview);
	glUniformMatrix4fv(this->shader->projectionMatrixLocation, 1, GL_FALSE, projection);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)offsetof(Vertex, position));
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)offsetof(Vertex, textureCoord));
//	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)offsetof(Vertex, normal));
	glDrawArrays(GL_TRIANGLES, 0, numTriangles * 3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	this->shader->DisableShader();
	//glBindTexture(GL_TEXTURE_2D, 0);
}