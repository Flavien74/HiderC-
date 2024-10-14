#pragma once
#include <stdint.h>
#include <Windows.h>
#include <gdiplus.h>
#include <string>


class Stenography
{
public:
	Gdiplus::Bitmap* LSBEncode(const wchar_t* fileName, std::string message);
	std::string LSBDecode(Gdiplus::Bitmap* Decyphered);


private:
	bool isBitSet(char ch, int pos);

};