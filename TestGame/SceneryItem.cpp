#include "SceneryItem.h"
#include "deps/static/tiny_obj_loader.h"

using namespace std;

SceneryItem::SceneryItem()
{
	
	this->shader = new Shader();

	this->shader->SetupShader("assets/shaders/scenery.vert", "assets/shaders/scenery.frag",
		list < string > {
		"in_Position",
		"in_TextureCoord",
		"in_Normal"
	},
	list < string > {
			"projectionMatrix",
				"modelMatrix"
		});
}

SceneryItem::~SceneryItem()
{
}

bool SceneryItem::LoadFromObj(string basePath, string filePath, string forceTexture){

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err = tinyobj::LoadObj(shapes, materials, (basePath + filePath).c_str(), basePath.c_str());

	std::cout << "# of shapes    : " << shapes.size() << std::endl;
	std::cout << "# of materials : " << materials.size() << std::endl;

	string textureFile = "";

	if (forceTexture.length() == 0){
		textureFile = materials[0].diffuse_texname;
		for (size_t i = 0; i < materials.size(); i++) {
			printf("material[%ld].name = %s\n", i, materials[i].name.c_str());
			printf("  material.Ka = (%f, %f ,%f)\n", materials[i].ambient[0], materials[i].ambient[1], materials[i].ambient[2]);
			printf("  material.Kd = (%f, %f ,%f)\n", materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
			printf("  material.Ks = (%f, %f ,%f)\n", materials[i].specular[0], materials[i].specular[1], materials[i].specular[2]);
			printf("  material.Tr = (%f, %f ,%f)\n", materials[i].transmittance[0], materials[i].transmittance[1], materials[i].transmittance[2]);
			printf("  material.Ke = (%f, %f ,%f)\n", materials[i].emission[0], materials[i].emission[1], materials[i].emission[2]);
			printf("  material.Ns = %f\n", materials[i].shininess);
			printf("  material.Ni = %f\n", materials[i].ior);
			printf("  material.dissolve = %f\n", materials[i].dissolve);
			printf("  material.illum = %d\n", materials[i].illum);
			printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
			printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
			printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
			printf("  material.map_Ns = %s\n", materials[i].normal_texname.c_str());
			std::map<std::string, std::string>::const_iterator it(materials[i].unknown_parameter.begin());
			std::map<std::string, std::string>::const_iterator itEnd(materials[i].unknown_parameter.end());
			for (; it != itEnd; it++) {
				printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
			}
			printf("\n");
		}

	}
	else {
		textureFile = forceTexture;
	}


	glGenTextures(1, &texture);
	Textures::SetupTexture(texture, basePath + textureFile, true);



	auto shape = shapes[0];
	numTriangles = shape.mesh.indices.size() / 3;
	triangles = new Triangle[numTriangles];
	bool hasNormals = shape.mesh.normals.size() == shape.mesh.positions.size();

	for (int i = 0; i < shape.mesh.indices.size() / 3; i++) {
		int vert1 = shape.mesh.indices[3 * i + 0];
		int vert2 = shape.mesh.indices[3 * i + 1];
		int vert3 = shape.mesh.indices[3 * i + 2];

		triangles[i].a.position.x = shape.mesh.positions[3* vert1 + 0];
		triangles[i].a.position.y = shape.mesh.positions[3 * vert1 + 1];
		triangles[i].a.position.z = shape.mesh.positions[3 * vert1 + 2];
		triangles[i].b.position.x = shape.mesh.positions[3 * vert2 + 0];
		triangles[i].b.position.y = shape.mesh.positions[3 * vert2 + 1];
		triangles[i].b.position.z = shape.mesh.positions[3 * vert2 + 2];
		triangles[i].c.position.x = shape.mesh.positions[3 * vert3 + 0];
		triangles[i].c.position.y = shape.mesh.positions[3 * vert3 + 1];
		triangles[i].c.position.z = shape.mesh.positions[3 * vert3 + 2];
		if (hasNormals){
			triangles[i].a.normal.x = shape.mesh.normals[3 * vert1 + 0];
			triangles[i].a.normal.y = shape.mesh.normals[3 * vert1 + 1];
			triangles[i].a.normal.z = shape.mesh.normals[3 * vert1 + 2];
			triangles[i].b.normal.x = shape.mesh.normals[3 * vert2 + 0];
			triangles[i].b.normal.y = shape.mesh.normals[3 * vert2 + 1];
			triangles[i].b.normal.z = shape.mesh.normals[3 * vert2 + 2];
			triangles[i].c.normal.x = shape.mesh.normals[3 * vert3 + 0];
			triangles[i].c.normal.y = shape.mesh.normals[3 * vert3 + 1];
			triangles[i].c.normal.z = shape.mesh.normals[3 * vert3 + 2];
		}
		else {
			auto a = glm::vec3(triangles[i].a.position.x, triangles[i].a.position.y, triangles[i].a.position.z);
			auto b = glm::vec3(triangles[i].b.position.x, triangles[i].b.position.y, triangles[i].b.position.z);
			auto c = glm::vec3(triangles[i].c.position.x, triangles[i].c.position.y, triangles[i].c.position.z);

			auto normal = glm::cross(b-a, c-a);
			triangles[i].a.normal.x = normal.x;
			triangles[i].a.normal.y = normal.y;
			triangles[i].a.normal.z = normal.z;
			triangles[i].b.normal.x = normal.x;
			triangles[i].b.normal.y = normal.y;
			triangles[i].b.normal.z = normal.z;
			triangles[i].c.normal.x = normal.x;
			triangles[i].c.normal.y = normal.y;
			triangles[i].c.normal.z = normal.z;
		}
	
		triangles[i].a.textureCoord.x = shape.mesh.texcoords[2 * vert1 + 0];
		triangles[i].a.textureCoord.y = shape.mesh.texcoords[2 * vert1 + 1];
		triangles[i].b.textureCoord.x = shape.mesh.texcoords[2 * vert2 + 0];
		triangles[i].b.textureCoord.y = shape.mesh.texcoords[2 * vert2 + 1];
		triangles[i].c.textureCoord.x = shape.mesh.texcoords[2 * vert3 + 0];
		triangles[i].c.textureCoord.y = shape.mesh.texcoords[2 * vert3 + 1];
	}
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, numTriangles * sizeof(Triangle), triangles, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] triangles;
	return true;
}


void SceneryItem::Render(float * modelview, float * projection)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	this->shader->EnableShader();
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glUniformMatrix4fv(this->shader->modelMatrix, 1, GL_FALSE, modelview);
	glUniformMatrix4fv(this->shader->projectionMatrixLocation, 1, GL_FALSE, projection);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)offsetof(Vertex, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)offsetof(Vertex, textureCoord));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)offsetof(Vertex, normal));
	glDrawArrays(GL_TRIANGLES, 0, numTriangles * 3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	this->shader->DisableShader();
	glBindTexture(GL_TEXTURE_2D, 0);
}