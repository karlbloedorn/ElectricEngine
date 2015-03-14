#include "Terrain.h"
#define heightArray(WIDTH, r, c) (heights[(r)*WIDTH + (c)])
#define normalArray(WIDTH, r, c) (normals[(r)*WIDTH + (c)])

Terrain::Terrain(int xSize, int zSize, noise::module::Perlin * perlin)
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


	this->xSize = xSize;
	this->zSize = zSize;

	float * heights = new float[ (xSize + 1 ) * (zSize+1)];
	for (int x = 0; x < xSize+1; x++){
		for (int z = 0; z < zSize+1; z++){
			auto height = 5 + perlin->GetValue(x /25.5, 0, (z /25.5))*15;
			heights[x*zSize + z] = height;// height;
		}
	}

	glm::vec3 * normals = new glm::vec3[(xSize+1)  * ( zSize +1)];

	//http://www.lighthouse3d.com/opengl/terrain/tnormals.gif see this for logic
	for (int x = 1; x < xSize; x++){
		for (int z = 1; z < zSize; z++){
			//given x and z, get the 4 normals

			float heightMain = heightArray(zSize + 1, x, z);

				auto v1 = glm::vec3(-1, heightMain- heightArray(zSize + 1, x - 1, z), 0);
			auto v2 = glm::vec3(0, heightMain-heightArray(zSize + 1, x, z - 1), -1);
			auto v3 = glm::vec3(1, heightMain- heightArray(zSize + 1, x + 1, z), 0);
			auto v4 = glm::vec3(0, heightMain- heightArray(zSize + 1, x, z + 1), 1);

			auto v12 = glm::cross(v2, v1);
			auto v23 = glm::cross(v3, v2);
			auto v34 = glm::cross(v4, v3);
			auto v41 = glm::cross(v1, v4);

			// combine the 4 normals and get the new normal
			normalArray(zSize+1, x, z) = (glm::normalize(v12 + v23 + v34 + v41));
		}
	}


	numTriangles = xSize * zSize * 2;
	triangles = new Triangle[numTriangles];

	int index = 0;
	for (int x = 0; x < xSize; x++){
		for (int z = 0; z < zSize; z++){

			triangles[index].a.position.x = x;
			triangles[index].a.position.y = heightArray(zSize + 1, x, z);
			triangles[index].a.position.z = z;

			triangles[index].b.position.x = x + 1;
			triangles[index].b.position.y = heightArray(zSize + 1, x + 1, z);
			triangles[index].b.position.z = z;

			triangles[index].c.position.x = x + 1;
			triangles[index].c.position.y = heightArray(zSize + 1, x + 1, z + 1);
			triangles[index].c.position.z = z + 1;

			glm::vec3  aNormal = normalArray(zSize + 1, x, z);
			glm::vec3  bNormal = normalArray(zSize + 1, x + 1, z);
			glm::vec3  cNormal = normalArray(zSize + 1, x+1, z+1);
			triangles[index].a.normal.x = aNormal.x;
			triangles[index].a.normal.y = aNormal.y;
			triangles[index].a.normal.z = aNormal.z;
			triangles[index].b.normal.x = bNormal.x;
			triangles[index].b.normal.y = bNormal.y;
			triangles[index].b.normal.z = bNormal.z;
			triangles[index].c.normal.x = cNormal.x;
			triangles[index].c.normal.y = cNormal.y;
			triangles[index].c.normal.z = cNormal.z;

			index++;
			triangles[index].a.position.x = x;
			triangles[index].a.position.y = heightArray(zSize + 1, x, z);
			triangles[index].a.position.z = z;

			triangles[index].b.position.x = x + 1;
			triangles[index].b.position.y = heightArray(zSize + 1, x + 1, z + 1);
			triangles[index].b.position.z = z + 1;

			triangles[index].c.position.x = x;
			triangles[index].c.position.y = heightArray(zSize + 1, x, z + 1);
			triangles[index].c.position.z = z + 1;

			aNormal = normalArray(zSize + 1, x, z);
			bNormal = normalArray(zSize + 1, x + 1, z+1);
			cNormal = normalArray(zSize + 1, x, z + 1);

			triangles[index].a.normal.x = aNormal.x;
			triangles[index].a.normal.y = aNormal.y;
			triangles[index].a.normal.z = aNormal.z;
			triangles[index].b.normal.x = bNormal.x;
			triangles[index].b.normal.y = bNormal.y;
			triangles[index].b.normal.z = bNormal.z;
			triangles[index].c.normal.x = cNormal.x;
			triangles[index].c.normal.y = cNormal.y;
			triangles[index].c.normal.z = cNormal.z;
			index++;
		}
	}

	float factor =20.0;
	for (int i = 0; i < numTriangles; i++){
		triangles[i].a.textureCoord.x = triangles[i].a.position.x / factor;
		triangles[i].a.textureCoord.y = triangles[i].a.position.z / factor;
		triangles[i].b.textureCoord.x = triangles[i].b.position.x / factor;
		triangles[i].b.textureCoord.y = triangles[i].b.position.z / factor;
		triangles[i].c.textureCoord.x = triangles[i].c.position.x / factor;
		triangles[i].c.textureCoord.y = triangles[i].c.position.z / factor;
	}

	delete[] heights;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, numTriangles * sizeof(Triangle), triangles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)offsetof(Vertex, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)offsetof(Vertex, textureCoord));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)offsetof(Vertex, normal));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] triangles;
}

Terrain::~Terrain()
{
	delete[] triangles;
}

void Terrain::Render(float * modelview, float * projection)
{

	glBindTexture(GL_TEXTURE_2D, texture);

	this->shader->EnableShader();
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glUniformMatrix4fv(this->shader->modelMatrix, 1, GL_FALSE, modelview);
	glUniformMatrix4fv(this->shader->projectionMatrixLocation, 1, GL_FALSE, projection);

	glDrawArrays(GL_TRIANGLES, 0, numTriangles * 3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	this->shader->DisableShader();

	glBindTexture(GL_TEXTURE_2D, 0);

}
