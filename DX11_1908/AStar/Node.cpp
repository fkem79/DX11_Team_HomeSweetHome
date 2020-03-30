#include "Framework.h"

Node::Node(Vector3 pos, int index, Float2 interval)
	: pos(pos), index(index), via(-1), f(0), g(0), h(0), state(NONE), interval(interval)
{
	sphere = new SphereCollider();
	sphere->position = pos;
}

Node::~Node()
{
	delete sphere;
}

void Node::Update()
{
	switch (state)
	{
	case Node::NONE:
		sphere->SetColor(Float4(1, 1, 1, 1));
		break;
	case Node::OPEN:
		sphere->SetColor(Float4(0, 0, 1, 1));
		break;
	case Node::CLOSED:
		sphere->SetColor(Float4(0, 0, 0, 1));
		break;
	case Node::USING:
		sphere->SetColor(Float4(0, 1, 0, 1));
		break;
	case Node::OBSTACLE:
		sphere->SetColor(Float4(1, 0, 0, 1));
		break;
	default:
		break;
	}
}

void Node::Render()
{
	sphere->Render();
}

void Node::AddEdge(Node* node)
{
	EdgeInfo* edge = new EdgeInfo();
	edge->index = node->index;
	edge->edgeCost = Distance(pos, node->pos);

	edges.push_back(edge);
}
