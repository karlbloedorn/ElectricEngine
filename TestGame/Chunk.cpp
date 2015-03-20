#include "Chunk.h"
#define heightArray(r, c) (heights[(r)*heightLoopMax + (c)])
#define heightArrayShiftedTwo(r, c) (heights[(r+2)*heightLoopMax + (c+2)])
#define heightArrayShiftedOne(r, c) (heights[(r+1)*heightLoopMax + (c+1)])
#define normalArray(r, c) (normals[(r)*normalLoopMax + (c)])
#define normalArrayShiftedOne(r, c) (normals[(r+1)*normalLoopMax + (c+1)])

const int Chunk::ChunkSize = 50;

/*
4x4 chunk

o o o o o o o o
o y y y y y y o
o y x x x x y o
o y x x x x y o
o y x x x x y o
o y x x x x y o
o y y y y y y o
o o o o o o o o

height needs to loop chunksize +2
- when reading values, need to add 2 to both x and z

normal calc needs to loop chunksize + 1
- when reading values, need to add 1 to both x and z

add triangles needs to loop chunksize

o = height calc
xy = normal calc
x = add triangles

*/

Chunk::Chunk(int x, int z)
{
	this->x = x;
	this->z = z;
}

Chunk::~Chunk()
{
}


void Chunk::Generate(noise::module::Perlin * noiseGenerator)
{

	const int heightLoopMax = Chunk::ChunkSize + 4;
	const int normalLoopMax = Chunk::ChunkSize + 2;
	
	float * heights = new float[heightLoopMax * heightLoopMax];
	for (int x = 0; x < heightLoopMax; x++){
		for (int z = 0; z < heightLoopMax; z++){
			auto global_x = x + this->x * Chunk::ChunkSize;
			auto global_z = z + this->z * Chunk::ChunkSize;
			auto height = 55 + noiseGenerator->GetValue(global_x / 505.5, 0, (global_z / 505.5)) * 15;
			heightArray(x,z) = height;
		}
	}

	glm::vec3 * normals = new glm::vec3[normalLoopMax*normalLoopMax];

	//http://www.lighthouse3d.com/opengl/terrain/tnormals.gif see this for logic
	for (int x = 0; x < normalLoopMax; x++){
		for (int z = 0; z < normalLoopMax; z++){
			//given x and z, get the 4 normals

			float heightMain = heightArrayShiftedOne(x, z);

			auto v1 = glm::vec3(-1, heightMain - heightArrayShiftedOne(x - 1, z), 0);
			auto v2 = glm::vec3(0, heightMain - heightArrayShiftedOne(x, z - 1), -1);
			auto v3 = glm::vec3(1, heightMain - heightArrayShiftedOne(x + 1, z), 0);
			auto v4 = glm::vec3(0, heightMain - heightArrayShiftedOne(x, z + 1), 1);

			auto v12 = glm::cross(v2, v1);
			auto v23 = glm::cross(v3, v2);
			auto v34 = glm::cross(v4, v3);
			auto v41 = glm::cross(v1, v4);

			// combine the 4 normals and get the new normal
			normalArray(x, z) = (glm::normalize(v12 + v23 + v34 + v41));
		}
	}

	numTriangles = Chunk::ChunkSize* Chunk::ChunkSize * 2;
	triangles = new Triangle[numTriangles];

	int index = 0;
	for (int x = 0; x < Chunk::ChunkSize; x++){
		for (int z = 0; z < Chunk::ChunkSize; z++){
			auto global_x = x + this->x * Chunk::ChunkSize;
			auto global_z = z + this->z * Chunk::ChunkSize;

			triangles[index].a.position.x = global_x;
			triangles[index].a.position.y = heightArrayShiftedTwo(x, z);
			triangles[index].a.position.z = global_z;

			triangles[index].b.position.x = global_x + 1;
			triangles[index].b.position.y = heightArrayShiftedTwo(x + 1, z);
			triangles[index].b.position.z = global_z;

			triangles[index].c.position.x = global_x + 1;
			triangles[index].c.position.y = heightArrayShiftedTwo(x + 1, z + 1);
			triangles[index].c.position.z = global_z + 1;
			
			glm::vec3  aNormal = normalArrayShiftedOne(x, z);
			glm::vec3  bNormal = normalArrayShiftedOne(x + 1, z);
			glm::vec3  cNormal = normalArrayShiftedOne(x + 1, z + 1);

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
			triangles[index].a.position.x = global_x;
			triangles[index].a.position.y = heightArrayShiftedTwo(x, z);
			triangles[index].a.position.z = global_z;

			triangles[index].b.position.x = global_x + 1;
			triangles[index].b.position.y = heightArrayShiftedTwo(x + 1, z + 1);
			triangles[index].b.position.z = global_z + 1;

			triangles[index].c.position.x = global_x;
			triangles[index].c.position.y = heightArrayShiftedTwo(x, z + 1);
			triangles[index].c.position.z = global_z + 1;

			aNormal = normalArrayShiftedOne(x, z);
			bNormal = normalArrayShiftedOne(x + 1, z + 1);
			cNormal = normalArrayShiftedOne(x, z + 1);

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
	float factor = 20.0;
	for (int i = 0; i < numTriangles; i++){
		triangles[i].a.textureCoord.x = triangles[i].a.position.x / factor;
		triangles[i].a.textureCoord.y = triangles[i].a.position.z / factor;
		triangles[i].b.textureCoord.x = triangles[i].b.position.x / factor;
		triangles[i].b.textureCoord.y = triangles[i].b.position.z / factor;
		triangles[i].c.textureCoord.x = triangles[i].c.position.x / factor;
		triangles[i].c.textureCoord.y = triangles[i].c.position.z / factor;
	}
	delete[] heights;
}
void Chunk::LoadVBO(){
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, numTriangles * sizeof(Triangle), triangles, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] triangles;
}
void Chunk::UnloadVBO(){
	glDeleteBuffers(1, &vbo);
}
void Chunk::Render(){
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)offsetof(Vertex, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)offsetof(Vertex, textureCoord));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)offsetof(Vertex, normal));
	glDrawArrays(GL_TRIANGLES, 0, numTriangles * 3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
