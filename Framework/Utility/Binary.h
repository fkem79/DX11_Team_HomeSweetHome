#pragma once

class BinaryWriter
{
private:
	HANDLE file;
	DWORD size;

public:
	BinaryWriter(wstring filePath);
	BinaryWriter(string filePath);
	~BinaryWriter();

	void Int(int data);
	void UInt(UINT data);
	void Float(float data);
	void String(string data);
	void Float4x4(XMFLOAT4X4 data);

	void Byte(void* data, UINT dataSize);
};

////////////////////////////////////////////////////////////////////////////

class BinaryReader
{
private:
	HANDLE file;
	DWORD size;

public:
	BinaryReader(wstring filePath);
	BinaryReader(string filePath);
	~BinaryReader();

	int Int();
	UINT UInt();
	float Float();
	string String();
	XMFLOAT4X4 Matrix();

	void Byte(void** data, UINT dataSize);
};