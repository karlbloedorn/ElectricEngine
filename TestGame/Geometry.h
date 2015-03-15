#pragma once
struct Position{
	float x;
	float y;
	float z;
};
struct TextureCoord{
	float x;
	float y;
};
struct Normal{
	float x;
	float y;
	float z;
};
struct Vertex{
	Position position;
	TextureCoord textureCoord;
	Normal normal;
};

struct Triangle{
	Vertex a;
	Vertex b;
	Vertex c;
};