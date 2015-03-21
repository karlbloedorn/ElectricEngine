#include "Terrain.h"
#define heightArray(WIDTH, r, c) (heights[(r)*WIDTH + (c)])
#define normalArray(WIDTH, r, c) (normals[(r)*WIDTH + (c)])


Terrain::Terrain()
{
	glGenTextures(5, textures);
	Textures::SetupTexture(textures[0], "assets/textures/terrain/map.png", true);
	Textures::SetupTexture(textures[1], "assets/textures/terrain/grass_green_d.jpg", true);
	Textures::SetupTexture(textures[2], "assets/textures/terrain/paving 2.png", true);
	Textures::SetupTexture(textures[3], "assets/textures/terrain/ground_mud_d.jpg", true);
	Textures::SetupTexture(textures[4], "assets/textures/terrain/130713 089x2 scb01.png", true);

	this->shader = new Shader();
	this->shader->SetupShader("assets/shaders/terrain.vert", "assets/shaders/terrain.frag",
		list < string > {
		"in_Position",
			"in_TextureCoord",
			"in_Normal"
	},
	list < string > {
			"projectionMatrix",
				"modelMatrix",
				"texture0",
				"texture1",
				"texture2",
				"texture3",
				"texture4"
		});

	// load initial chunks for testing.
		
	auto nearbyChunkIndices = this->Near(glm::vec3(0, 0, 0),9);
	for (auto chunkIndex : nearbyChunkIndices){
		Chunk * chunk = new Chunk(chunkIndex.x, chunkIndex.z);
		chunk->Generate();
		chunk->LoadVBO();
		loadedChunks.push_back(chunk);
	}
}

std::list< glm::ivec3 > Terrain::Near(glm::vec3 position, int distance){

	int x = position.x / Chunk::ChunkSize;
	int z = position.y / Chunk::ChunkSize;

	auto list = std::list<glm::ivec3>();
	for (int offset_x = 0; offset_x < distance; offset_x++)
	{
		for (int offset_z = 0; offset_z < distance; offset_z++)
		{
			double dist = sqrt(pow(offset_x, 2) + pow(offset_z, 2));
			if (dist < distance)
			{
					list.push_back(glm::ivec3(offset_x + x, 0, z + offset_z));			
			}
		}
	}
	return list;
}

void Terrain::Render(float * modelview, float * projection)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
	}
	this->shader->EnableShader();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->textures[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->textures[1]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->textures[2]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, this->textures[3]);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, this->textures[4]);

	glUniformMatrix4fv(this->shader->modelMatrix, 1, GL_FALSE, modelview);
	glUniformMatrix4fv(this->shader->projectionMatrixLocation, 1, GL_FALSE, projection);

	glUniform1i(this->shader->texture0, 0);
	glUniform1i(this->shader->texture1, 1);
	glUniform1i(this->shader->texture2, 2);
	glUniform1i(this->shader->texture3, 3);
	glUniform1i(this->shader->texture4, 4);

	// render loaded chunks here.
	for (auto chunk : loadedChunks){
		chunk->Render();
	}

	this->shader->DisableShader();
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
}
