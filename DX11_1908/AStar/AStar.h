#pragma once

class AStar
{
private:
	UINT width, height;

	vector<Node*> nodes;	
	Heap* heap;

	Float2 interval;

	vector<BoxCollider*> obstacles;
public:
	AStar(UINT width = 20, UINT height = 20);
	~AStar();

	void Setup(Terrain* terrain);
	void Update();
	void Render();

	vector<Vector3> FindPath(int start, int end);
	int FindCloseNode(Vector3 pos);
	void MakeDirectPath(IN Vector3 start, IN Vector3 end, OUT vector<Vector3>& path);

	bool IsCollisionObstacle(Ray ray, float destDistance);

	void Reset();

private:
	float GetDistance(int curIndex, int end);

	void Extend(int center, int end);
	int GetMinNode();
};