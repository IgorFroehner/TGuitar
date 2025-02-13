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
    class Graph {
    public:
        Graph() {}

        std::vector<int> operator()(int width, int height) const {
            std::vector<int> output(width);
            for (int i = 0; i < width; ++i) {
                float v = 0;
                v += 0.1f * sin((i + shift) * 0.1f);        // NOLINT
                v += 0.2f * sin((i + shift + 10) * 0.15f);  // NOLINT
                v += 0.1f * sin((i + shift) * 0.03f);       // NOLINT
                v *= height;                                // NOLINT
                v += 0.5f * height;                         // NOLINT
                output[i] = static_cast<int>(v);
            }
            return output;
        }
        std::vector<double> data;
        int shift = 0;
    };

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
        ftxui::Element Body() const;
        ftxui::Element Footer() const;

        void computeFFT(const std::vector<float> &samplesBlock) const;

        ftxui::Element theGraph() const;

        void UpdateLoop();
        Graph my_graph;
    };
}

#endif //TGUITAR_UI_H
