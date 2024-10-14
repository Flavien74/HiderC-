#pragma once
#include <stdint.h>
#include <Windows.h>
#include <gdiplus.h>
#include <string>


class Stenography
{
public:
	Gdiplus::Bitmap* LSBEncode(WCHAR fileName, std::string message);
	std::string LSBDecode(WCHAR fileName);


private:
	bool isBitSet(char ch, int pos);

};