/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#include <ftxui/component/component.hpp>

#include "ui/TGuitarUI.h"
#include "../Globals.hpp"

namespace ui {
    TGuitarUI::TGuitarUI() : level(0.0f), running(false), screen(ftxui::ScreenInteractive::Fullscreen()) {
    }

    TGuitarUI::~TGuitarUI() = default;

    void TGuitarUI::start() {
        using namespace ftxui;

        auto renderer = Renderer([this] {
            return vbox({
                Header(),
                filler()
            });
        });

        auto eventWrapper = CatchEvent(renderer, [this](const Event &event) {
            if (event == Event::Return || event == Event::CtrlC ||
                event == Event::Character('q')) {
                screen.ExitLoopClosure()();
                running = false;
                return true;
            }
            return false;
        });

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
                                text("in: "), levelBar(level),
                                text("out: "), levelBar(level)
                            })
                            | center | border | flex;

        return hbox({
                   header_left,
                   header_right,
               }) | size(HEIGHT, EQUAL, 3);
    }

    // Update loop that periodically reads gInputLevel and refreshes the UI.
    void TGuitarUI::UpdateLoop() {
        using namespace std::chrono;
        while (running) {
            // Simulate reading an updated level.
            float currentLevel = globals::gInputLevel.load(std::memory_order_relaxed);

            std::this_thread::sleep_for(milliseconds(100));
            level = currentLevel;
            screen.PostEvent(ftxui::Event::Custom);
        }
    }
}
