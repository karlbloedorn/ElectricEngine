#include "Grid.hpp"

Grid::Grid(){}

bool Grid::Load(){

	//glGenTextures(1, &texture);
	//Texture::SetupTexture(texture, basePath + textureFile, true);

	int chunkSize = 200;
	numTriangles = chunkSize * chunkSize * 2;

	triangles = new Triangle2D[numTriangles];
	int index = 0;

	for (int x = 0; x < chunkSize; x++){
		for (int z = 0; z < chunkSize; z++){
			triangles[index].a.position.x = x;
			triangles[index].a.position.z = z;
			triangles[index].b.position.x = x + 1;
			triangles[index].b.position.z = z;
			triangles[index].c.position.x = x + 1;
			triangles[index].c.position.z = z + 1;
			index++;
		}
	}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, numTriangles * sizeof(Triangle2D), triangles, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] triangles;
	return true;
}


void Grid::RenderInstances(Shader * shader)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (const GLvoid *)offsetof(Vertex2D, position));

	glUniform1i(shader->texture0, GL_TEXTURE0);	
	
	/*
	for (int index : renderList){
		// Changed to instanced rendering sometime.
		StaticProp * cur = (*staticPropMap)[index];
		glUniformMatrix4fv(shader->modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::translate(cur->position)));
		glDrawArrays(GL_TRIANGLES, 0, this->numTriangles * 3);
	}*/

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}