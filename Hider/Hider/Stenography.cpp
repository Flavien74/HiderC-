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

Gdiplus::Bitmap* Stenography::LSBEncode(WCHAR fileName, std::string message)
{
	Gdiplus::Bitmap* CypheredBitmap = Gdiplus::Bitmap::FromFile(&fileName);

	int height = CypheredBitmap->GetHeight();
	int width = CypheredBitmap->GetWidth();

	char ch;
	int charIndex = 0;

	ch = message[charIndex];
	int bitCount = 0;
	bool lastChecked = false;
	bool isEncoded = false;

	if (message.length() * 8 > height * width * 3)
	{
		// message too big or image too small
	}

	while (charIndex < message.length())
	{
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				Gdiplus::Color pixelColor;
				CypheredBitmap->GetPixel(x, y, &pixelColor);
				int col[3]{ pixelColor.GetR(),pixelColor.GetG() ,pixelColor.GetB() };

				for (int c = 0; c < 3; c++)
				{
					if (isBitSet(ch, 7 - bitCount))

						col[c] |= 1;
					else
						col[c] &= ~1;

					// increment bit_count to work on next bit
					bitCount++;

					if (bitCount > 8)
					{
						charIndex++;
						ch = message[charIndex];
					}
				}
				// update the image with the changed pixel values
				CypheredBitmap->SetPixel(x, y, Gdiplus::Color(col[0], col[1], col[2]));
			}
		}
	}
	return CypheredBitmap;
}

