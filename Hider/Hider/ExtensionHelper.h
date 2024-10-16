#pragma once
#include <string>

class ExtensionHelper
{

public:
	std::wstring m_extension;
	std::wstring m_imageName;
	std::wstring m_casePath;

	ExtensionHelper();
	~ExtensionHelper();

	bool SetExtensionByPath(const std::wstring& path);
	const std::wstring GetCompletePath(std::wstring suppName = L"");

private :
	const std::wstring CutText(const std::wstring* str, int end, int start = -1);
};

