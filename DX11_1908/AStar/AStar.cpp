#include "Framework.h"

AStar::AStar(UINT width, UINT height)
	: width(width), height(height), interval(0, 0)
{
	heap = new Heap();
}

AStar::~AStar()
{
	delete heap;

	for (Node* node : nodes)
		delete node;

	for (BoxCollider* obstacle : obstacles)
		delete obstacle;
}

void AStar::Setup(Terrain* terrain)
{
	Float2 size = terrain->GetSize();
	
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

	for (UINT i = 0; i < nodes.size(); i++)
	{
		if (i % width != width - 1)
		{
			nodes[i + 0]->AddEdge(nodes[i + 1]);
			nodes[i + 1]->AddEdge(nodes[i + 0]);
		}

		if (i < nodes.size() - width)
		{
			nodes[i + 0]->AddEdge(nodes[i + width]);
			nodes[i + width]->AddEdge(nodes[i + 0]);
		}

		if (i < nodes.size() - width && i % width != width - 1)
		{
			nodes[i + 0]->AddEdge(nodes[i + width + 1]);
			nodes[i + width + 1]->AddEdge(nodes[i + 0]);
		}

		if (i < nodes.size() - width && i % width != 0)
		{
			nodes[i + 0]->AddEdge(nodes[i + width - 1]);
			nodes[i + width - 1]->AddEdge(nodes[i + 0]);
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
				obstacles.push_back(node->SetObstacle());
		}
	}

	for (Node* node : nodes)
		node->Update();
}

void AStar::Render()
{
	for (Node* node : nodes)
		node->Render();

	for (BoxCollider* obstacle : obstacles)
		obstacle->Render();
}

vector<Vector3> AStar::FindPath(int start, int end)
{
	float G = 0.0f;
	float H = GetDistance(start, end);

	nodes[start]->g = G;
	nodes[start]->h = H;
	nodes[start]->f = G + H;
	nodes[start]->via = start;
	nodes[start]->state = Node::OPEN;

	heap->Insert(nodes[start]);

	while (nodes[end]->state != Node::CLOSED)
	{
		int curIndex = GetMinNode();
		Extend(curIndex, end);
		nodes[curIndex]->state = Node::CLOSED;
	}

	vector<Vector3> path;

	int curIndex = end;
	while (curIndex != start)
	{
		nodes[curIndex]->state = Node::USING;
		path.push_back(nodes[curIndex]->pos);
		curIndex = nodes[curIndex]->via;
	}

	nodes[curIndex]->state = Node::USING;
	path.push_back(nodes[curIndex]->pos);
	heap->Clear();

	return path;
}

int AStar::FindCloseNode(Vector3 pos)
{
	float minDistance = FLT_MAX;
	int index = -1;

	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i]->state == Node::OBSTACLE)
			continue;

		float distance = Distance(pos, nodes[i]->pos);

		if (distance < minDistance)
		{
			minDistance = distance;
			index = i;
		}
	}

	return index;
}

void AStar::MakeDirectPath(IN Vector3 start, IN Vector3 end, OUT vector<Vector3>& path)
{
	int cutNodeNum = 0;
	Ray ray;
	ray.position = start;

	for (int i = 0; i < path.size(); i++)
	{
		ray.direction = path[i] - ray.position;
		float distance = ray.direction.Length();

		ray.direction.Normalize();

		if (!IsCollisionObstacle(ray, distance))
		{
			cutNodeNum = path.size() - i - 1;
			break;
		}
	}

	for (int i = 0; i < cutNodeNum; i++)
		path.pop_back();

	cutNodeNum = 0;
	ray.position = end;

	for (int i = 0; i < path.size(); i++)
	{
		ray.direction = path[path.size() - i - 1] - ray.position;
		float distance = ray.direction.Length();

		ray.direction.Normalize();

		if (!IsCollisionObstacle(ray, distance))
		{
			cutNodeNum = path.size() - i - 1;
			break;
		}
	}

	for (int i = 0; i < cutNodeNum; i++)
		path.erase(path.begin());
}

bool AStar::IsCollisionObstacle(Ray ray, float destDistance)
{
	for (BoxCollider* obstacle : obstacles)
	{
		float distance = FLT_MAX;

		if (obstacle->IsCollision(ray, &distance))
		{
			if (distance < destDistance)
				return true;
		}
	}

	return false;
}

void AStar::Reset()
{
	for (Node* node : nodes)
	{
		if (node->state != Node::OBSTACLE)
			node->state = Node::NONE;
	}
}

float AStar::GetDistance(int curIndex, int end)
{
	Vector3 temp = nodes[end]->pos - nodes[curIndex]->pos;
	temp.SetX(abs(temp.GetX()));
	temp.SetZ(abs(temp.GetZ()));

	float min = min(temp.GetX(), temp.GetZ());
	float max = max(temp.GetX(), temp.GetZ());

	return (max - min) + sqrt(min*min + min*min);
}

void AStar::Extend(int center, int end)
{
	vector<Node::EdgeInfo*> edges = nodes[center]->edges;

	for (int i = 0; i < edges.size(); i++)
	{
		int index = edges[i]->index;

		if (nodes[index]->state == Node::CLOSED || nodes[index]->state == Node::OBSTACLE)
			continue;

		float G = nodes[center]->g + edges[i]->edgeCost;
		float H = GetDistance(index, end);
		float F = G + H;

		if (nodes[index]->state == Node::OPEN)
		{
			if (F < nodes[index]->f)
			{
				nodes[index]->g = G;
				nodes[index]->f = F;
				nodes[index]->via = center;
			}
		}
		else if (nodes[index]->state == Node::NONE)
		{
			nodes[index]->g = G;
			nodes[index]->h = H;
			nodes[index]->f = F;
			nodes[index]->via = center;
			nodes[index]->state = Node::OPEN;

			heap->Insert(nodes[index]);
		}
	}
}

int AStar::GetMinNode()
{
	return heap->DeleteRoot()->index;
}
