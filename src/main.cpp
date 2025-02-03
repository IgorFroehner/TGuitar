#include <iostream>
#include <vector>
#include <rtaudio/RtAudio.h>
#include <thread>
#include <random>

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

// #include "audio/AudioProcessor.h"

#include "ui/InitialMenu.h"

constexpr unsigned int SAMPLE_RATE = 48000; // 44.1 kHz (CD quality)
constexpr unsigned int BUFFER_SIZE = 256; // Frames per buffer

std::atomic gInputLevel(0.0f);

int audioCallback(void *outputBuffer, void *inputBuffer, const unsigned int nFrames,
                  double, const RtAudioStreamStatus status, void *) {
    if (status) {
        std::cout << "Stream over/underflow detected.\n";
    }

    const auto *in = static_cast<float *>(inputBuffer);
    const auto out = static_cast<float *>(outputBuffer);

    float peak = 0.0f;

    for (unsigned int i = 0; i < nFrames; i++) {
        const float inputSample = in[i];

        const float absSample = fabs(inputSample);
        if (absSample > peak) {
            peak = absSample;
        }

        out[i * 2] = inputSample; // Left channel
        out[i * 2 + 1] = inputSample; // Right channel
    }

    gInputLevel.store(peak, std::memory_order_relaxed);

    return 0;
}

using namespace ftxui;

Element levelBar(float level) {
    int width = 40;
    int percentage = static_cast<int>(level * width);

    std::string filled(percentage, ' ');
    std::string empty(width - percentage, ' ');

    return hbox({
        text("["),
        text(std::string(percentage, '=')) | color(Color::Green), // Filled part
        text(std::string(width - percentage, ' ')), // Empty part
        text("] "),
    });
}

int main() {
    RtAudio audio;

    if (audio.getDeviceCount() < 1) {
        std::cout << "No audio devices found!\n";
        return 1;
    }

    ui::printDevicesAvailable(audio);

    const auto deviceIds = audio.getDeviceIds();

    unsigned int inputDeviceId, outputDeviceId;
    std::cout << "Input device id: ";
    std::cin >> inputDeviceId;
    auto inputParams = ui::selectDevice(inputDeviceId, 1, deviceIds);

    std::cout << "Output device id: ";
    std::cin >> outputDeviceId;
    auto outputParams = ui::selectDevice(outputDeviceId, 2, deviceIds);

    unsigned int bufferFrames = BUFFER_SIZE;

    try {
        audio.openStream(&outputParams, &inputParams, RTAUDIO_FLOAT32, SAMPLE_RATE, &bufferFrames, &audioCallback);
        audio.startStream();
    } catch (RtAudioErrorType &e) {
        std::cerr << e << std::endl;
        return 1;
    }

    auto screen = ScreenInteractive::Fullscreen();

    float progress = 0.0f;
    std::atomic running = true;

    const auto renderer = Renderer([&] {
        return vbox({
                   filler(),
                   hbox({filler(), text("input level: "), levelBar(progress), filler()}),
                   hbox({filler(), text("output level: "), levelBar(progress), filler()}),
                   filler(),
               }) | border | center;
    });

    std::thread updater([&] {
        while (running) {
            const float currentLevel = gInputLevel.load(std::memory_order_relaxed);

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            progress = currentLevel;
            screen.PostEvent(Event::Custom);
        }
    });

    const auto eventWrapper = CatchEvent(renderer, [&](const Event &event) {
        // exit when enter pressed
        if (event == Event::Return) {
            screen.ExitLoopClosure()();
            running = false;
            return true;
        }
        return false;
    });

    screen.Loop(eventWrapper);
    updater.join();

    try {
        audio.stopStream();
    } catch (RtAudioErrorType &e) {
        std::cerr << e << std::endl;
    }

    if (audio.isStreamOpen()) audio.closeStream();

    return 0;
}
