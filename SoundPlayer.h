#pragma once

#include <cstddef>
#include <cstdint>

struct Sound
{
	const unsigned char * data;
	std::uint32_t length;
	std::uint16_t sampleRate;
	std::uint32_t repeatStart;
	std::uint32_t repeatEnd;

	Sound() = default;

	constexpr Sound(const unsigned char * data, std::uint32_t length, int16_t sampleRate) :
		data(data), length(length), sampleRate(sampleRate), repeatStart(0), repeatEnd(0)
	{
	}

	constexpr Sound(const unsigned char * data, std::uint32_t length, int16_t sampleRate, int32_t repeatStart, int32_t repeatEnd) :
		data(data), length(length), sampleRate(sampleRate), repeatStart(repeatStart), repeatEnd(repeatEnd)
	{
	}

	template< std::size_t length >
	constexpr Sound(const unsigned char (&data)[length], int16_t sampleRate) :
		data(data), length(length), sampleRate(sampleRate), repeatStart(0), repeatEnd(0)
	{
	}

	template< std::size_t length >
	constexpr Sound(const unsigned char (&data)[length], int16_t sampleRate, int32_t repeatStart, int32_t repeatEnd) :
		data(data), length(length), sampleRate(sampleRate), repeatStart(repeatStart), repeatEnd(repeatEnd)
	{
	}
};

struct Channel
{
	Sound sound;
	bool enabled;
	std::uint32_t soundPoint;
	int volume;
	int speed;
	int repeat;

	std::uint32_t getSoundLength() const
	{
		return this->sound.length;
	}
};

class SoundPlayer
{
public:
	Channel channels[4];

public:
	void playSound(std::uint8_t channel, const Sound & sound, int volume = 255, int speed = 255);
	void playSound(std::uint8_t channel, const Sound & sound, int volume, int speed, int offset);
};

void SoundPlayer::playSound(std::uint8_t channel, const Sound & sound, int volume, int speed)
{
	std::uint32_t soundLength = sound.length;
	std::uint32_t repeatStart = sound.repeatStart;
	std::uint32_t repeatEnd = sound.repeatEnd;

	// cheat, if there is a looping sample, make the size the length of the loop
	if(repeatEnd != 0)
	{
		soundLength = repeatEnd;
		// also cheat for start loop
		if(repeatStart == 0)
		{
			repeatStart = 1;
		}
	}

	float frequency = (POK_AUD_FREQ / static_cast<float>(sound.sampleRate));

	this->channels[channel].sound = sound;
	this->channels[channel].volume = volume;
	this->channels[channel].speed = (speed / frequency);
	this->channels[channel].soundPoint = 0;
	this->channels[channel].repeat = ((repeatStart << 8) / this->channels[channel].speed);
	this->channels[channel].enabled = true;
}

void SoundPlayer::playSound(std::uint8_t channel, const Sound & sound, int volume, int speed, int offset)
{
	this->playSound(channel, sound, volume, speed);
	this->channels[channel].sound.data += offset;
}