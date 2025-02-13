/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#include <audio/AudioData.h>
#include <ftxui/component/component.hpp>

#include "ui/TGuitarUI.h"

#include <iostream>

namespace ui {
    ftxui::Element TGuitarUI::theGraph() const {
        using namespace ftxui;
        using namespace std::chrono_literals;

        auto document = vbox({
                            graph(std::ref(my_graph)) | flex,
                        }) | flex | border;

        return document;
    }

    TGuitarUI::TGuitarUI() : input_level(0.0f), output_level(0.0f), running(false),
                             screen(ftxui::ScreenInteractive::Fullscreen()) {
    }

    TGuitarUI::~TGuitarUI() = default;

    void TGuitarUI::start() {
        using namespace ftxui;

        const auto renderer = Renderer([this] {
            return vbox({
                Header(),
                Body(),
                Footer(),
            });
        });

        const auto eventWrapper = CatchEvent(renderer, [this](const Event &event) {
            if (event == Event::Return || event == Event::CtrlC ||
                event == Event::Character('q')) {
                screen.ExitLoopClosure()();
                running = false;
                return true;
            }
            return false;
        });

        running = true;

        // Start a thread to update the level periodically.
        std::thread updater(&TGuitarUI::UpdateLoop, this);

        // Run the event loop.
        screen.Loop(eventWrapper);

        updater.join();
    }

    ftxui::Element levelBar(const float level) {
        using namespace ftxui;
        constexpr int width = 40;
        const int percentage = static_cast<int>(level * width);

        return hbox({
            text("["),
            text(std::string(percentage, '=')) | color(Color::Green),
            text(std::string(width - percentage, ' ')),
            text("] "),
        });
    }

    ftxui::Element TGuitarUI::Header() const {
        using namespace ftxui;
        auto header_left = hbox(
                               text("🎸 TGuitar") | center | bold | size(WIDTH, EQUAL, 40)
                           ) | border;

        auto header_right = hbox({
                                text("🎤in: "), levelBar(input_level),
                                text("🔈out: "), levelBar(output_level)
                            })
                            | center | border | flex;

        return hbox({
                   header_left,
                   header_right,
               }) | size(HEIGHT, EQUAL, 3);
    }

    ftxui::Element TGuitarUI::Body() const {
        using namespace ftxui;

        const auto sidebar = vbox({
                                 text("Sidebar Content") | center,
                             })
                             | border
                             | size(WIDTH, EQUAL, 40)
                             | flex;

        const auto main_content = theGraph() | flex;

        return hbox({
                   sidebar,
                   main_content,
               })
               | flex;
    }

    ftxui::Element TGuitarUI::Footer() const {
        using namespace ftxui;
        return filler();
    }

    // void TGuitarUI::computeFFT(const std::vector<float> &samplesBlock) const {
    //     // Ensure samplesBlock has at least FFT_SIZE samples
    //     if (samplesBlock.size() < FFT_SIZE) {
    //         std::cerr << "Not enough samples for FFT" << std::endl;
    //         return;
    //     }
    //
    //     // 3. Fill the input buffer, applying a window function (Hann window)
    //     for (int i = 0; i < FFT_SIZE; i++) {
    //         double hannWindow = 0.5 * (1 - cos((2 * M_PI * i) / (FFT_SIZE - 1)));
    //         fftInput[i] = samplesBlock[i] * hannWindow;
    //     }
    //
    //     // 4. Execute the FFT
    //     fftw_execute(fftPlan);
    //
    //     // 5. Process the FFT output: compute magnitude for each bin
    //     // The output is an array of FFT_SIZE/2+1 complex numbers.
    //     std::vector<double> magnitudes(FFT_SIZE / 2 + 1);
    //     for (int k = 0; k <= FFT_SIZE / 2; k++) {
    //         double real = fftOutput[k][0];
    //         double imag = fftOutput[k][1];
    //         magnitudes[k] = std::sqrt(real * real + imag * imag);
    //     }
    //
    //     std::vector<double> decibels(FFT_SIZE / 2 + 1);
    //     for (int k = 0; k <= FFT_SIZE / 2; k++) {
    //         decibels[k] = (magnitudes[k] > 1e-10) ? 20 * log10(magnitudes[k]) : -100;
    //     }
    //
    //     my_graph.data = decibels;
    // }

    void TGuitarUI::UpdateLoop() {
        using namespace std::chrono;
        while (running) {
            const float currentInputLevel = audio::inputLevel.load(std::memory_order_relaxed);
            const float currentOutputLevel = audio::outputLevel.load(std::memory_order_relaxed);

            std::this_thread::sleep_for(milliseconds(50));
            input_level = currentInputLevel;
            output_level = currentOutputLevel;

            my_graph.shift++;

            // if (audio::fftReady.load(std::memory_order_relaxed)) {
            //     computeFFT(fftBuffer);
            //
            //     audio::fftReady.store(false,std::memory_order_relaxed);
            //     fftBuffer.clear();
            // }

            screen.PostEvent(ftxui::Event::Custom);
        }
    }
}
