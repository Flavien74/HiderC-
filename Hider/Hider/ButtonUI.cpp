#include "ButtonUI.h"

ObjectUI::ObjectUI() :
	m_id(0),
	m_name(""),
	m_hwnd(nullptr)
{

}

ObjectUI::ObjectUI(int id, std::string name, TransformUI& transform, HWND* hwnd)
{
	m_id = id;
	m_name = name;
	m_transform = transform;
	m_transformResize = transform;
	m_hwnd = hwnd;
}
