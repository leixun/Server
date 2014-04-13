#pragma once
class Octree
{
public:
	Octree();
	~Octree();
private:
	Octree* child[8];
};

