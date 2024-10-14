#pragma once
#include <stdint.h>
#include <Windows.h>
#include <gdiplus.h>
#include <string>


class Stenography
{
public:

private:
	bool isBitSet(char ch, int pos);
	Gdiplus::Bitmap* LSBEncode(WCHAR fileName, std::string message);

};