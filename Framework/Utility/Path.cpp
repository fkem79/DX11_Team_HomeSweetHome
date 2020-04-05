#include "Framework.h"
#include "Path.h"

string Path::ToString(wstring value)
{
	string temp = "";
	temp.assign(value.begin(), value.end());
	return temp;
}

wstring Path::ToWString(string value)
{
	wstring temp = L"";
	temp.assign(value.begin(), value.end());
	return temp;
}

bool Path::ExistFile(string path)
{
	DWORD fileValue = GetFileAttributesA(path.c_str());

	return fileValue < 0xffffffff;
}

bool Path::ExistDirectory(string path)
{
	DWORD fileValue = GetFileAttributesA(path.c_str());

	BOOL temp = (fileValue != INVALID_FILE_ATTRIBUTES &&
		(fileValue & FILE_ATTRIBUTE_DIRECTORY));

	return temp == TRUE;
}

wstring Path::GetExtension(wstring path)
{
	size_t index = path.find_last_of('.');

	return path.substr(index + 1, path.length());
}

string Path::GetFileName(string path)
{
	Replace(&path, "\\", "/");
	size_t index = path.find_last_of('/');

	return path.substr(index + 1, path.length());
}

string Path::GetDirectoryName(string path)
{
	Replace(&path, "\\", "/");
	size_t index = path.find_last_of('/');

	return path.substr(0, index + 1);
}

string Path::GetFileNameWithoutExtension(string path)
{
	string fileName = GetFileName(path);

	size_t index = fileName.find_last_of('.');

	return fileName.substr(0, index);
}

void Path::CreateFolders(string path)
{
	vector<string> folders = SplitString(path, "/");

	string temp = "";
	for (string folder : folders)
	{
		temp += folder + "/";

		if (!ExistDirectory(temp))
			CreateDirectoryA(temp.c_str(), 0);
	}
}

vector<string> Path::SplitString(string origin, string tok)
{
	vector<string> result;

	size_t cutAt = 0;

	while ((cutAt = origin.find_first_of(tok)) != origin.npos)
	{
		if (cutAt > 0)
			result.push_back(origin.substr(0, cutAt));

		origin = origin.substr(cutAt + 1);
	}

	//if (origin.length() > 0)
		//result.push_back(origin.substr(0, cutAt));

	return result;
}

void Path::Replace(string* str, string comp, string rep)
{
	string temp = *str;

	size_t at = 0;
	while ((at = temp.find(comp, at)) != string::npos)
	{
		temp.replace(at, comp.length(), rep);
		at += rep.length();
	}

	*str = temp;
}
