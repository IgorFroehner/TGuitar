/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#include <iostream>
#include <rtaudio/RtAudio.h>
#include <thread>
#include <random>

#include <ui/TGuitarUI.h>
#include <ui/InitialMenu.h>
#include <audio/AudioData.h>
#include <audio/AudioProcessor.h>
#include <audio/DelayEffect.h>
#include <audio/DistortionEffect.h>

// #include <fftw3.h>

audio::AudioProcessor processor;

struct AudioData {
    unsigned int inputChannels;
    unsigned int outputChannels;
};

float clip(const float sample) {
    if (sample > 1.0f) return 1.0f;
    if (sample < -1.0f) return -1.0f;
    return sample;
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

    audio::inputLevel.store(peak, std::memory_order_relaxed);

    processor.applyEffects(nFrames, in);
    float peak_out = 0.0f;

    for (unsigned int i = 0; i < nFrames; i++) {
        const float input_sample = clip(in[i]);

        float abs_out_sample = fabs(input_sample);
        peak_out = std::max(peak_out, abs_out_sample);

        audio::g_FFTBuffer.push_back(input_sample);

        if (audio::g_FFTBuffer.size() >= FFT_SIZE) {
            audio::g_FFTReady.store(true, std::memory_order_relaxed);
        }

        if (audio_data_in->outputChannels == 2) {
            out[i * 2] = input_sample;
            out[i * 2 + 1] = input_sample;
        } else {
            out[i] = input_sample;
        }
    }

    audio::outputLevel.store(peak_out, std::memory_order_relaxed);

    return 0;
}

int main() {
    RtAudio audio;

    if (audio.getDeviceCount() < 1) {
        std::cout << "No audio devices found!\n";
        return EXIT_FAILURE;
    }

    ui::printDevicesAvailable(audio);

    auto [input_params, output_params] = ui::selectDevices(audio);

    auto audioData = AudioData{input_params.nChannels, output_params.nChannels};

    unsigned int buffer_frames = BUFFER_SIZE;

    auto distortion = audio::DistortionEffect(1.0f);

    processor.addEffect(std::make_unique<audio::DistortionEffect>(distortion));

    auto delay = audio::DelayEffect(0.2, 0.2);

    processor.addEffect(std::make_unique<audio::DelayEffect>(delay));

    try {
        audio.openStream(&output_params, &input_params, RTAUDIO_FLOAT32, SAMPLE_RATE, &buffer_frames, &audioCallback,
                         &audioData);
        audio.startStream();
    } catch (RtAudioErrorType &e) {
        std::cerr << e << std::endl;
        return EXIT_FAILURE;
    }

    const auto ui = new ui::TGuitarUI();
    ui->start();

    try {
        audio.stopStream();
    } catch (RtAudioErrorType &e) {
        std::cerr << e << std::endl;
    }

    if (audio.isStreamOpen()) audio.closeStream();

    return EXIT_SUCCESS;
}
