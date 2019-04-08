#include <Pokitto.h>
#include <HWSound.h>

#include <cstdint>

#include "gfx.h"

Pokitto::Core mygame;
#include "drawing.h"

#include "TunePlayer.h"

TunePlayer tunePlayer;

const std::uint16_t* currentPal;

/**************************************************************************/

// print text
void print(int16_t x, std::uint16_t y, const char * text)
{
	const std::size_t textLength = strlen(text);

	if(x == -1)
	{
		int width = 0;

		for(std::size_t index = 0; index < textLength; ++index)
		{
			unsigned char character = static_cast<unsigned char>(text[index]);
			width += letterWidths[character - 32];
		}

		x = (110 - (width / 2));
	}

    // 0 = 8bit
    // 1 = 4bit
    // 2 = 2bit
    /*for(int t=y/2; t<(y/2)+8; t++) {
        Pokitto::Display::scanType[t]=2; // 2bit
    }*/

	for(std::size_t index = 0; index < textLength; ++index)
	{
		unsigned char character = static_cast<unsigned char>(text[index]);
		drawBitmap(x, y, largefont[character - 32], 2);
		x += letterWidths[character - 32];
	}
}

void drawPic(int16_t x, int16_t y, const std::uint8_t * bitmap, int16_t bpp)
{
	using Pokitto::Display;

	int h = *(bitmap + 1); // second byte is height

	/*for(int index = (y / 2); index < (y + (h * 2)) / 2; ++index) {
		Display::scanType[index] = 1; // 4bit
	}*/


	int y1 = y;
	for(int i = y; i < (y + h); ++i) {
		drawBitmapLines(x, y1, i - y , 1, bitmap, bpp);
		++y1;
	}
}

int main ()
{
	using Pokitto::Display;
	using Pokitto::Core;
	using Pokitto::Sound;

	Core::begin();
	Core::setFrameRate(255);
	Display::persistence=1;

	// Init audio stream.
	pokPlayStream(); // activate stream
	Sound::ampEnable(true);
	Sound::playMusicStream();

	Display::setInvisibleColor(0);
	load2bitPalette(tinyfont_pal, 0, 4);

	tunePlayer.enable();
	tunePlayer.play(0.08);

	while (Core::isRunning())
	{
		if (!Core::update())
			continue;

		print(0, 0, "Hello World!");
	}
}

