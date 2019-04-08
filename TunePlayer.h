#pragma once

#include <cstdint>

#include "SoundPlayer.h"
#include "SoundEffects.h"
#include "Sounds.h"
#include "TuneData.h"

class TunePlayer
{
private:
	SoundPlayer soundPlayer;
	Ticker timer;
	bool enabled;
	bool hasJumped;
	std::uint32_t pattern = 0;
	std::uint32_t currentLine = 0;

protected:
	void update();

public:
	void play(float frequency)
	{
		this->timer.attach(callback(this, &TunePlayer::update), frequency);
	}

	void stop()
	{
		this->timer.detach();
	}

	void enable()
	{
		this->enabled = true;
	}

	void disable()
	{
		this->enabled = false;
	}
};

void TunePlayer::update()
{
	if(!this->enabled)
		return;

	std::uint8_t pat = tunePattern[this->pattern];

	for(std::size_t channelIndex = 0; channelIndex < 4; ++channelIndex) {
		TunePart part = tune[pat][this->currentLine][channelIndex];
		
		// max volume from xm/mod was 64
		std::uint32_t volume = part.volume;
		std::uint8_t instrument = part.instrument;
		EffectType type = part.effect.type;
		std::uint8_t operand = part.effect.operand;

		int offset = 0;

		// if valid volume change, change it, if not use previous
		if(volume >=128) {
			volume -=128;
			volume *= 4;
		}else{
			// don't change volume
			volume = soundPlayer.channels[channelIndex].volume;
		}

		switch(type)
		{
			case EffectType::Arpeggio:
				// can't be implemented because of my timing method
				// I'm changing the timer instead of counting the ticks.
				break;
			case EffectType::PortamentoUp:
				soundPlayer.channels[channelIndex].speed += operand;
				break;
			case EffectType::PortamentoDown:
				soundPlayer.channels[channelIndex].speed -= operand;
				break;
			case EffectType::PortamentoSlide:
				// not a clue what this does or how wrong it is.
				//soundPlayer.channels[t].speed = (oldNote<<8) / operand;
				break;
			case EffectType::Offset:
				offset = ((operand << 8) / soundPlayer.channels[channelIndex].speed);
				break;
			case EffectType::VolumeAdjust:
				if(operand > 15)
					volume += (operand >> 4);
				else
					volume -= (operand & 0x0F);
				break;
			case EffectType::PositionAdjust:
				if(!this->hasJumped)
				{
					this->hasJumped = true;
					this->currentLine = operand;
				}
				break;
			case EffectType::SetVolume:
				volume = (operand * 4);
				break;
			case EffectType::NextPattern:
				this->currentLine = operand;

				if(this->pattern < (sizeof(tunePattern) - 1))
					++this->pattern;
				else
					this->pattern = 0;

				break;
			case EffectType::Mystery:
				// F in MOD format. 0.02 = 1 'tick'
				// any value below 32 changes the number of 'ticks' per note
				this->stop();

				if(operand <32)
					this->play(0.02 * operand);
				else
					// any value above 32 sets beats per minute
					this->play(10.0 / operand);

				break;
		}

		if(volume > 255)
			volume = 255;

		const Note note = part.getNote();
		const std::uint8_t octave = part.getOctave();

		if(note != Note::None)
		{
			std::uint32_t speed = (getNoteSpeed(note) << octave);

			if((instrument >= 1) && (instrument <= 27))
			{
				const Sound & sound = sounds[instrument];
				soundPlayer.playSound(channelIndex, sound, volume, speed, offset);
			}
		}

		soundPlayer.channels[channelIndex].volume = volume;
	}

	// patterns are 64 lines long, or until type code 20.
	if(this->currentLine < 63)
		++this->currentLine;
	else
	{
		this->currentLine = 0;
		this->hasJumped = false;

		if(this->pattern < (sizeof(tunePattern) - 1))
			++this->pattern;
		else
			this->pattern = 0;
	}
}