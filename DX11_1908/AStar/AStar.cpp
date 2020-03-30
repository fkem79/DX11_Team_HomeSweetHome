#include "Framework.h"

AStar::AStar(UINT width, UINT height)
	: width(width), height(height)
{
	heap = new Heap();
}

AStar::~AStar()
{
	delete heap;

	for (Node* node : nodes)
		delete node;
}

void AStar::Setup(Terrain* terrain)
{
	Float2 size = terrain->GetSize();

	Float2 interval;
	interval.x = size.x / width;
	interval.y = size.y / height;

	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			Vector3 pos = Vector3(x * interval.x, 0, z * interval.y);
			pos.SetY(terrain->GetHeight(pos));

			int index = z * width + x;
			Node* node = new Node(pos, index, interval);
			nodes.push_back(node);
		}
	}
}

void AStar::Update()
{
	if (KEYDOWN(VK_RBUTTON))
	{
		Ray ray = CAMERA->GetRay();

		for (Node* node : nodes)
		{
			if (node->sphere->IsCollision(ray))
				node->state = Node::OBSTACLE;
		}
	}

	for (Node* node : nodes)
		node->Update();
}

void AStar::Render()
{
	for (Node* node : nodes)
		node->Render();
}
