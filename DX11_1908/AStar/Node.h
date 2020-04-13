#pragma once

class Node
{
public:
	friend class AStar;
	friend class Heap;

	struct EdgeInfo
	{
		int index;
		float edgeCost;
	};

	enum State
	{
		NONE,
		OPEN,
		CLOSED,
		USING,
		OBSTACLE
	};

private:
	SphereCollider* sphere;

	Vector3 pos;
	int index;
	int via;

	float f, g, h;

	State state;

	Float2 interval;

	vector<EdgeInfo*> edges;

	Node(Vector3 pos, int index, Float2 interval);
	~Node();

	void Update();
	void Render();

	void AddEdge(Node* node);

	BoxCollider* SetObstacle();
};