#include "Stenography.h"

// Checks whether the bit is set or not at a particular position.
// Returns true if set
// Returns false if not set
bool Stenography::isBitSet(char ch, int pos)
{
	ch = ch >> pos;
	if (ch & 1)
		return true;
	return false;
}

Stenography::Stenography()
{

}

Stenography::~Stenography()
{

}

void Stenography::LSBEncode(Gdiplus::Bitmap* CypheredBitmap, const wchar_t* message)
{
	int height = CypheredBitmap->GetHeight();
	int width = CypheredBitmap->GetWidth();

	char ch;
	int charIndex = 0;

	ch = message[charIndex];
	int bitCount = 0;

	if (wcslen(message) * 8 > height * width * 3)
	{
		// message too big or image too small
	}

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			Gdiplus::Color pixelColor;
			CypheredBitmap->GetPixel(x, y, &pixelColor);
			int col[3]{ pixelColor.GetR(),pixelColor.GetG() ,pixelColor.GetB() };

			for (int c = 0; c < 3; c++)
			{
				col[c] &= 0xFE;

				if (isBitSet(ch, 7 - bitCount))
					col[c] |= 1;

				// increment bit_count to work on next bit
				bitCount++;

				if (bitCount == 8)
				{
					charIndex++;
					if (charIndex >= wcslen(message))
					{
						CypheredBitmap->SetPixel(x, y, Gdiplus::Color(col[0], col[1], col[2]));
						return;
					}
					ch = message[charIndex];
					bitCount = 0;
				}
				// update the image with the changed pixel values
				CypheredBitmap->SetPixel(x, y, Gdiplus::Color(col[0], col[1], col[2]));
			}
		}
	}
}

std::wstring Stenography::LSBDecode(Gdiplus::Bitmap* Decyphered)
{
	int height = Decyphered->GetHeight();
	int width = Decyphered->GetWidth();

	char ch = 0;
	std::wstring message;

	int bitCount = 0;

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			Gdiplus::Color pixelColor;
			Decyphered->GetPixel(x, y, &pixelColor);
			int col[3]{ pixelColor.GetR(),pixelColor.GetG() ,pixelColor.GetB() };

			for (int c = 0; c < 3; c++)
			{
				if (isBitSet(col[c], 0))	ch |= 1;

				// increment bit_count to work on next bit
				bitCount++;

				if (bitCount == 8)
				{
					if (ch == -1)
					{
						return message;
					}
					message.push_back(ch);
					bitCount = 0;
					ch = 0;
				}
				else
				{
					ch = ch << 1;
				}
			}
		}
	}
	return message;
}



