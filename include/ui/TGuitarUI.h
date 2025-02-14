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

#include <audio/AudioData.h>

namespace ui {
    class Graph {
    public:
        Graph() = default;

        std::vector<int> operator()(int width, int height) const {
            std::vector<int> output(width);

            // normalize between 0 and height giving that we have the max value
            for (int i = 0; i < width && i < data.size(); ++i) {
                const float v = data[i] / max * height;

                output[i] = static_cast<int>(v);
            }
            return output;
        }

        std::vector<float> data;
        float max = 0.0f;
    };

    class TGuitarUI {
    public:
        TGuitarUI();

        virtual ~TGuitarUI();

        void start();

    private:
        std::atomic<float> input_level_;
        std::atomic<float> output_level_;
        std::atomic<bool> running;
        ftxui::ScreenInteractive screen;

        ftxui::Element Header() const;
        ftxui::Element Body() const;
        static ftxui::Element Footer();

        void computeFFT(const std::vector<float> &samplesBlock);

        ftxui::Element theGraph() const;

        void UpdateLoop();
        Graph my_graph_;
    };
}

#endif //TGUITAR_UI_H
