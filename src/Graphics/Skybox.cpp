#include "Skybox.hpp"

using namespace std;

Skybox::Skybox()
{
}
bool Skybox::Load(std::string assetsPath){
	glGenTextures(6, textures);
	Texture::SetupTexture(textures[0], assetsPath + "textures/skybox/cloud/negx.jpg", false);
	Texture::SetupTexture(textures[1], assetsPath + "textures/skybox/cloud/posy.jpg", false);
	Texture::SetupTexture(textures[2], assetsPath + "textures/skybox/cloud/negz.jpg", false);
	Texture::SetupTexture(textures[3], assetsPath + "textures/skybox/cloud/posx.jpg", false);
	Texture::SetupTexture(textures[4], assetsPath + "textures/skybox/cloud/negy.jpg", false);
	Texture::SetupTexture(textures[5], assetsPath + "textures/skybox/cloud/posz.jpg", false);

	numTriangles = 12;

	static const float vertexData[5 * 4 * 12] = {
		-800, 800, -800, 0, 0,
		-800, 800, 800, 1, 0,
		-800, -800, 800, 1, 1,
		-800, -800, 800, 1, 1,
		-800, -800, -800, 0, 1,
		-800, 800, -800, 0, 0,

		800, 800, -800, 1, 0,
		800, 800, 800, 1, 1,
		-800, 800, 800, 0, 1,
		-800, 800, 800, 0, 1,
		-800, 800, -800, 0, 0,
		800, 800, -800, 1, 0,

		800, -800, -800, 0, 1,
		800, 800, -800, 0, 0,
		-800, 800, -800, 1, 0,
		-800, 800, -800, 1, 0,
		-800, -800, -800, 1, 1,
		800, -800, -800, 0, 1,

		800, -800, -800, 1, 1,
		800, -800, 800, 0, 1,
		800, 800, 800, 0, 0,
		800, 800, 800, 0,0,
		800, 800, -800, 1,0,
		800, -800, -800, 1, 1,

		-800, -800, -800, 0, 1,
		-800, -800, 800, 0, 0,
		800, -800, 800, 1, 0,
		800, -800, 800, 1, 0,
		800, -800, -800, 1, 1,
		-800, -800, -800, 0, 1,

		800, 800, 800, 1, 0,
		800, -800, 800, 1, 1,
		-800, -800, 800, 0, 1,
		-800, -800, 800, 0, 1,
		-800, 800, 800, 0, 0,
		800, 800, 800, 1, 0
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 5 * 4 * 12 * 4, vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return true;
}



void Skybox::Render(Shader * shader)
{
	glActiveTexture(GL_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*5, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (const GLvoid *)(sizeof(float) * 3));
	glUniform1i(shader->texture0, GL_TEXTURE0);
	//negx 0, negy 1, negz 2, posx 3, posy 4, posz 5
	
	for (int i = 0; i < 6; i++){
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glDrawArrays(GL_TRIANGLES, i*2*3, 2*3);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}