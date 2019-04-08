#pragma once

#include <cstdint>

#include "SoundData.h"
#include "SoundPlayer.h"

constexpr std::uint32_t SampleRate8272Hz = 802;

const Sound sounds[]
{
	{ sound01Data, SampleRate8272Hz, 4956, 9977 },
	{ sound02Data, SampleRate8272Hz },
	{ sound03Data, SampleRate8272Hz },
	{ sound04Data, SampleRate8272Hz },
	{ sound05Data, SampleRate8272Hz },
	{ sound06Data, SampleRate8272Hz },
	{ sound07Data, SampleRate8272Hz },
	{ sound08Data, SampleRate8272Hz },
	{ sound09Data, SampleRate8272Hz },
	{ sound10Data, SampleRate8272Hz, 1140, 1203 },
	{ sound11Data, SampleRate8272Hz, 1616, 3441 },
	{ sound12Data, SampleRate8272Hz },
	{ sound13Data, SampleRate8272Hz },
	{ sound14Data, SampleRate8272Hz, 346, 909 },
	{ sound15Data, SampleRate8272Hz },
	{ sound16Data, SampleRate8272Hz, 614, 793 },
	{ sound17Data, SampleRate8272Hz },
	{ sound18Data, SampleRate8272Hz },
	{ sound19Data, SampleRate8272Hz },
	{ sound20Data, SampleRate8272Hz },
	{ sound21Data, SampleRate8272Hz, 1072, 6319 },
	{ sound22Data, SampleRate8272Hz },
	{ sound23Data, SampleRate8272Hz, 614, 793 },
	{ sound24Data, SampleRate8272Hz, 672, 3939 },
	{ sound25Data, SampleRate8272Hz, 0, 7171 },
	{ sound26Data, SampleRate8272Hz, 0, 6195 },
	{ sound27Data, SampleRate8272Hz, 0, 6121 },
};