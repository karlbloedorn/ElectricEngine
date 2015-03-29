#include "Grid.hpp"

Grid::Grid(){}

const static int chunkSize = 100;

bool Grid::Load(string assetsPath, World * world){

	glGenTextures(6, textures);

	Texture::SetupTexture(textures[0], assetsPath + "textures/heightmap.png", true, false, &world->heightMap, &world->outHeight, &world->outWidth);
	Texture::SetupTexture(textures[1], assetsPath + "textures/terrain/130713 089x2 scb01.png", true, true, NULL, NULL, NULL);
	Texture::SetupTexture(textures[2], assetsPath + "textures/terrain/paving 2.png", true, true, NULL, NULL, NULL);
	Texture::SetupTexture(textures[3], assetsPath + "textures/terrain/lava2_d.jpg", true, true, NULL, NULL, NULL);
	Texture::SetupTexture(textures[4], assetsPath + "textures/terrain/lava2_d.jpg", true, true, NULL, NULL, NULL);
	Texture::SetupTexture(textures[5], assetsPath + "textures/texturemap.png", true, true, NULL, NULL, NULL);

	numTriangles = chunkSize * chunkSize * 2;

	triangles = new Triangle2D[numTriangles];
	int index = 0;


	for (int i = 0; i < chunkSize; i++){
		for (int j = 0; j < (i+1); j++){
			if (i == j){
			  //	i, j
				triangles[index].a.position.x = i;
				triangles[index].a.position.z = j;
				triangles[index].b.position.x = i + 1;
				triangles[index].b.position.z = j;
				triangles[index].c.position.x = i + 1;
				triangles[index].c.position.z = j + 1;
				index++;

				triangles[index].a.position.x = i + 1;
				triangles[index].a.position.z = j + 1;
				triangles[index].b.position.x = i;
				triangles[index].b.position.z = j + 1;
				triangles[index].c.position.x = i;
				triangles[index].c.position.z = j;
				index++;

			}	else {
				//i, j
				
				triangles[index].a.position.x = i;
				triangles[index].a.position.z = j;
				triangles[index].b.position.x = i + 1;
				triangles[index].b.position.z = j;
				triangles[index].c.position.x = i + 1;
				triangles[index].c.position.z = j + 1;
				index++;

				triangles[index].a.position.x = i + 1;
				triangles[index].a.position.z = j + 1;
				triangles[index].b.position.x = i;
				triangles[index].b.position.z = j + 1;
				triangles[index].c.position.x = i;
				triangles[index].c.position.z = j;
				index++;

				//	j, i
				triangles[index].a.position.x = j;
				triangles[index].a.position.z = i;
				triangles[index].b.position.x = j + 1;
				triangles[index].b.position.z = i;
				triangles[index].c.position.x = j + 1;
				triangles[index].c.position.z = i + 1;
				index++;

				triangles[index].a.position.x = j + 1;
				triangles[index].a.position.z = i + 1;
				triangles[index].b.position.x = j;
				triangles[index].b.position.z = i + 1;
				triangles[index].c.position.x = j;
				triangles[index].c.position.z = i;
				index++;
			}
		}
	}

	/*

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
	}*/

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

	int posx = cameraPosition.x / chunkSize;
	int posz = cameraPosition.z / chunkSize;
	int startx =  - 15;
	int endx =   15;
	int startz =  - 15;
	int endz =   15;

	for (int x = startx; x < endx; x++){
		for (int z = startz; z < endz; z++){

			float scaleUp = 10.0f;
			int resolution = 10;
			//if (abs(x) < 2 && abs(z) < 2){
			//	scaleUp = 1.0;
			//	resolution = 100;
			//} else if (abs(x) < 4 && abs(z) < 4){
				scaleUp = 2.0;
				resolution = 50;
			//}

			float dest_x = (posx + x)*chunkSize;
			float dest_z = (posz + z)*chunkSize;
			if (dest_x >= 0 && dest_z >= 0 && ((dest_x + chunkSize) <= 4096) && ((dest_z + chunkSize) <= 4096)){

				auto translate = glm::translate(glm::vec3(dest_x, 0, dest_z));
				auto scale = glm::scale(glm::vec3(scaleUp, 0.0f, scaleUp));
				glUniformMatrix4fv(shader->modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(translate * scale));
				glDrawArrays(GL_TRIANGLES, 0, resolution*resolution * 2 * 3); //  this->numTriangles * 3);
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