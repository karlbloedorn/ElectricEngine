#include "Terrain.h"
#define heightArray(WIDTH, r, c) (heights[(r)*WIDTH + (c)])
#define normalArray(WIDTH, r, c) (normals[(r)*WIDTH + (c)])

Terrain::Terrain(noise::module::Perlin * perlin)
{
	glGenTextures(1, &texture);
	Textures::SetupTexture(texture, "assets/textures/terrain/130713 089x2 scb01.png", true);
	this->shader = new Shader();
	this->shader->SetupShader("assets/shaders/terrain.vert", "assets/shaders/terrain.frag",
		list < string > {
		"in_Position",
			"in_TextureCoord",
			"in_Normal"
	},
	list < string > {
			"projectionMatrix",
				"modelMatrix"
		});


	// load initial chunks for testing.
		
	auto nearbyChunkIndices = this->Near(glm::vec3(0, 0, 0),5);
	for (auto chunkIndex : nearbyChunkIndices){
		Chunk * chunk = new Chunk(chunkIndex.x, chunkIndex.z);
		chunk->Generate(perlin);
		chunk->LoadVBO();
		loadedChunks.push_back(chunk);
	}
}

std::list< glm::ivec3 > Terrain::Near(glm::vec3 position, int distance){

	int x = position.x / Chunk::ChunkSize;
	int z = position.y / Chunk::ChunkSize;

	auto list = std::list<glm::ivec3>();
	for (int offset_x = -distance; offset_x < distance; offset_x++)
	{
		for (int offset_z = -distance; offset_z < distance; offset_z++)
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
	glBindTexture(GL_TEXTURE_2D, texture);
	this->shader->EnableShader();
	glUniformMatrix4fv(this->shader->modelMatrix, 1, GL_FALSE, modelview);
	glUniformMatrix4fv(this->shader->projectionMatrixLocation, 1, GL_FALSE, projection);


	// render loaded chunks here.
	for (auto chunk : loadedChunks){
		chunk->Render();
	}


	this->shader->DisableShader();
	glBindTexture(GL_TEXTURE_2D, 0);
}
