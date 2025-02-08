/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#ifndef TGUITAR_UI_H
#define TGUITAR_UI_H

#include <atomic>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

namespace ui {
    class TGuitarUI {
    public:
        TGuitarUI();

        virtual ~TGuitarUI();

        void start();

    private:
        std::atomic<float> input_level;
        std::atomic<float> output_level;
        std::atomic<bool> running;
        ftxui::ScreenInteractive screen;

        ftxui::Element Header() const;
        ftxui::Element page() const;

        void UpdateLoop();
    };
}

#endif //TGUITAR_UI_H
