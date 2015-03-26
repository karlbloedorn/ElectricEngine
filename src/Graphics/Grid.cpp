#include "Grid.hpp"

Grid::Grid(){}

const static int chunkSize =20;

bool Grid::Load(string assetsPath){
	glGenTextures(6, textures);
	Texture::SetupTexture(textures[0], assetsPath + "textures/heightmap.png", true);
	Texture::SetupTexture(textures[1], assetsPath + "textures/terrain/130713 089x2 scb01.png", true);
	Texture::SetupTexture(textures[2], assetsPath + "textures/terrain/paving 2.png", true);
	Texture::SetupTexture(textures[3], assetsPath + "textures/terrain/ground_mud_d.jpg", true);
	Texture::SetupTexture(textures[4], assetsPath + "textures/terrain/grass_green_d.jpg", true);
	Texture::SetupTexture(textures[5], assetsPath + "textures/texturemap.png", true);


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

			triangles[index].a.position.x = x + 1;
			triangles[index].a.position.z = z + 1;
			triangles[index].b.position.x = x;
			triangles[index].b.position.z = z+1;
			triangles[index].c.position.x = x;
			triangles[index].c.position.z = z;
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


void Grid::RenderInstances(Shader * shader, glm::vec3 cameraPosition)
{
	GLenum err;

	while ((err = glGetError()) != GL_NO_ERROR) {
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->textures[0]);
	while ((err = glGetError()) != GL_NO_ERROR) {
		cerr << "OpenGL error: " << err << endl;
	}
	glUniform1i(shader->texture0, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->textures[1]);
	glUniform1i(shader->texture1, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->textures[2]);
	glUniform1i(shader->texture2, 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, this->textures[3]);
	glUniform1i(shader->texture3, 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, this->textures[4]);
	glUniform1i(shader->texture4, 4);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, this->textures[5]);
	glUniform1i(shader->texture5, 5);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (const GLvoid *)offsetof(Vertex2D, position));

	int x = cameraPosition.x / chunkSize;
	int z = cameraPosition.z / chunkSize;
	int startx = x - 25;
	int endx = x + 25;
	int startz = z - 25;
	int endz = z + 25;

	for (int x = startx; x < endx; x++){
		for (int z = startz; z < endz; z++){

			float dest_x = x*chunkSize;
			float dest_z = z*chunkSize;
			if (dest_x >= 0 && dest_z >= 0 && ((dest_x + chunkSize) <= 4096) && ((dest_z + chunkSize) <= 4096)){
				glUniformMatrix4fv(shader->modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::translate(glm::vec3(dest_x, 0, dest_z))));
				glDrawArrays(GL_TRIANGLES, 0, this->numTriangles * 3);
			}
		}
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}