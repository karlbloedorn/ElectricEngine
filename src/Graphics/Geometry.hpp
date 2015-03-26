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
struct Vertex2D{
	Position2D position;
};
struct Triangle2D{
	Vertex2D a;
	Vertex2D b;
	Vertex2D c;
};
