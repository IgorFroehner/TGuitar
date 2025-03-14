/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#include <ftxui/component/component.hpp>
#include <complex>
#include <iostream>

#include <audio/AudioData.h>
#include <audio/Constants.h>
#include <audio/Globals.h>
#include <ui/TGuitarUI.h>
#include <math/dj_fft.h>

namespace ui {
    TGuitarUI::TGuitarUI() : running(false), screen(ftxui::ScreenInteractive::Fullscreen()) {
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

        const auto event_wrapper = CatchEvent(renderer, [this](const Event &event) {
            if (event == Event::Return || event == Event::CtrlC ||
                event == Event::Character('q')) {
                screen.ExitLoopClosure()();
                running = false;
                return true;
            }
            if (event == Event::m) {
                const auto metronome = audio::runTimeValues.metronome.load(std::memory_order_relaxed);
                const auto new_metronome = !metronome;

                audio::runTimeValues.metronome.store(new_metronome, std::memory_order_relaxed);

                const auto processor = audio::AudioProcessor::GetInstance();
                processor->setMetronome(new_metronome);
            }
            if (event == Event::j) {
                const auto bpm = audio::runTimeValues.metronome_bpm.load(std::memory_order_relaxed);

                const auto new_bpm = std::min(255u, bpm + 1);

                const auto processor = audio::AudioProcessor::GetInstance();
                processor->setBPM(new_bpm);

                audio::runTimeValues.metronome_bpm.store(new_bpm, std::memory_order_relaxed);
            }
            if (event == Event::k) {
                const auto bpm = audio::runTimeValues.metronome_bpm.load(std::memory_order_relaxed);

                const auto new_bpm = std::max(1u, bpm - 1);

                const auto processor = audio::AudioProcessor::GetInstance();
                processor->setBPM(new_bpm);

                audio::runTimeValues.metronome_bpm.store(new_bpm, std::memory_order_relaxed);
            }
            return false;
        });

        running = true;

        // Start a thread to update the UI periodically.
        std::thread updater(&TGuitarUI::UpdateLoop, this);

        // Run the event loop.
        screen.Loop(event_wrapper);

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
        auto header_left = hbox({
                               filler(),
                               text("🎸 TGuitar") | bold,
                               filler(),
                           }) | border | size(WIDTH, EQUAL, 40);

        const auto input_level = audio::runTimeValues.input_level.load(std::memory_order_relaxed);
        const auto output_level = audio::runTimeValues.input_level.load(std::memory_order_relaxed);
        const auto metronome = audio::runTimeValues.metronome.load(std::memory_order_relaxed);
        const auto metronome_bpm = audio::runTimeValues.metronome_bpm.load(std::memory_order_relaxed);

        auto header_right = hbox({
                                text("🎤in: "), levelBar(input_level),
                                text("🔈out: "), levelBar(output_level),
                                text("Metronome: "),
                                text(metronome ? std::to_string(metronome_bpm) + " BPM" : "OFF"),
                            })
                            | center | border | flex;

        return hbox({
                   header_left,
                   header_right,
               }) | size(HEIGHT, EQUAL, 3);
    }

    ftxui::Element TGuitarUI::theGraph() const {
        using namespace ftxui;

        auto graph_container = vbox({
                                   hbox({
                                       graph(std::ref(frequency_graph_))
                                   }) | flex,
                               }) | flex | border;

        return graph_container;
    }

    ftxui::Element TGuitarUI::Body() const {
        using namespace ftxui;

        const auto sidebar = vbox({
                                 text("Sidebar Content") | center,
                             })
                             | border
                             | size(WIDTH, EQUAL, 40);

        const auto main_content = theGraph() | flex;

        return hbox({
                   sidebar,
                   main_content,
               })
               | flex;
    }

    ftxui::Element TGuitarUI::Footer() {
        using namespace ftxui;

        return vbox({
                   hbox({
                       text("m: toggle metronome  |  j: reduce metronome BPM  |  k: increase metronome BPM | q: quit")
                   })
               }) |
               size(HEIGHT, EQUAL, 1);
    }

    void TGuitarUI::computeFFT(const std::vector<float> &samplesBlock) {
        // Ensure samplesBlock has at least FFT_SIZE samples
        if (samplesBlock.size() < FFT_SIZE) {
            std::cerr << "Not enough samples for FFT" << std::endl;
            return;
        }

        std::vector<std::complex<float> > fftInput;
        fftInput.reserve(FFT_SIZE);

        // 3. Fill the input buffer, applying a window function (Hann window)
        for (size_t i = 0; i < FFT_SIZE; i++) {
            double hannWindow = 0.5 * (1 - cos((2.0f * M_PI * static_cast<float>(i)) / (FFT_SIZE - 1)));
            fftInput.emplace_back(samplesBlock[i] * hannWindow);
        }

        // 4. Execute the FFT
        auto fftResult = fft1d(fftInput, dj::fft_dir::DIR_FWD);

        // 5. Process the FFT output: compute magnitude for each bin
        // The output is an array of FFT_SIZE/2+1 complex numbers.
        frequency_graph_.data.clear();
        float max = -1.0f;
        for (size_t i = 0; i < FFT_SIZE / 2 + 1; i++) {
            float magnitude = std::abs(fftResult[i]);
            max = std::max(max, magnitude);
            frequency_graph_.data.push_back(magnitude);
        }

        frequency_graph_.max = max;
    }

    void TGuitarUI::UpdateLoop() {
        using namespace std::chrono;

        while (running) {
            std::this_thread::sleep_for(milliseconds(50));

            if (audio::g_FFTReady.load(std::memory_order_relaxed)) {
                computeFFT(audio::g_FFTBuffer);

                audio::g_FFTReady.store(false, std::memory_order_relaxed);
                audio::g_FFTBuffer.clear();
            }

            screen.PostEvent(ftxui::Event::Custom);
        }
    }
}
