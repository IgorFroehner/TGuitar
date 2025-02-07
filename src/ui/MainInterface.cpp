/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#include <audio/AudioData.h>
#include <ftxui/component/component.hpp>

#include "ui/TGuitarUI.h"

namespace ui {
    TGuitarUI::TGuitarUI() : input_level(0.0f), running(false), screen(ftxui::ScreenInteractive::Fullscreen()) {
    }

    TGuitarUI::~TGuitarUI() = default;

    void TGuitarUI::start() {
        using namespace ftxui;

        const auto renderer = Renderer([this] {
            return vbox({
                Header(),
                filler()
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
                               text("TGuitar") | center | bold | size(WIDTH, EQUAL, 40)
                           ) | border;

        auto header_right = hbox({
                                text("in: "), levelBar(input_level),
                                text("out: "), levelBar(output_level)
                            })
                            | center | border | flex;

        return hbox({
                   header_left,
                   header_right,
               }) | size(HEIGHT, EQUAL, 3);
    }

    void TGuitarUI::UpdateLoop() {
        using namespace std::chrono;
        while (running) {
            // Simulate reading an updated level.
            const float currentInputLevel = audio::inputLevel.load(std::memory_order_relaxed);
            const float currentOutputLevel = audio::outputLevel.load(std::memory_order_relaxed);

            std::this_thread::sleep_for(milliseconds(100));
            input_level = currentInputLevel;
            output_level = currentOutputLevel;

            screen.PostEvent(ftxui::Event::Custom);
        }
    }
}
