#pragma once

#include <functional>
#include <gsl/gsl>
#include <memory>
#include <vector>
#include "halley/text/halleystring.h"
#include "halley/maths/vector2.h"

namespace Halley
{
	class AudioClip;

    namespace AudioConfig {
        constexpr int sampleRate = 48000;
        using SampleFormat = float;
    }

	class AudioDevice
	{
	public:
		virtual ~AudioDevice() {}
		virtual String getName() const = 0;
	};

	struct alignas(64) AudioSamplePack
	{
		std::array<AudioConfig::SampleFormat, 16> samples; // AVX-512 friendly
	};

	enum class AudioSampleFormat
	{
		Undefined,
		Int16,
		Int32,
		Float
	};

	class AudioSpec
	{
	public:
		int sampleRate;
		int numChannels;
		int bufferSize;
		AudioSampleFormat format;

		AudioSpec() {}
		AudioSpec(int sampleRate, int numChannels, int bufferSize, AudioSampleFormat format)
			: sampleRate(sampleRate)
			, numChannels(numChannels)
			, bufferSize(bufferSize)
			, format(format)
		{}
	};

	class AudioOutputAPI
	{
	public:
		virtual ~AudioOutputAPI() {}

		virtual Vector<std::unique_ptr<const AudioDevice>> getAudioDevices() = 0;
		virtual AudioSpec openAudioDevice(const AudioSpec& requestedFormat, const AudioDevice* device = nullptr) = 0;
		virtual void closeAudioDevice() = 0;

		virtual void startPlayback() = 0;
		virtual void stopPlayback() = 0;

		virtual void queueAudio(gsl::span<const AudioSamplePack> data) = 0;
		virtual size_t getQueuedSize() const = 0;
	};

	class IAudioHandle
	{
	public:
		virtual ~IAudioHandle() {}

		virtual void setGain(float gain) = 0;
		virtual void setPosition(Vector2f pos) = 0;
		virtual void setPan(float pan) = 0;

		virtual void stop() = 0;
		virtual bool isPlaying() const = 0;
	};
	using AudioHandle = std::shared_ptr<IAudioHandle>;

	class AudioAPI
	{
	public:
		virtual ~AudioAPI() {}

		virtual Vector<std::unique_ptr<const AudioDevice>> getAudioDevices() = 0;
		virtual void startPlayback(int deviceNumber = 0) = 0;
		virtual void stopPlayback() = 0;

		virtual AudioHandle playUI(std::shared_ptr<AudioClip> clip, float volume = 1.0f, float pan = 0.5f, bool loop = false) = 0;
		virtual AudioHandle playWorld(std::shared_ptr<AudioClip> clip, Vector2f position, float volume = 1.0f, bool loop = false) = 0;

		virtual void setListener(Vector2f position) = 0;
	};
}