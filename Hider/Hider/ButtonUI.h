#pragma once
#include "TransformUI.h"
#include <string>
#include <windows.h>

class ObjectUI
{

public :

	ObjectUI();
	ObjectUI(int id, std::string name, TransformUI& transform, HWND* hwnd);

	int m_id;
	std::string m_name;
	TransformUI m_transform;
	HWND* m_hwnd;
};

