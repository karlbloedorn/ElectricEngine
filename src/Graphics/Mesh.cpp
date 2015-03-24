#include "Mesh.hpp"

Mesh::Mesh(){}

bool Mesh::LoadFromObj(string basePath, string filePath, string forceTexture){

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err = tinyobj::LoadObj(shapes, materials, (basePath + filePath).c_str(), basePath.c_str());
	if (err.length() > 0){
		cout << "Could not load mesh from object file. Path: " << filePath << endl;
		exit(1);
	}
	std::cout << "# of shapes    : " << shapes.size() << std::endl;
	std::cout << "# of materials : " << materials.size() << std::endl;

	string textureFile = "";

	if (forceTexture.length() == 0){
		textureFile = materials[materials.size()-1].diffuse_texname;

		for (size_t i = 0; i < materials.size(); i++) {

			/*
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
			*/
		}

	}
	else {
		textureFile = forceTexture;
	}

	glGenTextures(1, &texture);
	Texture::SetupTexture(texture, basePath + textureFile, true);

	tinyobj::shape_t shape;
	if (shapes.size() > 1){
		shape = shapes[1];
	}
	else {
		shape = shapes[0];

	}

	numTriangles = shape.mesh.indices.size() / 3;
	triangles = new Triangle[numTriangles];
	bool hasNormals = shape.mesh.normals.size() == shape.mesh.positions.size();
	bool hasTextureCoords = (shape.mesh.texcoords.size() / 2) == (shape.mesh.positions.size() / 3);

	for (unsigned int i = 0; i < shape.mesh.indices.size() / 3; i++) {
		int vert1 = shape.mesh.indices[3 * i + 0];
		int vert2 = shape.mesh.indices[3 * i + 1];
		int vert3 = shape.mesh.indices[3 * i + 2];

		triangles[i].a.position.x = shape.mesh.positions[3 * vert1 + 0];
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

			auto normal = glm::cross(b - a, c - a);
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
		if (hasTextureCoords){
			triangles[i].a.textureCoord.x = shape.mesh.texcoords[2 * vert1 + 0];
			triangles[i].a.textureCoord.y = 1 - shape.mesh.texcoords[2 * vert1 + 1];
			triangles[i].b.textureCoord.x = shape.mesh.texcoords[2 * vert2 + 0];
			triangles[i].b.textureCoord.y = 1 - shape.mesh.texcoords[2 * vert2 + 1];
			triangles[i].c.textureCoord.x = shape.mesh.texcoords[2 * vert3 + 0];
			triangles[i].c.textureCoord.y = 1-shape.mesh.texcoords[2 * vert3 + 1];
		}

	}
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, numTriangles * sizeof(Triangle), triangles, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] triangles;
	return true;
}


void Mesh::RenderInstances(Shader * shader, vector<int> renderList, map<int, StaticProp *> * staticPropMap)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)offsetof(Vertex, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)offsetof(Vertex, textureCoord));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)offsetof(Vertex, normal));

	glUniform1i(shader->texture0, GL_TEXTURE0);	
	
	for (int index : renderList){
		// Changed to instanced rendering sometime.
		StaticProp * cur = (*staticPropMap)[index];
		glUniformMatrix4fv(shader->modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::translate(cur->position)));
		glDrawArrays(GL_TRIANGLES, 0, this->numTriangles * 3);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}