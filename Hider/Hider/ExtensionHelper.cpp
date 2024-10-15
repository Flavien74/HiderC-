#include "ExtensionHelper.h"

ExtensionHelper::ExtensionHelper() :
	m_extension()
{
}

ExtensionHelper::~ExtensionHelper()
{
}

bool ExtensionHelper::SetExtensionByPath(const std::wstring& path)
{
	size_t dotPos = path.find_last_of(L".");
	if (dotPos == std::wstring::npos) {
		return false; // Pas d'extension trouvée
	}

	size_t slashPos = path.find_last_of(L"\\/");
	if (slashPos == std::wstring::npos) {
		return false; // Pas d'extension trouvée
	}

	m_casePath = CutText(&path, slashPos + 1, 0);
	m_imageName = CutText(&path, dotPos, slashPos + 1);
	m_extension = CutText(&path, dotPos);

	return true;
}

void ExtensionHelper::SetExtensionByString(const std::wstring& extension)
{
	m_extension = extension;
}

const std::wstring ExtensionHelper::GetCompletePath()
{
	return m_casePath + m_imageName + m_extension;
}

const std::wstring ExtensionHelper::GetNewCompletePath(std::wstring suppName)
{
	return m_casePath + m_imageName + suppName + m_extension;
}

const std::wstring ExtensionHelper::CutText(const std::wstring* str, int end, int start)
{
	if (start != -1) 
	{
		return str->substr(start, end - start);
	}
	else 
	{
		return str->substr(end);
	}
}
