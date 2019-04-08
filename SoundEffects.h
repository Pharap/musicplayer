#pragma once

#include <cstdint>

enum class EffectType : std::uint8_t
{
	Arpeggio,
	PortamentoUp,
	PortamentoDown,
	PortamentoSlide,

	Offset = 0x09,
	VolumeAdjust,
	PositionAdjust,
	SetVolume,
	NextPattern,

	Mystery = 0x0F,
};

enum class Note : std::uint8_t
{
	None,
	C,
	CSharp,
	D,
	DSharp,
	E,
	F,
	FSharp,
	G,
	GSharp,
	A,
	ASharp,
	B,
};

struct SoundEffect
{
	EffectType type;
	std::uint8_t operand;

	SoundEffect() = default;

	constexpr SoundEffect(EffectType type) :
		type(type), operand(0)
	{
	}

	constexpr SoundEffect(EffectType type, std::uint8_t operand) :
		type(type), operand(operand)
	{
	}
};

class TunePart
{
private:
	std::uint8_t noteAndOctave;

	static constexpr std::uint8_t combine(Note note, std::uint8_t octave)
	{
		return static_cast<std::uint8_t>(((static_cast<std::uint8_t>(note) & 0x0F) << 4) | (octave & 0x0F));
	}

public:
	std::uint8_t volume;
	std::uint8_t instrument;
	SoundEffect effect;

public:
	TunePart() = default;

	constexpr TunePart(EffectType type) :
		noteAndOctave(0), volume(0), instrument(0), effect(type)
	{
	}

	constexpr TunePart(EffectType type, std::uint8_t operand) :
		noteAndOctave(0), volume(0), instrument(0), effect(type, operand)
	{
	}

	constexpr TunePart(SoundEffect effect) :
		noteAndOctave(0), volume(0), instrument(0), effect(effect)
	{
	}

	constexpr TunePart(Note note, std::uint8_t octave, std::uint8_t volume, std::uint8_t instrument, SoundEffect effect) :
		noteAndOctave(combine(note, octave)), volume(volume), instrument(instrument), effect(effect)
	{
	}

	constexpr TunePart(Note note, std::uint8_t octave, std::uint8_t volume, std::uint8_t instrument, EffectType type) :
		noteAndOctave(combine(note, octave)), volume(volume), instrument(instrument), effect(type)
	{
	}

	constexpr TunePart(Note note, std::uint8_t octave, std::uint8_t volume, std::uint8_t instrument, EffectType type, uint8_t operand) :
		noteAndOctave(combine(note, octave)), volume(volume), instrument(instrument), effect(type, operand)
	{
	}

	Note getNote() const
	{
		// Upper nibble
		return static_cast<Note>(this->noteAndOctave >> 4);
	}

	std::uint8_t getOctave() const
	{
		// Lower nibble
		return static_cast<std::uint8_t>(this->noteAndOctave & 0x0F);
	}
};