#pragma once

class Heap
{
private:
	vector<Node*> heap;

public:
	Heap();
	~Heap();

	void Insert(Node* node);
	void UpdateUpper(int index);

	Node* DeleteRoot();
	void UpdateLower(int index);

	void Swap(int n1, int n2);
	void Clear();
};