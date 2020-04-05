#pragma once

namespace Path
{
	string ToString(wstring value);
	wstring ToWString(string value);

	bool ExistFile(string path);
	bool ExistDirectory(string path);

	wstring GetExtension(wstring path);
	string GetFileName(string path);
	string GetDirectoryName(string path);
	string GetFileNameWithoutExtension(string path);

	void CreateFolders(string path);

	vector<string> SplitString(string origin, string tok);

	void Replace(string* str, string comp, string rep);
}