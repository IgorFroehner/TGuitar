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

constexpr unsigned int SAMPLE_RATE = 48000; // 44.1 kHz (CD quality)
constexpr unsigned int BUFFER_SIZE = 256; // Frames per buffer

struct AudioData {
    unsigned int inputChannels;
    unsigned int outputChannels;
};

int audioCallback(void *outputBuffer, void *inputBuffer, const unsigned int nFrames,
                  double, const RtAudioStreamStatus status, void *audioData) {
    if (status) {
        std::cout << "Stream over/underflow detected.\n";
    }

    const auto *audioDataIn = static_cast<AudioData *>(audioData);

    const auto *in = static_cast<float *>(inputBuffer);
    const auto out = static_cast<float *>(outputBuffer);

    float peak = 0.0f;

    for (unsigned int i = 0; i < nFrames; i++) {
        const float inputSample = in[i];

        const float absSample = fabs(inputSample);
        peak = std::max(peak, absSample);

        if (audioDataIn->outputChannels == 2) {
            out[i * 2] = inputSample; // Left channel
            out[i * 2 + 1] = inputSample; // Right channel
        } else {
            out[i] = inputSample;
        }
    }

    audio::inputLevel.store(peak, std::memory_order_relaxed);

    return 0;
}

int main() {
    RtAudio audio;

    if (audio.getDeviceCount() < 1) {
        std::cout << "No audio devices found!\n";
        return EXIT_FAILURE;
    }

    ui::printDevicesAvailable(audio);

    auto [inputParams, outputParams] = ui::selectDevices(audio);

    auto audioData = AudioData{inputParams.nChannels, outputParams.nChannels};

    unsigned int bufferFrames = BUFFER_SIZE;

    try {
        audio.openStream(&outputParams, &inputParams, RTAUDIO_FLOAT32, SAMPLE_RATE, &bufferFrames, &audioCallback,
                         &audioData);
        audio.startStream();
    } catch (RtAudioErrorType &e) {
        std::cerr << e << std::endl;
        return EXIT_FAILURE;
    }

    auto ui = new ui::TGuitarUI();
    ui->start();

    try {
        audio.stopStream();
    } catch (RtAudioErrorType &e) {
        std::cerr << e << std::endl;
    }

    if (audio.isStreamOpen()) audio.closeStream();

    return EXIT_SUCCESS;
}
