#pragma once

class AStar
{
private:
	UINT width, height;

	vector<Node*> nodes;	
	Heap* heap;

public:
	AStar(UINT width = 20, UINT height = 20);
	~AStar();

	void Setup(Terrain* terrain);
	void Update();
	void Render();
};