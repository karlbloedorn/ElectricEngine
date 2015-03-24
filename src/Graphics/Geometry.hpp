#pragma once
struct Position2D{
	float x;
	float z;
};
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

struct Triangle2D{
	Vertex a;
	Vertex b;
	Vertex c;
};
struct Vertex2D{
	Position position;
};