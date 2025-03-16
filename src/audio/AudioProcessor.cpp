/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#include "audio/AudioProcessor.h"

#include <iostream>
#include <audio/AudioData.h>

#include <audio/Utils.h>
#include <audio/Constants.h>
#include <audio/Globals.h>

namespace audio {
    AudioProcessor *AudioProcessor::instance_ = nullptr;
    std::mutex AudioProcessor::mutex_;

    AudioProcessor::AudioProcessor() : metronome_(Metronome()) {
    }

    AudioProcessor::~AudioProcessor() {
        if (audio_.isStreamOpen()) audio_.closeStream();
    }

    AudioProcessor *AudioProcessor::GetInstance() {
        std::lock_guard lock(mutex_);
        if (instance_ == nullptr) {
            instance_ = new AudioProcessor();
        }
        return instance_;
    }

    void AudioProcessor::setInputStreamParameters(const RtAudio::StreamParameters &parameters) {
        input_stream_parameters_ = parameters;

        audio_data_.inputChannels = parameters.nChannels;
    }

    void AudioProcessor::setOutputStreamParameters(const RtAudio::StreamParameters &parameters) {
        output_stream_parameters_ = parameters;

        audio_data_.outputChannels = parameters.nChannels;
    }

    void AudioProcessor::applyEffects(const unsigned int nFrames, float *buffer) const {
        for (const auto &effect: effects_) {
            effect->applyEffect(nFrames, buffer);
        }
    }

    void AudioProcessor::addEffect(std::unique_ptr<Effect> effect) {
        effects_.push_back(std::move(effect));
    }

    void AudioProcessor::setMetronome(const bool value) {
        metronome_.setActive(value);
    }

    void AudioProcessor::setBPM(const unsigned bpm) {
        metronome_.setBPM(bpm);
    }

    float AudioProcessor::mixIn(const unsigned currentCount) const {
        return metronome_.mixIn(currentCount);
    }

    int audioCallback(void *outputBuffer, void *inputBuffer, const unsigned int nFrames,
                      double, const RtAudioStreamStatus status, void *audioData) {
        if (status) {
            std::cout << "Stream over/underflow detected.\n";
        }

        const auto *audio_data_in = static_cast<AudioData *>(audioData);

        auto *in = static_cast<float *>(inputBuffer);
        const auto out = static_cast<float *>(outputBuffer);

        float peak = 0.0f;

        for (unsigned int i = 0; i < nFrames; i++) {
            const float abs_sample = fabs(in[i]);
            peak = std::max(peak, abs_sample);
        }

        runTimeValues.input_level.store(peak, std::memory_order_relaxed);

        const auto processor = AudioProcessor::GetInstance();
        processor->applyEffects(nFrames, in);
        float peak_out = 0.0f;

        for (unsigned int i = 0; i < nFrames; i++) {
            float processed_sample = clip(in[i]);

            float abs_out_sample = fabs(processed_sample);
            peak_out = std::max(peak_out, abs_out_sample);

            g_FFTBuffer.push_back(processed_sample);

            if (g_FFTBuffer.size() >= FFT_SIZE) {
                g_FFTReady.store(true, std::memory_order_relaxed);
            }

            const auto current_count = g_MetronomeCounter.fetch_add(1, std::memory_order_relaxed);
            processed_sample = processor->mixIn(current_count);

            if (audio_data_in->outputChannels == 2) {
                out[i * 2] = processed_sample;
                out[i * 2 + 1] = processed_sample;
            } else {
                out[i] = processed_sample;
            }
        }

        runTimeValues.output_level.store(peak_out, std::memory_order_relaxed);

        return 0;
    }

    void AudioProcessor::runStream(const std::function<void()> &interfaceFunction) {
        unsigned int buffer_frames = BUFFER_SIZE;

        try {
            audio_.openStream(&output_stream_parameters_, &input_stream_parameters_, RTAUDIO_FLOAT32, SAMPLE_RATE,
                              &buffer_frames, &audioCallback, &audio_data_);
            audio_.startStream();
        } catch (RtAudioErrorType &e) {
            throw;
        }

        interfaceFunction();

        try {
            audio_.stopStream();
        } catch (RtAudioErrorType &e) {
            std::cerr << e << std::endl;
        }

        if (audio_.isStreamOpen()) audio_.closeStream();
    }

    const std::vector<std::unique_ptr<Effect> > &AudioProcessor::getEffects() const {
        return effects_;
    }
}
