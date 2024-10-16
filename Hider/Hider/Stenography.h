#pragma once
#include <stdint.h>
#include <Windows.h>
#include <gdiplus.h>
#include <string>


class Stenography
{
public:
	Stenography();
	~Stenography();

	void LSBEncode(Gdiplus::Bitmap* bitmap, const wchar_t* message);
	std::wstring LSBDecode(Gdiplus::Bitmap* Decyphered);


private:
	bool isBitSet(char ch, int pos);

};